/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   floor_a.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ihhrabar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 18:47:13 by ihhrabar          #+#    #+#             */
/*   Updated: 2024/06/04 18:47:14 by ihhrabar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "cub3d_core.h"
#include "ytypes.h"
#include "yvec2.h"
#include "yvec3.h"
#include <math.h>

static void	draw_floor_pixel(t_scene *scene, t__fdd *fdd, t_dvec2 l, t_dvec2 r)
{
	t_ivec2		cell;
	t_grid_node	*gn;
	t_image		*img;
	double		depth;

	gn = _sample_floor_pos(scene, (t_dvec2 []){fdd->floor_pos, l, r},
			&cell);
	depth = color_as_depth(image_get_pixel(scene->rc_depth, fdd->px.x,
				fdd->px.y));
	if (!gn || fdd->row_dist > depth)
		return ;
	if (gn->type == GNT_FLOOR)
		img = gn->data.floor.bottom_texture;
	else
		img = gn->data.door.bottom_texture;
	if (!img)
		return ;
	cell.x = (int)floor((double)img->size.x
			* (fdd->floor_pos.x - (double)cell.x));
	cell.y = (int)floor((double)img->size.y
			* (fdd->floor_pos.y - (double)cell.y));
	image_set_pixel(scene->rc_target, fdd->px.x, fdd->px.y,
		image_get_pixel(img, cell.x, cell.y));
	image_set_pixel(scene->rc_depth, fdd->px.x, fdd->px.y, depth_as_color(fdd->row_dist));
}

static void	draw_ceil_pixel(t_scene *scene, t__fdd *fdd, t_dvec2 l, t_dvec2 r)
{
	t_ivec2		cell;
	t_grid_node	*gn;
	t_image		*img;
	double		depth;

	gn = _sample_floor_pos(scene, (t_dvec2 []){fdd->floor_pos, l, r},
			&cell);
	depth = color_as_depth(image_get_pixel(scene->rc_depth, fdd->px.x,
				fdd->px.y));
	if (!gn || fdd->row_dist > depth)
		return ;
	if (gn->type == GNT_FLOOR)
		img = gn->data.floor.top_texture;
	else
		img = gn->data.door.top_texture;
	if (!img)
		return ;
	cell.x = (int)floor((double)img->size.x
			* (fdd->floor_pos.x - (double)cell.x));
	cell.y = img->size.y - 1 - (int)floor((double)img->size.y
			* (fdd->floor_pos.y - (double)cell.y));
	image_set_pixel(scene->rc_target, fdd->px.x, fdd->px.y,
		image_get_pixel(img, cell.x, cell.y));
	image_set_pixel(scene->rc_depth, fdd->px.x, fdd->px.y, depth_as_color(fdd->row_dist));
}

// fdd.floor_pos = dvec2_sub(fdd.floor_pos, dvec2_scale(fdd.floor_step, 0.5));
static void	draw_floor_line(t_scene *scene, t__fdd fdd)
{
	int		limit;
	t_dvec2	prev;
	t_dvec2	next;

	if (fdd.px.y >= scene->rc_target->size.y)
		return ;
	fdd.floor_step = dvec2_scale(dvec2_sub(fdd.tl.b, fdd.tl.a),
			fdd.row_dist / ((double)(scene->rc_target->size.x)
				* (fdd.end - fdd.start)));
	fdd.floor_pos = dvec2_add(dvec3_xz(scene->camera.pos),
			dvec2_scale(fdd.tl.a, fdd.row_dist));
	prev = dvec2_sub(fdd.floor_pos, fdd.floor_step);
	next = dvec2_add(fdd.floor_pos, fdd.floor_step);
	fdd.px.x = (int)floor(scene->rc_target->size.x * fdd.start) - 1;
	limit = (int)floor(scene->rc_target->size.x * fdd.end);
	while (++fdd.px.x < limit)
	{
		draw_floor_pixel(scene, &fdd, prev, next);
		prev = fdd.floor_pos;
		fdd.floor_pos = next;
		next = dvec2_add(next, fdd.floor_step);
	}
}

// draw_floor_pixel(scene, &fdd, prev, next);
static void	draw_ceil_line(t_scene *scene, t__fdd fdd)
{
	int		limit;
	t_dvec2	prev;
	t_dvec2	next;

	fdd.floor_step = dvec2_scale(dvec2_sub(fdd.tl.b, fdd.tl.a),
			fdd.row_dist / ((double)scene->rc_target->size.x
				* (fdd.end - fdd.start)));
	fdd.floor_pos = dvec2_add(dvec3_xz(scene->camera.pos),
			dvec2_scale(fdd.tl.a, fdd.row_dist));
	prev = dvec2_sub(fdd.floor_pos, fdd.floor_step);
	next = dvec2_add(fdd.floor_pos, fdd.floor_step);
	fdd.px.x = (int)floor(scene->rc_target->size.x * fdd.start) - 1;
	fdd.px.y = scene->rc_target->size.y - fdd.px.y;
	limit = (int)floor(scene->rc_target->size.x * fdd.end);
	while (++fdd.px.x < limit)
	{
		draw_ceil_pixel(scene, &fdd, prev, next);
		prev = fdd.floor_pos;
		fdd.floor_pos = next;
		next = dvec2_add(next, fdd.floor_step);
	}
}

void	draw_floors_and_ceilings(t_scene *scene, double start, double end)
{
	t__fdd			fdd;
	int				p;
	t_dvec2			plane;

	fdd.tl = camera_get_lr_worldspace_nn(&scene->camera,
			scene->camera.dir);
	plane = dvec2_sub(fdd.tl.b, fdd.tl.a);
	fdd.tl.a = dvec2_add(fdd.tl.a, dvec2_scale(plane, start));
	fdd.tl.b = dvec2_sub(fdd.tl.b, dvec2_scale(plane, 1.0 - end));
	fdd.px.y = scene->rc_target->size.y / 2;
	fdd.start = start;
	fdd.end = end;
	while (++fdd.px.y <= scene->rc_target->size.y)
	{
		p = fdd.px.y - (scene->rc_target->size.y) / 2;
		fdd.row_dist = ((double)(scene->rc_target->size.y) / 2.0) / (p
				* 2.0 * tan(scene->camera.vfov / 2.0));
		draw_floor_line(scene, fdd);
		draw_ceil_line(scene, fdd);
	}
	p = fdd.px.y - scene->rc_target->size.y / 2;
	fdd.row_dist = ((double)scene->rc_target->size.y / 2.0) / (p
			* 2.0 * tan(scene->camera.vfov / 2.0));
	fdd.px.y = 0;
	draw_ceil_line(scene, fdd);
}
