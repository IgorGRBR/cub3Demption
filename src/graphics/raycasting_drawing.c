/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycasting_drawing.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ihhrabar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 18:47:03 by ihhrabar          #+#    #+#             */
/*   Updated: 2024/06/04 18:47:04 by ihhrabar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "cub3d_core.h"
#include "ymath.h"
#include "ytypes.h"
#include "yvec2.h"
#include "yvec3.h"
#include <math.h>

static void	set_pixel(t_image *img[3], t_ivec2_pair coords, double depth)
{
	t_color	srcc;

	srcc = image_get_pixel(img[1], coords.b.x, coords.b.y);
	if (srcc.a == 0)
		return ;
	image_set_pixel(img[0], coords.a.x, coords.a.y, srcc);
	image_set_pixel(img[2], coords.a.x, coords.a.y, depth_as_color(depth));
}

void	draw_the_line(t_scene *scene, t_image *img, double d[6], int k[2])
{
	t_uint	mid;
	int		bottom_pixel;
	int		y;
	int		sample_coord;
	int		scalar;

	y = int_max(-1, (int)floor(scene->rc_target->size.y * d[0])) + 1;
	bottom_pixel = (int)ceil(scene->rc_target->size.y * d[1]);
	bottom_pixel = int_min(scene->rc_target->size.y, bottom_pixel);
	mid = scene->rc_target->size.y / 2;
	scalar = (img->size.y) * (d[4] / d[3]);
	while (y < bottom_pixel)
	{
		sample_coord = (int)(y - mid);
		set_pixel((t_image *[]){scene->rc_target, img, scene->rc_depth},
			(t_ivec2_pair){ivec2_new(k[1], y), ivec2_new(k[0],
				(int)floor(sample_coord / d[2]) - scalar)}, d[5]);
		y++;
	}
}

void	draw_the_solid_line(t_scene *scene, t_image *img, double d[6], int k[2])
{
	t_uint	mid;
	int		bottom_pixel;
	int		y;
	int		sample_coord;
	int		scalar;

	y = int_max(-1, (int)floor(scene->rc_target->size.y * d[0])) + 1;
	bottom_pixel = (int)ceil(scene->rc_target->size.y * d[1]);
	bottom_pixel = int_min(scene->rc_target->size.y, bottom_pixel);
	mid = scene->rc_target->size.y / 2;
	scalar = (img->size.y) * (d[4] / d[3]);
	while (y < bottom_pixel)
	{
		sample_coord = (int)(y - mid);
		image_set_pixel(scene->rc_target, k[1], y,
			image_get_pixel(img, k[0], (int)floor(sample_coord / d[2])
				- scalar));
		image_set_pixel(scene->rc_depth, k[1], y, depth_as_color(d[5]));
		y++;
	}
}

static void	draw_doors(t_scene *scene, t_raycast_query query, int pos)
{
	int		i;
	double	scale_factor;
	int		tx[2];
	double	col[6];
	t_image	*img;

	i = query.door_count;
	while (--i >= 0)
	{
		if (!query.doors[i].visible)
			continue ;
		img = query.doors[i].node->data.door.closed_texture;
		if (query.doors[i].node->data.door.open)
			img = query.doors[i].node->data.door.open_texture;
		col[5] = dvec2_length(dvec2_sub(dvec3_xz(scene->camera.pos),
					query.doors[i].pos)) * query.cos_angle;
		scale_factor = 1.0 / (col[5] * (tan(scene->camera.vfov / 2.0)));
		compute_texture_scaling(col, dvec2_new(1.0, 0.5), scale_factor,
			(double)(scene->rc_target->size.y) / (double)img->size.y);
		tx[0] = (int)floor((double)img->size.x * query.doors[i].tex_offset);
		tx[1] = pos;
		draw_the_line(scene, img, col, tx);
	}
}

void	draw_query_result(t_scene *scene, t_raycast_query query,
			int pos)
{
	double	scale_factor;
	int		tx[2];
	double	col[6];
	t_image	*img;

	if (!query.hit_node || query.hit_dist == 0.0)
		return (draw_doors(scene, query, pos));
	img = query.hit_node->data.wall.texture[query.hit_dir];
	scale_factor = 1.0 / ((query.hit_dist * query.cos_angle)
			* (tan(scene->camera.vfov / 2.0)));
	compute_texture_scaling(col, dvec2_new(1.0, 0.5), scale_factor,
		(double)(scene->rc_target->size.y) / (double)img->size.y);
	tx[0] = (int)floor((double)img->size.x * query.tex_offset);
	tx[1] = pos;
	col[5] = query.hit_dist * query.cos_angle;
	draw_the_solid_line(scene, img, col, tx);
	draw_doors(scene, query, pos);
}
