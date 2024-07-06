/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   image_drawing_e.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ihhrabar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/08 15:59:34 by ihhrabar          #+#    #+#             */
/*   Updated: 2024/06/08 15:59:35 by ihhrabar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "cub3d_generic.h"
#include "ymath.h"
#include "yvec2.h"
#include <math.h>

static t_ivec2	sample_color_pos(t_texture *img, t_dvec2 s_pos,
					t__paint_info pn)
{
	t_ivec2	texture_pos;
	t_ivec2	tex_size;

	tex_size = texture_get_size(img);
	s_pos.x = double_clamp((s_pos.x * pn.quad.b.x) + pn.quad.a.x, 0.0, 1.0);
	s_pos.y = double_clamp((s_pos.y * pn.quad.b.y) + pn.quad.a.y, 0.0, 1.0);
	texture_pos.x = int_clamp((int)floor((double)tex_size.x * s_pos.x), 0,
			tex_size.x - 1);
	texture_pos.y = int_clamp((int)floor((double)tex_size.y * s_pos.y), 0,
			tex_size.y - 1);
	if (pn.pd.x)
		texture_pos.x = tex_size.x - texture_pos.x - 1;
	if (pn.pd.y)
		texture_pos.y = tex_size.y - texture_pos.y - 1;
	return (texture_pos);
}

static void	paint_pixel(t_texture *tex[2], t_ivec2 iv[2],
		t__paint_info pn, t__fx_info fn)
{
	t_dvec2	s_pos;
	t_ivec2	t_pos;
	t_color	color;

	s_pos.x = (double)(iv[0].x - pn.tl.x) / (double)iv[1].x;
	s_pos.y = (double)(iv[0].y - pn.tl.y) / (double)iv[1].y;
	t_pos = sample_color_pos(tex[1], s_pos, pn);
	color = (*fn.fx)(t_pos, iv[0], fn.ctx);
	texture_set_pixel_unsafe(tex[0], iv[0].x, iv[0].y,
		color);
}

static void	paint_texture(t_texture *target, t_texture *texture,
	t__paint_info pn, t__fx_info fn)
{
	t_ivec2	brush;
	t_ivec2	size;
	t_ivec2	lpts[2];

	size = ivec2_sub(pn.br, pn.tl);
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
				(t_ivec2 []){brush, size}, pn, fn);
			brush.x++;
		}
		brush.y++;
	}
}

t_bool	_image_draw_fx_into(t_image *target, t_image *source,
				t_draw_info dinfo)
{
	t_ivec2			size;
	t__paint_info	pn;
	t__fx_info		fn;

	pn = _prepare_paint_info(dinfo, source->_data);
	fn.fx = dinfo.ctx.fx.func;
	if (!fn.fx)
		return (error_exit("no drawing function specified"), FALSE);
	fn.ctx = dinfo.ctx.fx.ctx;
	size = ivec2_sub(dinfo.tgt_clip.b, dinfo.tgt_clip.a);
	if (iaabb_test(dinfo.tgt_clip.a, size, pn.tl, ivec2_sub(pn.br, pn.tl)))
	{
		paint_texture(target->_data, source->_data, pn, fn);
		return (TRUE);
	}
	return (FALSE);
}
