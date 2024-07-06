/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threaded_rendering_c.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ihhrabar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/08 16:01:16 by ihhrabar          #+#    #+#             */
/*   Updated: 2024/06/08 16:01:17 by ihhrabar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include <math.h>
#include "ymath.h"

void	mt_draw_image_into(t_drawing_thread *thread)
{
	int			ssize;
	int			tsize;
	t_draw_info	dinfo;

	dinfo = thread->data.drawing.info;
	ssize = dinfo.src_quad.b.x - dinfo.src_quad.a.x;
	tsize = dinfo.tgt_quad.b.x - dinfo.tgt_quad.a.x;
	dinfo.src_quad.a.x = ceil((double)ssize * thread->min_range);
	dinfo.src_quad.b.x = floor((double)ssize * thread->max_range);
	dinfo.tgt_quad.a.x += ceil((double)tsize * thread->min_range);
	dinfo.tgt_quad.b.x -= floor((double)tsize * (1.0 - thread->max_range));
	image_draw_into(thread->data.drawing.target,
		thread->data.drawing.source, dinfo);
}

static t_color	depth_fog(t_ivec2 src_px, t_ivec2 tgt_px, t_fx_context ctx)
{
	t_color	col;
	float	depth;

	(void)tgt_px;
	col = image_get_pixel(ctx.src, src_px.x, src_px.y);
	depth = color_as_depth(image_get_pixel(ctx.depth, src_px.x, src_px.y));
	col.r = (t_uchar)floor((double)col.r
			* (1.0 - double_min(1.0, depth / 10.0)));
	col.g = (t_uchar)floor((double)col.g
			* (1.0 - double_min(1.0, depth / 10.0)));
	col.b = (t_uchar)floor((double)col.b
			* (1.0 - double_min(1.0, depth / 10.0)));
	col.a = (t_uchar)floor((double)col.a
			* (1.0 - double_min(1.0, depth / 10.0)));
	return col;
}

static void	draw_fx(t_drawing_thread *thread)
{
	t_scene		*scene;
	t_draw_info	dinfo;

	scene = thread->data.scene;
	dinfo = draw_info_new(DRAW_WITH_EFFECT);
	dinfo.src_quad = full_quad(scene->rc_target->size);
	dinfo.tgt_quad = full_quad(scene->final_target->size);
	dinfo.tgt_clip = full_quad(scene->final_target->size);
	dinfo.src_quad.a.x = (int)floor(thread->min_range
			* (double)scene->rc_target->size.x);
	dinfo.tgt_quad.a.x = dinfo.src_quad.a.x;
	dinfo.tgt_clip.a.x = dinfo.src_quad.a.x;
	dinfo.src_quad.b.x = (int)floor(thread->max_range
			* (double)scene->rc_target->size.x);
	dinfo.tgt_quad.b.x = dinfo.src_quad.b.x;
	dinfo.tgt_clip.b.x = dinfo.src_quad.b.x;
	dinfo.ctx.fx.func = depth_fog;
	dinfo.ctx.fx.ctx.depth = scene->rc_depth;
	dinfo.ctx.fx.ctx.src = scene->rc_target;
	dinfo.ctx.fx.ctx.tgt = scene->final_target;
	image_draw_into(scene->final_target, scene->rc_target, dinfo);
}

void	mt_draw_scene(t_drawing_thread *thread)
{
	t_scene		*scene;

	scene = thread->data.scene;
	draw_ceil_floor_colors(scene, thread->min_range, thread->max_range);
	raycast_region(scene, thread->min_range, thread->max_range,
		scene->timing.draw_interp);
	draw_floors_and_ceilings(scene, thread->min_range, thread->max_range);
	draw_fx(thread);
}
