/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   window.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ihhrabar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/09 16:30:10 by ihhrabar          #+#    #+#             */
/*   Updated: 2024/05/09 16:30:14 by ihhrabar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#if BACKEND == SDL
# include "sdl_backend.h"
# include "cub3d.h"
# include "ylib.h"
# include <SDL2/SDL.h>

t_wctx	make_window(t_ivec2 size)
{
	t_wctx	wctx;

	wctx.win = SDL_CreateWindow(WINDOW_TITLE,
			SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
			size.x, size.y, 0);
	wctx.sur = SDL_GetWindowSurface(wctx.win);
	if (!wctx.win || !wctx.sur)
		error_exit("Failed to create SDL2 window");
	wctx.game_update = YNULL;
	wctx.game_deinit = YNULL;
	return (wctx);
}

void	destroy_window(t_wctx wctx)
{
	if (wctx.win)
		SDL_DestroyWindow(wctx.win);
}

void	setup_hooks(t_wctx wctx, t_game *game)
{
	(void)game;
	(void)wctx;
}

#endif
