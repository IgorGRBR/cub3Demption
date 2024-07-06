/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture_b.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ihhrabar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 18:46:20 by ihhrabar          #+#    #+#             */
/*   Updated: 2024/06/04 18:46:21 by ihhrabar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#if BACKEND == MLX
# include "cub3d_generic.h"
# include "ymath.h"
# include "yvec2.h"

// t__pinfo	_prepare_paint_info_old(t_draw_info dinfo, t_texture *tx)
// {
// 	t__paint_info	pn;

// 	pn.tl.x = int_min(dinfo.tgt_quad.a.x, dinfo.tgt_quad.b.x);
// 	pn.tl.y = int_min(dinfo.tgt_quad.a.y, dinfo.tgt_quad.b.y);
// 	pn.br.x = int_max(dinfo.tgt_quad.a.x, dinfo.tgt_quad.b.x);
// 	pn.br.y = int_max(dinfo.tgt_quad.a.y, dinfo.tgt_quad.b.y);
// 	pn.pd = ivec2_new((dinfo.tgt_quad.b.x < dinfo.tgt_quad.a.x),
// 			(dinfo.tgt_quad.b.y < dinfo.tgt_quad.a.y));
// 	pn.quad.a.x = (double)dinfo.src_quad.a.x / (double)tx->size.x;
// 	pn.quad.a.y = (double)dinfo.src_quad.a.y / (double)tx->size.y;
// 	pn.quad.b.x = (double)(dinfo.src_quad.b.x - dinfo.src_quad.a.x)
// 		/ (double)tx->size.x;
// 	pn.quad.b.y = (double)(dinfo.src_quad.b.y - dinfo.src_quad.a.y)
// 		/ (double)tx->size.y;
// 	pn.inv_size = dvec2_new(1.0 / (double)(pn.br.x - pn.tl.x),
// 			1.0 / (double)(pn.br.y - pn.tl.y));
// 	return (pn);
// }

static t_dvec2_pair	make_dclip(t_ivec2_pair tgt_clip, t_ivec2 tl, t_ivec2 br)
{
	t_dvec2_pair	dclip;

	dclip.a.x = (double)(tgt_clip.a.x - tl.x)
		/ (double)(br.x - tl.x);
	dclip.a.y = (double)(tgt_clip.a.y - tl.y)
		/ (double)(br.y - tl.y);
	dclip.b.x = (double)(br.x - tgt_clip.b.x)
		/ (double)(br.x - tl.x);
	dclip.b.y = (double)(br.y - tgt_clip.b.y)
		/ (double)(br.y - tl.y);
	return (dclip);
}

static t_dvec2_pair	make_quad(t_dvec2_pair dclip, t_ivec2 txsize,
						t_ivec2_pair src_quad)
{
	t_dvec2_pair	quad;

	quad.a.x = double_max(dclip.a.x,
			(double)src_quad.a.x / (double)txsize.x);
	quad.a.y = double_max(dclip.a.y,
			(double)src_quad.a.y / (double)txsize.y);
	quad.b.x = double_min(1.0 - dclip.b.x,
			(double)(src_quad.b.x)
			/ (double)txsize.x) - quad.a.x;
	quad.b.y = double_min(1.0 - dclip.b.y,
			(double)(src_quad.b.y)
			/ (double)txsize.y) - quad.a.y;
	return (quad);
}

t__pinfo	_prepare_paint_info(t_draw_info dinfo, t_texture *tx)
{
	t__paint_info	pn;
	t_dvec2_pair	dclip;

	pn.tl.x = int_min(dinfo.tgt_quad.a.x, dinfo.tgt_quad.b.x);
	pn.tl.y = int_min(dinfo.tgt_quad.a.y, dinfo.tgt_quad.b.y);
	pn.br.x = int_max(dinfo.tgt_quad.a.x, dinfo.tgt_quad.b.x);
	pn.br.y = int_max(dinfo.tgt_quad.a.y, dinfo.tgt_quad.b.y);
	pn.pd = ivec2_new((dinfo.tgt_quad.b.x < dinfo.tgt_quad.a.x),
			(dinfo.tgt_quad.b.y < dinfo.tgt_quad.a.y));
	dclip = make_dclip(dinfo.tgt_clip, pn.tl, pn.br);
	pn.tl.x = int_max(dinfo.tgt_clip.a.x, pn.tl.x);
	pn.tl.y = int_max(dinfo.tgt_clip.a.y, pn.tl.y);
	pn.br.x = int_min(dinfo.tgt_clip.b.x, pn.br.x);
	pn.br.y = int_min(dinfo.tgt_clip.b.y, pn.br.y);
	pn.inv_size = dvec2_new(1.0 / (double)(pn.br.x - pn.tl.x),
			1.0 / (double)(pn.br.y - pn.tl.y));
	pn.quad = make_quad(dclip, texture_get_size(tx), dinfo.src_quad);
	return (pn);
}

void	texture_set_pixel_unsafe(t_texture *texture, int x, int y,
			t_color color)
{
	char	*px_ptr;

	px_ptr = texture->pixel_data + y * texture->line_length
		+ x * texture->bpp_div_8;
	color.a = 255 - color.a;
	*(t_color *)px_ptr = color;
}

t_color	texture_get_pixel_unsafe(t_texture *texture, int x, int y)
{
	char	*px_ptr;
	t_color	color;

	px_ptr = texture->pixel_data + y * texture->line_length
		+ x * texture->bpp_div_8;
	color = *(t_color *)px_ptr;
	color.a = 255 - color.a;
	return (color);
}

#endif
