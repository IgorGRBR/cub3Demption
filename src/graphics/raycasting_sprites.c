/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycasting_sprites.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ihhrabar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/08 16:01:11 by ihhrabar          #+#    #+#             */
/*   Updated: 2024/06/08 16:01:12 by ihhrabar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "cub3d_core.h"
#include "ytypes.h"
#include "yvec2.h"
#include "yvec3.h"
#include <math.h>
#include <stdlib.h>

static t_draw_info	setup_draw_info(t_image *target, double min, double max)
{
	t_draw_info	dinfo;
	t_ivec2		limits;

	dinfo = draw_info_new(DRAW_WITH_DEPTH);
	limits.x = target->size.x * min;
	limits.y = target->size.x * max;
	dinfo.tgt_clip = full_quad(target->size);
	dinfo.tgt_clip.a.x = limits.x;
	dinfo.tgt_clip.b.x = limits.y;
	return (dinfo);
}

static const double	g_h = 64.0;

// if (!img)
// 	return (error_exit("bad stuff\n"));
static void	vis_ent_drawest(t_vis_ent *vent, t_scene *scene, t_dvec2_pair stf,
				double dv[3])
{
	t_ivec2		spr_p;
	t_image		*img;
	double		sprite_scale;
	t_draw_info	dinfo;
	int			xscale;

	spr_p.x = (int)(((double)scene->rc_target->size.x / 2.0)
			* (double)(1.0 + stf.a.x / stf.a.y));
	spr_p.y = abs((int)(scene->rc_target->size.y / stf.a.y));
	img = frame_get_image(&vent->frame, &scene->camera, vent->dir, &xscale);
	sprite_scale = (double)spr_p.y
		/ (g_h * tan(scene->camera.vfov / 2.0) * 2.0);
	dinfo = setup_draw_info(scene->rc_target, dv[1], dv[2]);
	dinfo.ctx.depth.src_depth = dv[0] * dvec2_dot(stf.b, scene->camera.dir);
	dinfo.ctx.depth.buffer = scene->rc_depth;
	dinfo.src_quad = full_quad(img->size);
	dinfo.tgt_quad.a = ivec2_sub(ivec2_new(spr_p.x, scene->rc_target
				->size.y / 2), ivec2_dscale(img->origin, sprite_scale));
	dinfo.tgt_quad.b = ivec2_sub(ivec2_add(ivec2_new(spr_p.x, scene->rc_target
					->size.y / 2), ivec2_dscale(img->size, sprite_scale)),
			ivec2_dscale(img->origin, sprite_scale));
	(void)((xscale == -1) && (xscale = dinfo.tgt_quad.b.x,
		dinfo.tgt_quad.b.x = dinfo.tgt_quad.a.x, dinfo.tgt_quad.a.x = xscale));
	image_draw_into(scene->rc_target, img, dinfo);
}

static void	vis_ent_drawer(t_vis_ent *vent, t_scene *scene,
				t_dvec2_pair cam, t_dvec2_pair ent_vec)
{
	double	vent_dist;
	double	inv_det;
	t_dvec2	tf;

	vent_dist = dvec2_length(dvec2_normalized(ent_vec.a));
	inv_det = 1.0 / (cam.b.x * cam.a.y - cam.b.y * cam.a.x);
	tf = dvec2_new(inv_det * (cam.a.y * ent_vec.a.x - cam.a.x * ent_vec.a.y),
			inv_det * (cam.b.y * ent_vec.a.x - cam.b.x * ent_vec.a.y));
	vis_ent_drawest(vent, scene, (t_dvec2_pair){tf, ent_vec.a},
		(double []){vent_dist, ent_vec.b.x, ent_vec.b.y});
}

void	vis_ent_draw(t_vis_ent *vent, t_scene *scene, t_dvec2 minmax,
			double di)
{
	t_dvec3		vpos;
	t_dvec2		pdir;
	t_dvec2		ent_vec;
	t_dvec2		plane;

	(void)minmax;
	if (!vent->visible)
		return ;
	vpos = vis_ent_get_pos(vent, di);
	pdir = scene->camera.dir;
	ent_vec = dvec2_sub(dvec3_xz(vpos), dvec3_xz(scene->camera.pos));
	if (dvec2_dot(ent_vec, pdir) < 0)
		return ;
	plane = dvec2_scale(dvec2_new(-pdir.y, pdir.x),
			-scene->camera.plane_hwidth);
	vis_ent_drawer(vent, scene, (t_dvec2_pair){pdir, plane},
		(t_dvec2_pair){ent_vec, minmax});
}
