/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update_loop.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ihhrabar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/09 16:30:17 by ihhrabar          #+#    #+#             */
/*   Updated: 2024/05/09 16:30:22 by ihhrabar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#if BACKEND == SDL
# include "ytypes.h"
# include "cub3d.h"
# include "cub3d_core.h"
# include "ymap.h"
# include "yprint.h"
# include "SDL2/SDL_events.h"
# include <SDL2/SDL_events.h>
# include <SDL2/SDL_scancode.h>

static void	keyboard_event(t_game *game, SDL_Event e)
{
	SDL_Scancode	code;
	t_game_key		*gk_ptr;
	t_game_key		gk;

	code = e.key.keysym.scancode;
	gk_ptr = map_get(game->keymap, &code);
	gk = GK_NONE;
	if (gk_ptr)
		gk = *gk_ptr;
	if (e.type == SDL_KEYDOWN && !e.key.repeat)
	{
		yprintf("Key pressed %d\n", code);
		game_key_pressed(game, gk);
	}
	else if (e.type == SDL_KEYUP)
	{
		yprintf("Key released %d\n", code);
		game_key_released(game, gk);
	}
}

static t_bool	handle_event(t_game *game, SDL_Event e)
{
	if (e.type == SDL_QUIT)
		return (game_deinit(game));
	else if (e.type == SDL_KEYDOWN || e.type == SDL_KEYUP)
		return (keyboard_event(game, e), TRUE);
	return (TRUE);
}

void	update_loop(t_game *game)
{
	SDL_Event	e;

	while (game->running)
	{
		game->running = game_update(game);
		while (SDL_PollEvent(&e) > 0)
			handle_event(game, e);
	}
}

#endif
