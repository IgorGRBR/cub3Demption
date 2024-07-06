/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycasting_b.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ihhrabar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 18:47:31 by ihhrabar          #+#    #+#             */
/*   Updated: 2024/06/04 18:47:32 by ihhrabar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "ydefines.h"
#include "ylist.h"
#include "ymap.h"
#include "yprint.h"
#include "yvec2.h"
#include "yvec3.h"
#include <math.h>
// #include "yprint.h"

void	collect_vis_ents(t_map *ents, t_map *new_ents)
{
	t_map_iter	it;

	it = map_iter(new_ents);
	while (map_iter_next(&it))
		map_insert(ents, it.key, it.value);
	map_delete(new_ents);
}

static t_raycast_query	raycast_column(t_scene *scene, double cast_offset)
{
	t_dvec2			ray;
	t_dvec2			plane;
	t_raycast_query	rq;

	ray = scene->camera.dir;
	plane = dvec2_new(-ray.y, ray.x);
	ray = dvec2_add(ray, dvec2_scale(plane,
				scene->camera.plane_hwidth * cast_offset));
	ray = dvec2_normalized(ray);
	rq = cast_ray(scene, ray);
	rq.cos_angle = dvec2_dot(ray, scene->camera.dir);
	return (rq);
}

// t_uint	cringe = 0;
// if (vis_ent_list.size != cringe)
// {
			// cringe++;
// 	yprintf("cringe detected: %d, %d\n", vis_ent_list.size, cringe);
// }
static void	draw_ents(t_scene *scene, t_map *vis_ent_lists, t_dvec2 minmax,
				double interp)
{
	t_list			vis_ent_list;
	t_map_iter		mit;
	t_list_iter		lit;
	t_grid_node		*gn;

	gn = scene_node_at_i2(scene, dvec2_floor(
				dvec3_xz(scene->camera.pos)));
	if (gn)
		map_insert(vis_ent_lists, gn->vis_ents,
			gn->vis_ents);
	(list_init(&vis_ent_list), mit = map_iter(vis_ent_lists));
	while (map_iter_next(&mit))
	{
		lit = list_iter(mit.value);
		while (list_iter_next(&lit))
			list_insert(&vis_ent_list, lit.value);
	}
	lit = list_iter(&vis_ent_list);
	while (list_iter_next(&lit))
		vis_ent_draw(lit.value, scene, minmax, interp);
	list_deinit(&vis_ent_list);
}

void	raycast_region(t_scene *scene, double begin,
			double end, double interp)
{
	t_ivec2			pos;
	t_map			vis_ent_lists;
	t_raycast_query	rq;
	double			off;

	map_init(&vis_ent_lists, YNULL, YNULL);
	pos.x = (int)floor(scene->rc_target->size.x * begin);
	pos.y = (int)floor(scene->rc_target->size.x * end);
	while (pos.x < pos.y)
	{
		off = 2.0 * ((double)pos.x / (double)(scene->rc_target->size.x) - 0.5);
		rq = raycast_column(scene, off);
		draw_query_result(scene, rq, pos.x);
		collect_vis_ents(&vis_ent_lists, rq.vis_ent_lists);
		pos.x++;
	}
	draw_ents(scene, &vis_ent_lists, dvec2_new(begin, end), interp);
	map_deinit(&vis_ent_lists);
}
