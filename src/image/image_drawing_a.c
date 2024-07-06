/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   image_drawing_a.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ihhrabar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/09 16:26:36 by ihhrabar          #+#    #+#             */
/*   Updated: 2024/05/09 16:26:40 by ihhrabar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "cub3d_core.h"
#include "ytypes.h"
#include "yvec2.h"
#include <math.h>

// dinfo.src_quad.a.y = dinfo.src_quad.b.y / 2;
t_bool	image_draw_into_tf(t_image *target, t_image *img,
				t_transform2d tf2d)
{
	t_ivec2			pos;
	t_ivec2_pair	corners;
	t_draw_info		dinfo;

	dinfo = draw_info_new(DRAW_NORMAL);
	pos.x = trunc(tf2d.pos.x);
	pos.y = trunc(tf2d.pos.y);
	corners.a.x = pos.x + trunc(-img->origin.x * tf2d.sca.x);
	corners.a.y = pos.y + trunc(-img->origin.y * tf2d.sca.y);
	corners.b.x = pos.x + trunc((img->size.x - img->origin.x) * tf2d.sca.x);
	corners.b.y = pos.y + trunc((img->size.y - img->origin.y) * tf2d.sca.y);
	dinfo.tgt_quad = corners;
	dinfo.src_quad = full_quad(img->size);
	dinfo.tgt_clip = full_quad(target->size);
	return (image_draw_into(target, img, dinfo));
}

t_bool	image_draw_into_pos(t_image *target, t_image *img, t_ivec2 pos)
{
	t_ivec2_pair	corners;
	t_draw_info		dinfo;

	dinfo = draw_info_new(DRAW_NORMAL);
	corners.a.x = pos.x - img->origin.x;
	corners.a.y = pos.y - img->origin.y;
	corners.b.x = pos.x + img->size.x - img->origin.x;
	corners.b.y = pos.y + img->size.y - img->origin.y;
	dinfo.tgt_quad = corners;
	dinfo.src_quad = full_quad(img->size);
	dinfo.tgt_clip = full_quad(target->size);
	return (image_draw_into(target, img, dinfo));
}

void	image_clear(t_image *image, t_color color)
{
	t_ivec2	i;

	i = ivec2_zero();
	while (i.y < image->size.y)
	{
		i.x = 0;
		while (i.x < image->size.x)
		{
			image_set_pixel(image, i.x, i.y, color);
			i.x++;
		}
		i.y++;
	}
}

t_bool	image_draw_into(t_image *target, t_image *img,
			t_draw_info dinfo)
{
	if (dinfo.type == DRAW_NORMAL)
		return (_image_draw_normal_into(target, img, dinfo));
	else if (dinfo.type == DRAW_WITH_DEPTH)
		return (_image_draw_depth_into(target, img, dinfo));
	else if (dinfo.type == DRAW_WITH_EFFECT)
	{
		dinfo.ctx.fx.ctx.src = img;
		dinfo.ctx.fx.ctx.tgt = target;
		return (_image_draw_fx_into(target, img, dinfo));
	}
	else
		return (error_exit("Unknown/invalid draw type"), FALSE);
}
