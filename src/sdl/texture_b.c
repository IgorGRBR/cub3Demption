/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture_b.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ihhrabar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/09 16:30:00 by ihhrabar          #+#    #+#             */
/*   Updated: 2024/05/09 16:30:05 by ihhrabar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#if BACKEND == SDL
# include "yvec2.h"
# include "cub3d.h"
# include "cub3d_generic.h"
# include "ylib.h"
# include "sdl_backend.h"
# include "SDL2/SDL.h"

t__pinfo	_prepare_paint_info(t_draw_info dinfo, t_texture *tx)
{
	t__paint_info	pn;

	pn.tl.x = int_min(dinfo.tgt_quad.a.x, dinfo.tgt_quad.b.x);
	pn.tl.y = int_min(dinfo.tgt_quad.a.y, dinfo.tgt_quad.b.y);
	pn.br.x = int_max(dinfo.tgt_quad.a.x, dinfo.tgt_quad.b.x);
	pn.br.y = int_max(dinfo.tgt_quad.a.y, dinfo.tgt_quad.b.y);
	pn.pd = ivec2_new((dinfo.tgt_quad.b.x < dinfo.tgt_quad.a.x),
			(dinfo.tgt_quad.b.y < dinfo.tgt_quad.a.y));
	pn.quad.a.x = (double)dinfo.src_quad.a.x / (double)tx->w;
	pn.quad.a.y = (double)dinfo.src_quad.a.y / (double)tx->h;
	pn.quad.b.x = (double)(dinfo.src_quad.b.x - dinfo.src_quad.a.x)
		/ (double)tx->w;
	pn.quad.b.y = (double)(dinfo.src_quad.b.y - dinfo.src_quad.a.y)
		/ (double)tx->h;
	pn.inv_size = dvec2_new(1.0 / (double)(pn.br.x - pn.tl.x),
			1.0 / (double)(pn.br.y - pn.tl.y));
	return (pn);
}

void	texture_set_pixel_unsafe(t_texture *texture, int x, int y,
			t_color color)
{
	t_uint	*target_pixel;

	target_pixel = (Uint32 *)((Uint8 *)texture->pixels
			+ y * texture->pitch
			+ x * texture->format->BytesPerPixel);
	color.a = 255 - color.a;
	*(t_color *)target_pixel = color;
}

t_color	texture_get_pixel_unsafe(t_texture *texture, int x, int y)
{
	t_color	color;
	t_uint	*target_pixel;

	target_pixel = (Uint32 *)((Uint8 *)texture->pixels
			+ y * texture->pitch
			+ x * texture->format->BytesPerPixel);
	color = *(t_color *)target_pixel;
	color.a = 255 - color.a;
	return (color);
}

#endif
