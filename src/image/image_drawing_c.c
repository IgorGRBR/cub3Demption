/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   image_drawing_c.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ihhrabar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/08 15:59:26 by ihhrabar          #+#    #+#             */
/*   Updated: 2024/06/08 15:59:27 by ihhrabar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "cub3d_generic.h"
#include "ymath.h"
#include "yvec2.h"

// primitive blending
static t_color	blend_colors(t_color a, t_color b)
{
	if (b.a == 0)
		return (a);
	return (b);
}

static void	paint_texture(t_texture *target, t_texture *texture,
	t__paint_info pn)
{
	t_ivec2	brush;
	t_dvec2	s_pos;
	t_color	color;
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
			color = texture_get_pixel(target, brush.x, brush.y);
			s_pos.x = pn.inv_size.x * (double)(brush.x - pn.tl.x);
			s_pos.y = pn.inv_size.y * (double)(brush.y - pn.tl.y);
			texture_set_pixel_unsafe(target, brush.x, brush.y, blend_colors(
					color, sample_color(texture, s_pos, pn.quad, pn.pd)));
			brush.x++;
		}
		brush.y++;
	}
}

t_bool	_image_draw_normal_into(t_image *target, t_image *source,
			t_draw_info dinfo)
{
	t_ivec2			size;
	t__paint_info	pn;

	pn = _prepare_paint_info(dinfo, source->_data);
	size = ivec2_sub(dinfo.tgt_clip.b, dinfo.tgt_clip.a);
	if (iaabb_test(dinfo.tgt_clip.a, size, pn.tl, ivec2_sub(pn.br, pn.tl)))
	{
		paint_texture(target->_data, source->_data, pn);
		return (TRUE);
	}
	return (FALSE);
}
