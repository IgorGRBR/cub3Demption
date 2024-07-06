/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycasting_c.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ihhrabar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 18:47:35 by ihhrabar          #+#    #+#             */
/*   Updated: 2024/06/04 18:47:36 by ihhrabar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "cub3d_core.h"
#include "ydefines.h"
#include "ymap.h"
#include "ymath.h"
#include "yvec2.h"
#include "yvec3.h"
#include <math.h>

static void	process_hit(t_scene *scene, t_raycast *rc, t_raycast_query *rq,
	t_grid_node *gn)
{
	if (rq->hit_node || (gn->type != GNT_SOLID && gn->type != GNT_EXIT))
		return ;
	rq->hit_node = gn;
	rq->hit_dist = rc->ray_dist;
	if (rc->vertical)
		(void)(((rc->step.y > 0) && (rq->hit_dir = DIR_NORTH))
			|| (rq->hit_dir = DIR_SOUTH));
	else
		(void)(((rc->step.x > 0) && (rq->hit_dir = DIR_WEST))
			|| (rq->hit_dir = DIR_EAST));
	rq->hit_pos = dvec2_add(dvec3_xz(scene->camera.pos),
			dvec2_scale(rc->ray_dir, rq->hit_dist));
	rq->tex_offset = rq->hit_pos.x - (double)rc->map_check.x;
	if (rq->hit_dir == DIR_EAST || rq->hit_dir == DIR_WEST)
		rq->tex_offset = rq->hit_pos.y - (double)rc->map_check.y;
	if (rq->hit_dir == DIR_NORTH || rq->hit_dir == DIR_EAST)
		rq->tex_offset = 1.0 - rq->tex_offset;
}

static void	col_vslist(t_raycast_query *rq, t_grid_node *gn)
{
	if (gn->vis_ents)
		map_insert(rq->vis_ent_lists, gn->vis_ents,
			gn->vis_ents);
}

static void	ray_traversal(t_scene *scene, t_raycast *rc, t_raycast_query *rq)
{
	t_grid_node	*gn;

	gn = scene_node_at_i2(scene, rc->map_check);
	if (gn)
	{
		(void)((gn->type == GNT_FLOOR) && (col_vslist(rq, gn), 1));
		(void)((gn->type == GNT_DOOR) && (_door_ray_rc(scene, rc, rq, gn), 1));
		process_hit(scene, rc, rq, gn);
	}
	if (rc->ray_len.x < rc->ray_len.y)
	{
		rc->map_check.x += rc->step.x;
		rc->ray_dist = rc->ray_len.x;
		rc->ray_len.x += rc->ray_unit_step.x;
		rc->vertical = FALSE;
	}
	else
	{
		rc->map_check.y += rc->step.y;
		rc->ray_dist = rc->ray_len.y;
		rc->ray_len.y += rc->ray_unit_step.y;
		rc->vertical = TRUE;
	}
	if (rc->ray_dist <= RAYCAST_DISTANCE_LIMIT)
		ray_traversal(scene, rc, rq);
}

static void	init_query(t_raycast_query *rq)
{
	t_uint	i;

	rq->vis_ent_lists = map_new(YNULL, YNULL);
	rq->hit_dist = 0.0;
	rq->hit_pos = dvec2_zero();
	rq->hit_dir = DIR_COUNT;
	rq->hit_node = YNULL;
	rq->tex_offset = 0.0;
	i = 0;
	while (i < RC_DOOR_COUNT)
	{
		rq->doors[i].node = YNULL;
		i++;
	}
	rq->door_count = 0;
}

t_raycast_query	cast_ray(t_scene *scene, t_dvec2 ray)
{
	t_raycast_query	rq;
	t_raycast		rc;

	rc.ray_dir = ray;
	rc.ray_unit_step = dvec2_new(RAYCAST_DISTANCE_LIMIT,
			RAYCAST_DISTANCE_LIMIT);
	if (ray.x != 0.0)
		rc.ray_unit_step.x = sqrt(1.0 + (ray.y / ray.x) * (ray.y / ray.x));
	if (ray.y != 0.0)
		rc.ray_unit_step.y = sqrt(1.0 + (ray.x / ray.y) * (ray.x / ray.y));
	rc.map_check = dvec2_floor(dvec3_xz(scene->camera.pos));
	rc.step = ivec2_new(double_sign(ray.x), double_sign(ray.y));
	rc.ray_len.x = ((double)(rc.map_check.x + 1)
			- scene->camera.pos.x) * rc.ray_unit_step.x;
	if (ray.x < 0.0)
		rc.ray_len.x = (scene->camera.pos.x - (double)rc.map_check.x)
			* rc.ray_unit_step.x;
	rc.ray_len.y = ((double)(rc.map_check.y + 1)
			- scene->camera.pos.z) * rc.ray_unit_step.y;
	if (ray.y < 0.0)
		rc.ray_len.y = (scene->camera.pos.z - (double)rc.map_check.y)
			* rc.ray_unit_step.y;
	(init_query(&rq), rc.vertical = FALSE, rc.ray_dist = 0.0);
	ray_traversal(scene, &rc, &rq);
	return (rq);
}
