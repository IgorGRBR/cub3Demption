/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   image_drawing_d.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ihhrabar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/08 15:59:30 by ihhrabar          #+#    #+#             */
/*   Updated: 2024/06/08 15:59:31 by ihhrabar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "cub3d_generic.h"
#include "ymath.h"
#include "yvec2.h"

// This is the worst thing I've ever written in my existence.
static void	paint_pixel(t_texture *tex[2], t_ivec2 iv,
		t__paint_info pn, t__depth_info dn)
{
	t_color	color;
	t_dvec2	s_pos;
	t_color	color2;

	color = texture_get_pixel_unsafe(dn.depth, iv.x, iv.y);
	if (color_as_depth(color) < dn.threshold)
		return ;
	color = texture_get_pixel_unsafe(tex[0], iv.x, iv.y);
	s_pos.x = pn.inv_size.x * (double)(iv.x - pn.tl.x);
	s_pos.y = pn.inv_size.y * (double)(iv.y - pn.tl.y);
	color2 = sample_color(tex[1], s_pos, pn.quad, pn.pd);
	if (color2.a == 0)
		return ;
	texture_set_pixel_unsafe(dn.depth, iv.x, iv.y,
		depth_as_color(dn.threshold));
	texture_set_pixel_unsafe(tex[0], iv.x, iv.y,
		color2);
}

static void	paint_texture(t_texture *target, t_texture *texture,
	t__paint_info pn, t__depth_info dn)
{
	t_ivec2	brush;
	t_ivec2	lpts[2];

	lpts[0] = ivec2_new(int_max(pn.tl.x, 0), int_max(pn.tl.y, 0));
	brush = lpts[0];
	lpts[1].x = int_min(pn.br.x, texture_get_width(target));
	lpts[1].y = int_min(pn.br.y, texture_get_height(target));
	while (brush.y < lpts[1].y)
	{
		brush.x = lpts[0].x;
		while (brush.x < lpts[1].x)
		{
			paint_pixel((t_texture *[]){target, texture},
				brush, pn, dn);
			brush.x++;
		}
		brush.y++;
	}
}

t_bool	_image_draw_depth_into(t_image *target, t_image *source,
			t_draw_info dinfo)
{
	t_ivec2			size;
	t__paint_info	pn;
	t__depth_info	dn;

	pn = _prepare_paint_info(dinfo, source->_data);
	dn.depth = dinfo.ctx.depth.buffer->_data;
	dn.threshold = dinfo.ctx.depth.src_depth;
	size = ivec2_sub(dinfo.tgt_clip.b, dinfo.tgt_clip.a);
	if (iaabb_test(dinfo.tgt_clip.a, size, pn.tl, ivec2_sub(pn.br, pn.tl)))
	{
		paint_texture(target->_data, source->_data, pn, dn);
		return (TRUE);
	}
	return (FALSE);
}
