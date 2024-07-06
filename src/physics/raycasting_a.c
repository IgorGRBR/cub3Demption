/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycasting_a.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ihhrabar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 17:18:27 by ihhrabar          #+#    #+#             */
/*   Updated: 2024/06/13 17:18:29 by ihhrabar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "cub3d_core.h"
#include "ytypes.h"
#include "yvec3.h"
#include "yvec2.h"
#include <math.h>
#include "ymath.h"
#include "ylist.h"

static void	handle_door(t_raycast *rc, t_rc_res *rs, t_grid_node *gn)
{
	double	dist;
	t_dvec2	hit_pos;

	if (gn->data.door.vertical)
		dist = rc->ray_len.x - rc->ray_unit_step.x / 2.0;
	else
		dist = rc->ray_len.y - rc->ray_unit_step.y / 2.0;
	if (dist < 0)
		return ;
	hit_pos = dvec2_add(dvec3_xz(rs->ray_pos),
			dvec2_scale(rc->ray_dir, dist));
	if (!ivec2_eq(dvec2_floor(hit_pos), rc->map_check))
		return ;
	rs->hit_pos.x = hit_pos.x;
	rs->hit_pos.z = hit_pos.y;
	rs->hit_pos.y = rs->ray_dir.y * dist;
	rs->hit = TRUE;
}

static void	process_hit(t_raycast *rc, t_rc_res *rs,
	t_grid_node *gn, t_pb_rc_config cfg)
{
	t_dvec2	hit_pos;

	rs->hit_body = (t_phys_body){
		.half_radius = dvec2_new(0.5, 0.5),
		.pos = dvec2_new((double)rc->map_check.x + 0.5,
			(double)rc->map_check.y + 0.5),
		.vel = dvec2_zero(),
		.par_type = PBP_NODE,
		.parent = gn
	};
	if (gn->type == GNT_DOOR)
	{
		if (!gn->data.door.open || (cfg.type & PBP_DOOR))
			return (handle_door(rc, rs, gn));
		return ;
	}
	hit_pos = dvec2_add(dvec3_xz(rs->ray_pos),
			dvec2_scale(rc->ray_dir, rc->ray_dist));
	rs->hit_pos.x = hit_pos.x;
	rs->hit_pos.z = hit_pos.y;
	rs->hit_pos.y = rs->ray_dir.y * rc->ray_dist;
	rs->hit = TRUE;
}

// TODO: check the height of the ray at phys body position.
static void	col_vslist(t_raycast *rc, t_rc_res *rs, t_grid_node *gn,
				t_pb_rc_config cfg)
{
	t_list_iter	it;
	t_phys_body	*pb;
	t_dvec2		ray_norm;
	t_dvec2		diff;
	double		hit_dist[2];

	if (!gn || !gn->phys_bodies)
		return ;
	it = list_iter(gn->phys_bodies);
	hit_dist[0] = rc->ray_dist + 1.0;
	while (list_iter_next(&it))
	{
		pb = it.value;
		diff = dvec2_sub(pb->pos, dvec3_xz(rs->ray_pos));
		ray_norm = dvec2_normalized(dvec2_new(-rs->ray_dir.z, rs->ray_dir.x));
		hit_dist[1] = dvec2_dot(rc->ray_dir, diff);
		if (double_abs(dvec2_dot(diff, ray_norm)) < pb->half_radius.x
			&& hit_dist[1] < hit_dist[0] && (cfg.type & pb->par_type))
		{
			hit_dist[0] = hit_dist[1];
			*rs = (t_rc_res){(*pb), TRUE, dvec3_add(rs->ray_pos,
					dvec3_scale(dvec3_new(rc->ray_dir.x, 0.0, rc->ray_dir.y),
						hit_dist[0])), rs->ray_dir, rs->ray_pos};
		}
	}
}

static void	ray_traversal(t_scene *scene, t_raycast *rc, t_rc_res *rs,
				t_pb_rc_config cfg)
{
	t_grid_node	*gn;

	gn = scene_node_at_i2(scene, rc->map_check);
	if (gn)
	{
		if (node_is_physical(gn) && (cfg.type & (PBP_NODE | PBP_DOOR)))
			process_hit(rc, rs, gn, cfg);
		(void)((gn->phys_bodies) && (col_vslist(rc, rs, gn, cfg), 1));
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
	if (rc->ray_dist <= cfg.max_len && !rs->hit)
		ray_traversal(scene, rc, rs, cfg);
}

t_rc_res	scene_cast_ray(t_scene *scene, t_dvec3 pos, t_dvec3 dir,
			t_pb_rc_config cfg)
{
	t_rc_res			res;
	t_raycast			rc;

	rc.ray_dir = dvec3_xz(dir);
	rc.ray_unit_step = dvec2_new(cfg.max_len, cfg.max_len);
	if (dir.x != 0.0)
		rc.ray_unit_step.x = sqrt(1.0 + (dir.z / dir.x) * (dir.z / dir.x));
	if (dir.z != 0.0)
		rc.ray_unit_step.y = sqrt(1.0 + (dir.x / dir.z) * (dir.x / dir.z));
	rc.map_check = dvec2_floor(dvec3_xz(pos));
	rc.step = ivec2_new(double_sign(dir.x), double_sign(dir.z));
	rc.ray_len.x = ((double)(rc.map_check.x + 1)
			- pos.x) * rc.ray_unit_step.x;
	if (dir.x < 0.0)
		rc.ray_len.x = (pos.x - (double)rc.map_check.x)
			* rc.ray_unit_step.x;
	rc.ray_len.y = ((double)(rc.map_check.y + 1)
			- pos.z) * rc.ray_unit_step.y;
	if (dir.z < 0.0)
		rc.ray_len.y = (pos.z - (double)rc.map_check.y)
			* rc.ray_unit_step.y;
	rc.ray_dist = 0.0;
	res = (t_rc_res){.hit = FALSE, .ray_dir = dir, .ray_pos = pos};
	ray_traversal(scene, &rc, &res, cfg);
	return (res);
}
