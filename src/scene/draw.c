/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ihhrabar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 18:45:10 by ihhrabar          #+#    #+#             */
/*   Updated: 2024/06/04 18:45:11 by ihhrabar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

#ifdef NO_MT_RENDERING

// minimap_draw(scene->minimap, scene->rc_target);
void	scene_draw(t_scene *scene, t_gfx_ctx *gfx)
{
	t_transform2d	tf;
	double			scale_factor;

	raycast_scene(scene, gfx);
	hud_draw(scene);
	minimap_draw(scene->minimap, scene->final_target);
	tf = g_default2d;
	scale_factor = (double)gfx->target->size.x
		/ (double)scene->final_target->size.x;
	tf.sca.x = scale_factor;
	tf.sca.y = scale_factor;
	image_draw_into_tf(gfx->target, scene->final_target, tf);
}

#else

static t_draw_info	setup_draw_info(t_scene *scene, t_gfx_ctx *gfx)
{
	t_draw_info	dinfo;

	dinfo = draw_info_new(DRAW_NORMAL);
	dinfo.src_quad = full_quad(scene->final_target->size);
	dinfo.tgt_quad = full_quad(gfx->target->size);
	dinfo.tgt_clip = full_quad(gfx->target->size);
	return (dinfo);
}

void	scene_draw(t_scene *scene, t_gfx_ctx *gfx)
{
	t_dctx	dctx;

	raycast_scene(scene, gfx);
	hud_draw(scene);
	minimap_draw(scene->minimap, scene->final_target);
	dctx.target = gfx->target;
	dctx.source = scene->final_target;
	dctx.info = setup_draw_info(scene, gfx);
	draw_multithreaded(gfx, DTHREAD_IMAGE, (t_dthread_data)dctx);
}

#endif
