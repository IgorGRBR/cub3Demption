/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene_load_a.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ihhrabar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 16:45:08 by ihhrabar          #+#    #+#             */
/*   Updated: 2024/05/21 16:45:09 by ihhrabar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d_core.h"
#include "cub3d.h"
#include "yio.h"
#include "ylist.h"
#include "ymap.h"
#include "yprint.h"
#include "ystring.h"
#include "ytypes.h"
#include <fcntl.h>
#include <stdlib.h>

static t_bool	set_bind(t_list *tokens, t_map *binds)
{
	t_list		*t_copy;
	t_string	*i_copy;
	t_kv_pair	old;

	if (!list_get(tokens, 1))
		return (FALSE);
	i_copy = string_copy(list_get(tokens, 1));
	old = map_get_pair(binds, i_copy);
	t_copy = list_map(tokens, (t_map_lfn)string_copy);
	map_set(binds, i_copy, t_copy);
	if (old.value)
	{
		string_delete(old.key);
		list_apply(old.value, (t_apply_lfn)string_delete);
		list_delete(old.value);
	}
	return (TRUE);
}

static t_uint	handle_line(t_scene *scene, t_game *game, t_list *tokens,
					t_map *binds)
{
	t_string	*type;

	type = list_get(tokens, 0);
	if (tokens && !type)
		return (TRUE);
	if (string_equal_cstr(type, "cub"))
		return (set_bind(tokens, binds));
	else if (string_equal_cstr(type, "assets"))
		return (_load_custom_assets(game, tokens));
	else if (string_equal_cstr(type, "NO")
		|| string_equal_cstr(type, "SO")
		|| string_equal_cstr(type, "EA")
		|| string_equal_cstr(type, "WE")
		|| string_equal_cstr(type, "F")
		|| string_equal_cstr(type, "C"))
		return (cub_configure_texture(scene, game, tokens));
	return (FALSE);
}

static t_bool	further_processing(t_scene *scene, t_game *game,
					t_list *cub_cnt, t_map *binds)
{
	t_list_iter	it;
	t_string	*line;
	t_list		*tokens;
	t_bool		result;

	scene_init(scene, game);
	result = TRUE;
	it = list_iter(cub_cnt);
	while (result && list_iter_next(&it))
	{
		line = string_copy(it.value);
		tokens = tokenize_str(line);
		result = handle_line(scene, game, tokens, binds);
		list_apply(tokens, (t_apply_lfn)string_delete);
		(list_delete(tokens), string_delete(line));
	}
	if (!result)
		return (_scene_load_grid(scene, game, binds, it));
	return (FALSE);
}

void	map_cleanup(t_string *key, t_list *values)
{
	string_delete(key);
	list_apply(values, (t_apply_lfn)string_delete);
	list_delete(values);
}

t_scene	*scene_from_cub_file(t_game *game, const char *cub_path)
{
	t_scene		*result;
	t_list		*cub_contents;
	t_map		*binds;

	yprintf("Loading level from %s...\n", cub_path);
	result = (t_scene *) malloc(sizeof (t_scene));
	cub_contents = read_file_to_lines_cstr(cub_path, O_RDONLY);
	if (!cub_contents)
		return (free(result), (void *)0);
	binds = _scene_default_binds();
	if (!further_processing(result, game, cub_contents, binds))
		(scene_delete(result, game), result = (void *)0);
	map_apply(binds, (t_apply_mfn)map_cleanup);
	list_apply(cub_contents, (t_apply_lfn)string_delete);
	(list_delete(cub_contents), map_delete(binds));
	_scene_validate(result);
	scene_init_assets(result, game);
	return (result);
}
