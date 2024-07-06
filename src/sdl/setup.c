/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ihhrabar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/01 17:06:38 by ihhrabar          #+#    #+#             */
/*   Updated: 2024/05/09 16:30:37 by ihhrabar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#if BACKEND == SDL
# include "cub3d.h"
# include "ylib.h"
# include <SDL2/SDL.h>
# include <SDL2/SDL_ttf.h>
# include "cub3d_generic.h"

static const char	*g_font_path = "fonts/default.ttf";

t_bool	init_backend(t_game *game)
{
	yprintf("Initializing SDL backend...\n");
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_EVENTS) < 0)
		return (FALSE);
	if (TTF_Init() < 0)
		return (FALSE);
	game->window_ctx = make_window(game->config.window_size);
	game->window_ctx.font = TTF_OpenFont(g_font_path, 12);
	game->gfx = gfx_new(game->window_ctx, game->config.window_size,
			game->rthread_count);
	return (TRUE);
}

t_bool	deinit_backend(t_game *game)
{
	yprintf("Deinitializing SDL backend...\n");
	gfx_delete(game->gfx, game->window_ctx);
	TTF_CloseFont(game->window_ctx.font);
	destroy_window(game->window_ctx);
	TTF_Quit();
	SDL_Quit();
	return (TRUE);
}

#endif
