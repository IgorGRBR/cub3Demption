/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   floor_b.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ihhrabar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/20 19:34:53 by ihhrabar          #+#    #+#             */
/*   Updated: 2024/06/20 19:34:54 by ihhrabar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "cub3d_core.h"
#include "ydefines.h"
#include "ytypes.h"
#include "yvec2.h"
#include <math.h>

t_grid_node	*_sample_floor_pos(t_scene *scene, t_dvec2 spos[3],
				t_ivec2 *gpos)
{
	t_uint		i;
	t_grid_node	*gn;

	i = 0;
	while (i < 3)
	{
		*gpos = dvec2_floor(spos[i]);
		gn = scene_node_at_i2(scene, *gpos);
		if (gn && (gn->type == GNT_FLOOR || gn->type == GNT_DOOR))
			return (gn);
		i++;
	}
	return (YNULL);
}

void	draw_ceil_floor_colors(t_scene *scene, double begin, double end)
{
	t_ivec2	i;
	int		half_size;
	t_ivec2	lims;

	lims.x = (int)floor((double)scene->rc_target->size.x * begin);
	lims.y = (int)floor((double)scene->rc_target->size.x * end);
	half_size = scene->rc_target->size.y / 2;
	i = ivec2_new(lims.x - 1, -1);
	while (++i.y < half_size)
	{
		i.x = lims.x - 1;
		while (++i.x < lims.y)
			image_set_pixel_unsafe(scene->rc_target, i.x, i.y,
				scene->default_assets.top);
	}
	i = ivec2_new(lims.x - 1, half_size - 1);
	while (++i.y < scene->rc_target->size.y)
	{
		i.x = lims.x - 1;
		while (++i.x < lims.y)
			image_set_pixel_unsafe(scene->rc_target, i.x, i.y,
				scene->default_assets.bottom);
	}
}
