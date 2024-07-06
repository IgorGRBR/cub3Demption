/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene_load_c.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ihhrabar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 16:45:25 by ihhrabar          #+#    #+#             */
/*   Updated: 2024/05/21 16:45:26 by ihhrabar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "cub3d_core.h"
#include "ydefines.h"
#include "ylist.h"
#include "ymap.h"
#include "ystring.h"
#include "ytypes.h"

static t_bool	handle_wall(t_game *game, t_scene *scene, t_grid_node *node,
					t_list_iter *it)
{
	t_image		*wall_texture;
	t_string	*tex_name;
	t_uint		i;

	i = 0;
	node->type = GNT_SOLID;
	while (i < DIR_COUNT)
	{
		(list_iter_next(it), wall_texture = YNULL, tex_name = it->value);
		if (!tex_name)
			error_exit("incomplete amount of arguments for a wall");
		wall_texture = assets_get_image(game->assets, tex_name->cstr);
		if (!wall_texture)
			wall_texture = scene->default_assets.wall_tex[i];
		if (!wall_texture)
			error_exit("wall texture not found");
		node->data.wall.texture[i] = wall_texture;
		i++;
	}
	return (TRUE);
}

static t_bool	handle_space(t_scene_init_args args, t_grid_node *node,
					t_list_iter *it)
{
	t_image		*texs[2];
	t_string	*tex_name;

	(list_iter_next(it), tex_name = it->value);
	texs[0] = assets_get_image(args.game->assets, tex_name->cstr);
	(list_iter_next(it), tex_name = it->value);
	texs[1] = assets_get_image(args.game->assets, tex_name->cstr);
	if (!texs[0])
		error_exit("mandatory floor texture not found");
	node->type = GNT_FLOOR;
	node->data.floor.bottom_texture = texs[0];
	node->data.floor.top_texture = texs[1];
	node->data.floor.bottom_color = args.scene->default_assets.bottom;
	node->data.floor.top_color = args.scene->default_assets.top;
	return (TRUE);
}

static t_bool	handle_exit(t_game *game, t_scene *scene, t_grid_node *node,
					t_list_iter *it)
{
	t_bool	result;

	result = handle_wall(game, scene, node, it);
	node->type = GNT_EXIT;
	return (result);
}

t_bool	_handle_bind(t_scene_init_args args, t_grid_node *node, t_string *bind,
			t_ivec2 coords)
{
	t_list		*bind_args;
	t_list_iter	it;
	t_bool		result;

	bind_args = map_get(args.binds, bind);
	if (!bind_args)
		return (string_equal_cstr(bind, " "));
	it = list_iter(bind_args);
	(list_iter_next(&it), list_iter_next(&it), result = TRUE);
	while (result && list_iter_next(&it))
	{
		if (string_equal_cstr(it.value, "wall"))
			result = result && handle_wall(args.game, args.scene, node, &it);
		else if (string_equal_cstr(it.value, "exit"))
			result = result && handle_exit(args.game, args.scene, node, &it);
		else if (string_equal_cstr(it.value, "space"))
			result = result && handle_space(args, node, &it);
		else if (string_equal_cstr(it.value, "object"))
			result = result && _handle_object(args, &it, coords);
		else
			result = result && _handle_bind2(args, node, &it, coords);
	}
	return (result);
}

t_bool	_load_custom_assets(t_game *game, t_list *tokens)
{
	t_string	*path;

	path = list_get(tokens, 1);
	if (!path)
		return (FALSE);
	assets_load_from_file(game->assets, game->window_ctx, path->cstr);
	return (TRUE);
}
