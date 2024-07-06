/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   image_drawing_b.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ihhrabar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/09 16:26:46 by ihhrabar          #+#    #+#             */
/*   Updated: 2024/05/09 16:26:53 by ihhrabar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "cub3d_core.h"
#include "ytypes.h"
#include <math.h>

t_bool	image_draw(t_gfx_ctx *gfx, t_image *img, t_transform2d tf2d)
{
	return (image_draw_into_tf(gfx->target, img, tf2d));
}

t_bool	image_draw_pos(t_gfx_ctx *gfx, t_image *img, t_ivec2 pos)
{
	return (image_draw_into_pos(gfx->target, img, pos));
}

#ifdef NO_MT_RENDERING

t_bool	image_draw_into_mt(t_gfx_ctx *gfx, t_image *target, t_image *src,
			t_transform2d tf2d)
{
	(void)gfx;
	return (image_draw_into_tf(target, src, tf2d));
}

#else

t_bool	image_draw_into_mt(t_gfx_ctx *gfx, t_image *target, t_image *src,
			t_transform2d tf2d)
{
	t_dctx			dctx;
	t_ivec2			pos;
	t_ivec2_pair	corners;
	t_draw_info		dinfo;

	dinfo = draw_info_new(DRAW_NORMAL);
	pos.x = trunc(tf2d.pos.x);
	pos.y = trunc(tf2d.pos.y);
	corners.a.x = pos.x + trunc(-src->origin.x * tf2d.sca.x);
	corners.a.y = pos.y + trunc(-src->origin.y * tf2d.sca.y);
	corners.b.x = pos.x + trunc((src->size.x - src->origin.x) * tf2d.sca.x);
	corners.b.y = pos.y + trunc((src->size.y - src->origin.y) * tf2d.sca.y);
	dinfo.tgt_quad = corners;
	dinfo.src_quad.b = src->size;
	dinfo.tgt_clip = full_quad(target->size);
	dctx.source = src;
	dctx.target = target;
	dctx.info = dinfo;
	draw_multithreaded(gfx, DTHREAD_IMAGE, (t_dthread_data)dctx);
	return (TRUE);
}

#endif
