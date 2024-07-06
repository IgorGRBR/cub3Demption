/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycasting_d.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ihhrabar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/21 14:09:59 by ihhrabar          #+#    #+#             */
/*   Updated: 2024/06/21 14:10:00 by ihhrabar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "yvec2.h"
#include "yvec3.h"

static void	map_textures(t_uint i, t_raycast *rc,
				t_raycast_query *rq, t_grid_node *gn)
{
	if (gn->data.door.vertical)
		rq->doors[i].tex_offset = (double)(rc->map_check.y + 1)
			- rq->doors[i].pos.y;
	else
		rq->doors[i].tex_offset = (double)(rc->map_check.x + 1)
			- rq->doors[i].pos.x;
	if (gn->data.door.vertical)
		rq->doors[i].tex_offset = 1.0 - rq->doors[i].tex_offset;
	rq->doors[i].visible = (rq->doors[i].tex_offset <= 1.0
			&& rq->doors[i].tex_offset >= 0.0);
}

void	_door_ray_rc(t_scene *scene, t_raycast *rc,
			t_raycast_query *rq, t_grid_node *gn)
{
	t_uint	i;
	double	dist;

	if (rq->hit_node)
		return ;
	i = rq->door_count;
	if (i >= RC_DOOR_COUNT)
		return ;
	rq->doors[i].node = gn;
	if (gn->data.door.vertical)
		dist = rc->ray_len.x - rc->ray_unit_step.x / 2.0;
	else
		dist = rc->ray_len.y - rc->ray_unit_step.y / 2.0;
	if (dist < 0)
		return ;
	rq->door_count++;
	rq->doors[i].pos = dvec2_add(dvec3_xz(scene->camera.pos),
			dvec2_scale(rc->ray_dir, dist));
	map_textures(i, rc, rq, gn);
}

void	compute_texture_scaling(double col[6], t_dvec2 limits,
			double scale_factor, double tex_size_ratio)
{
	col[3] = limits.x;
	col[4] = limits.y;
	col[0] = -((col[3] - col[4]) * scale_factor) / 2.0 + limits.y;
	col[1] = ((col[4]) * scale_factor) / 2.0 + limits.y;
	col[2] = (col[1] - col[0]) * tex_size_ratio;
}
