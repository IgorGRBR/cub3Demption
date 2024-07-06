/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   text_drawing.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ihhrabar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/09 16:29:40 by ihhrabar          #+#    #+#             */
/*   Updated: 2024/05/09 16:29:45 by ihhrabar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#if BACKEND == SDL
# include "cub3d.h"
# include "cub3d_generic.h"
# include "sdl_backend.h"
# include "ylib.h"
# include <SDL2/SDL_surface.h>
# include <SDL2/SDL_pixels.h>
# include "SDL2/SDL_ttf.h"
# include <SDL2/SDL_rect.h>

void	draw_debug(t_game *game, t_string *text, t_ivec2 pos, t_color color)
{
	draw_debug_cstr(game, text->cstr, pos, color);
}

// SDL_Color	sdl_bg_color;
// sdl_bg_color = (SDL_Color) {0, 0, 0, 255};
void	draw_debug_cstr(t_game *game, const char *text, t_ivec2 pos,
			t_color color)
{
	SDL_Color	sdl_color;
	SDL_Surface	*text_surface;
	SDL_Rect	tgt;

	sdl_color.r = color.r;
	sdl_color.g = color.g;
	sdl_color.b = color.b;
	sdl_color.a = color.a;
	text_surface = TTF_RenderUTF8_Solid(game->window_ctx.font, text, sdl_color);
	tgt.x = pos.x;
	tgt.y = pos.y;
	tgt.w = 0;
	tgt.h = 0;
	SDL_BlitSurface(text_surface, YNULL, game->gfx->target->_data, &tgt);
	SDL_FreeSurface(text_surface);
}

#endif
