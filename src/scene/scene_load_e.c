/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene_load_e.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ihhrabar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/21 14:10:11 by ihhrabar          #+#    #+#             */
/*   Updated: 2024/06/21 14:10:13 by ihhrabar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "ycstr.h"
#include "ylist.h"
#include "ystring.h"

static void	load_textures(t_game *game, t_image *texs[4], t_list_iter *it)
{
	t_string	*tex_name;

	(list_iter_next(it), tex_name = it->value);
	texs[0] = assets_get_image(game->assets, tex_name->cstr);
	(list_iter_next(it), tex_name = it->value);
	texs[1] = assets_get_image(game->assets, tex_name->cstr);
	(list_iter_next(it), tex_name = it->value);
	texs[2] = assets_get_image(game->assets, tex_name->cstr);
	(list_iter_next(it), tex_name = it->value);
	texs[3] = assets_get_image(game->assets, tex_name->cstr);
}

static t_bool	handle_door(t_scene_init_args args, t_grid_node *node,
			t_list_iter *it)
{
	t_image		*texs[4];
	t_string	*tex_name;
	int			v;

	load_textures(args.game, texs, it);
	(list_iter_next(it), tex_name = it->value);
	v = 0;
	if (!tex_name || !cstr_try_atoi(tex_name->cstr, &v)
		|| !texs[0] || !texs[2] || !texs[3])
		error_exit("one of the mandatory door textures or params. not found");
	node->type = GNT_DOOR;
	(list_iter_next(it), tex_name = it->value);
	if (!tex_name)
		error_exit("door type not set");
	node->data.door = (t_grid_door_node){args.scene->default_assets.bottom,
		args.scene->default_assets.top, texs[0], texs[1], FALSE, v == 1,
		door_type_from_char_str(tex_name), texs[2], texs[3]};
	return (TRUE);
}

t_bool	_handle_bind2(t_scene_init_args args, t_grid_node *node,
			t_list_iter *it, t_ivec2 coords)
{
	t_string	*s;

	s = it->value;
	(void)coords;
	if (string_equal_cstr(s, "door"))
		return (handle_door(args, node, it));
	return (TRUE);
}
