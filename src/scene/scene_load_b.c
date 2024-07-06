/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene_load_b.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ihhrabar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 16:45:17 by ihhrabar          #+#    #+#             */
/*   Updated: 2024/05/21 16:45:18 by ihhrabar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "cub3d_core.h"
#include "ycstr.h"
#include "ydefines.h"
#include "ylist.h"
#include "ymath.h"
#include "ystring.h"
#include "ytypes.h"
#include "yvec2.h"
#include <stdlib.h>

static t_bool	cub_configure_color(t_scene *scene, t_list *str_data)
{
	t_list		*cols;
	int			cv[3];
	t_color		*col;
	t_bool		result;

	col = YNULL;
	result = FALSE;
	if (string_equal_cstr(list_get(str_data, 0), "C"))
		col = &scene->default_assets.top;
	else if (string_equal_cstr(list_get(str_data, 0), "F"))
		col = &scene->default_assets.bottom;
	cols = string_split(list_get(str_data, 1), ',');
	if (cols->size == 3 && col
		&& cstr_try_atoi(((t_string *)list_get(cols, 0))->cstr, &cv[0])
		&& cstr_try_atoi(((t_string *)list_get(cols, 1))->cstr, &cv[1])
		&& cstr_try_atoi(((t_string *)list_get(cols, 2))->cstr, &cv[2]))
	{
		col->r = cv[0];
		col->g = cv[1];
		col->b = cv[2];
		result = TRUE;
	}
	return (list_apply(cols, (t_apply_lfn)string_delete),
		list_delete(cols), result);
}

t_bool	cub_configure_texture(t_scene *scene, t_game *game,
			t_list *str_data)
{
	t_string	*tex_id;
	t_string	*tex_path;
	t_image		**i;

	tex_id = list_get(str_data, 0);
	tex_path = list_get(str_data, 1);
	i = YNULL;
	if (!tex_id || !tex_path)
		return (FALSE);
	if (string_equal_cstr(tex_id, "EA"))
		i = &scene->default_assets.wall_tex[DIR_EAST];
	else if (string_equal_cstr(tex_id, "WE"))
		i = &scene->default_assets.wall_tex[DIR_WEST];
	else if (string_equal_cstr(tex_id, "NO"))
		i = &scene->default_assets.wall_tex[DIR_NORTH];
	else if (string_equal_cstr(tex_id, "SO"))
		i = &scene->default_assets.wall_tex[DIR_SOUTH];
	else
		return (cub_configure_color(scene, str_data));
	if (*i && *i != assets_get_image(game->assets, "default_wall"))
		image_delete(*i, game->window_ctx);
	return (*i = image_from_tga(game->window_ctx, tex_path->cstr), TRUE);
}

static t_ivec2	get_dimensions(t_list_iter it)
{
	int		width;
	int		height;
	int		llen;
	t_bool	end;

	width = ((t_string *)it.value)->size;
	height = 1;
	end = FALSE;
	while (list_iter_next(&it))
	{
		llen = ((t_string *)it.value)->size;
		if (llen > 0 && end)
			error_exit("map data cannot contain empty lines");
		else if (llen == 0)
			end = TRUE;
		width = int_max(width, llen);
		height++;
	}
	return (ivec2_new(width, height));
}

static t_bool	handle_lines(t_scene *scene, t_game *game, t_map *binds,
					t_list_iter cub_it)
{
	t_ivec2		coord;
	t_bool		cont;
	t_string	*bind;

	coord = ivec2_zero();
	cont = TRUE;
	while (cont && list_iter_next(&cub_it))
	{
		coord.x = 0;
		while (cont && (t_uint)coord.x < ((t_string *)cub_it.value)->size)
		{
			bind = string_from_cstr(" ");
			bind->cstr[0] = ((t_string *)cub_it.value)->cstr[coord.x];
			cont = _handle_bind((t_scene_init_args){scene, game, binds},
					scene_node_at(scene, coord.x, coord.y), bind, coord);
			string_delete(bind);
			coord.x++;
		}
		coord.y++;
	}
	return (cont);
}

t_bool	_scene_load_grid(t_scene *scene, t_game *game, t_map *binds,
			t_list_iter cub_it)
{
	t_bool		result;
	t_ivec2		i;
	t_grid_node	*gn;

	scene->grid_size = get_dimensions(cub_it);
	scene->grid = (t_grid_node *)malloc(sizeof (t_grid_node)
			* scene->grid_size.x * scene->grid_size.y);
	(list_iter_prev(&cub_it), result = TRUE, i = ivec2_zero());
	while (i.x < scene->grid_size.x)
	{
		i.y = 0;
		while (i.y < scene->grid_size.y)
		{
			gn = scene_node_at(scene, i.x, i.y);
			gn->type = GNT_EMPTY;
			gn->phys_bodies = YNULL;
			gn->vis_ents = YNULL;
			i.y++;
		}
		i.x++;
	}
	result = handle_lines(scene, game, binds, cub_it);
	return (result);
}
