/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   assets_loading.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ihhrabar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/09 16:25:20 by ihhrabar          #+#    #+#             */
/*   Updated: 2024/05/09 16:25:24 by ihhrabar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "cub3d_core.h"
#include "ycstr.h"
#include "yio.h"
#include "ylist.h"
#include "ymap.h"
#include "ystring.h"
#include "ytypes.h"
#include "fcntl.h"
#include "yvec2.h"
#include <stdlib.h>
#include <unistd.h>

static t_bool	load_image(t_list_iter *it, t_wctx wctx, t_asset_col *col)
{
	t_string	*name;
	t_string	*path;
	int			x_off;
	int			y_off;

	(list_iter_next(it), name = it->value);
	(list_iter_next(it), path = it->value);
	x_off = 0;
	y_off = 0;
	if (list_iter_next(it)
		&& cstr_try_atoi(((t_string *)it->value)->cstr, &x_off))
	{
		if (!list_iter_next(it)
			|| !cstr_try_atoi(((t_string *)it->value)->cstr, &y_off))
		{
			y_off = x_off;
			list_iter_prev(it);
		}
	}
	else
		(void)(it->value && (list_iter_prev(it), 1));
	return (assets_load_image(col, wctx, (const char *[]){name->cstr,
			path->cstr}, ivec2_new(x_off, y_off)));
}

static t_bool	load_frame(t_list_iter *it, t_asset_col *col)
{
	t_string	*name;
	t_string	*img;
	t_direction	i;
	t_frame		*frame;
	t_kv_pair	kv;

	(list_iter_next(it), name = it->value);
	frame = frame_new();
	i = DIR_EAST;
	while (i < DIR_COUNT)
	{
		(list_iter_next(it), img = it->value);
		if (!img)
			return (FALSE);
		(*frame)[i] = assets_get_image(col, img->cstr);
		i++;
	}
	kv = map_get_pair(col->frames, name->cstr);
	if (kv.key)
		(free(kv.key), frame_delete(kv.value));
	map_set(col->frames, cstr_dup(name->cstr), frame);
	return (TRUE);
}

static t_bool	load_asset(t_list_iter *it, t_wctx wctx, t_asset_col *col)
{
	if (string_equal_cstr(it->value, "image"))
		return (load_image(it, wctx, col));
	else if (string_equal_cstr(it->value, "frame"))
		return (load_frame(it, col));
	else if (string_equal_cstr(it->value, "sprite"))
		return (_assets_load_sprite(it, col));
	else
		return (FALSE);
	return (TRUE);
}

static t_bool	parse_tokens(t_list *tokens, t_wctx wctx, t_asset_col *col)
{
	t_list_iter	it;

	it = list_iter(tokens);
	while (list_iter_next(&it))
	{
		if (!load_asset(&it, wctx, col))
			return (FALSE);
	}
	return (TRUE);
}

void	assets_load_from_file(t_asset_col *col, t_wctx wctx,
			const char *path)
{
	int			ifd;
	t_string	*col_str;
	t_list		*tokens;
	t_bool		result;

	if (!try_open_file_cstr(path, O_RDONLY, &ifd))
		error_exit("Couldn't open specified asset collection file");
	col_str = read_file_to_string(ifd);
	close(ifd);
	tokens = tokenize_str(col_str);
	string_delete(col_str);
	result = parse_tokens(tokens, wctx, col);
	list_apply(tokens, (t_apply_lfn)string_delete);
	list_delete(tokens);
	if (!result)
		error_exit("Error while parsing the asset collection file");
}
