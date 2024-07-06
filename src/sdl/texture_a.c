/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture_a.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ihhrabar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/09 16:29:51 by ihhrabar          #+#    #+#             */
/*   Updated: 2024/05/09 16:29:55 by ihhrabar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#if BACKEND == SDL
# include "cub3d_generic.h"
# include "ytypes.h"
# include "ydefines.h"
// # include "sdl_backend.h"
# include "SDL2/SDL_surface.h"

t_texture	*texture_new(t_wctx wctx, t_ivec2 size)
{
	t_texture	*texture;

	(void)wctx;
	texture = SDL_CreateRGBSurface(
			0, size.x, size.y, 32,
			0, 0, 0, 0);
	return (texture);
}

void	texture_delete(t_texture *texture, t_wctx wctx)
{
	(void)wctx;
	SDL_FreeSurface(texture);
}

void	texture_set_pixel(t_texture *texture, int x, int y, t_color color)
{
	t_uint	*target_pixel;

	x = x % texture->w;
	y = y % texture->h;
	x += (x < 0) * texture->w;
	y += (y < 0) * texture->h;
	target_pixel = (Uint32 *)((Uint8 *)texture->pixels
			+ y * texture->pitch
			+ x * texture->format->BytesPerPixel);
	color.a = 255 - color.a;
	*(t_color *)target_pixel = color;
}

t_color	texture_get_pixel(t_texture *texture, int x, int y)
{
	t_color	color;
	t_uint	*target_pixel;

	x = x % texture->w;
	y = y % texture->h;
	x += (x < 0) * texture->w;
	y += (y < 0) * texture->h;
	target_pixel = (Uint32 *)((Uint8 *)texture->pixels
			+ y * texture->pitch
			+ x * texture->format->BytesPerPixel);
	color = *(t_color *)target_pixel;
	color.a = 255 - color.a;
	return (color);
}

void	texture_blit_to_screen(t_texture *texture, t_wctx wctx)
{
	SDL_BlitScaled(texture, YNULL, wctx.sur, YNULL);
	SDL_UpdateWindowSurface(wctx.win);
}

#endif
