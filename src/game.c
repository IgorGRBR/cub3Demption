/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ihhrabar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/01 17:26:11 by ihhrabar          #+#    #+#             */
/*   Updated: 2024/05/09 16:23:15 by ihhrabar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "cub3d_core.h"
#include "cub3d_generic.h"
#include "ymap.h"
#include "ydefines.h"
#include "ymap.h"
#include "yvec2.h"
#include <stdlib.h>

t_game	game_new(t_game_config gcfg)
{
	t_game	game;

	game.config = gcfg;
	game.keymap = YNULL;
	game.gfx = YNULL;
	game.elapsed_time = 0.0;
	game.running = TRUE;
	game.assets = assets_new();
	configure_keymap(&game);
	game.rthread_count = gcfg.rendering_thread_count;
	game.scene = YNULL;
	game.mouse_pos = ivec2_zero();
	return (game);
}

// deinit_game_data(&game->data, game);
int	game_deinit(t_game *game)
{
	t_map_iter	it;

	if (game->keymap)
	{
		it = map_iter(game->keymap);
		while (map_iter_next(&it))
			(free(it.key), free(it.value));
		(map_delete(game->keymap), game->keymap = YNULL);
	}
	if (game->scene)
		(scene_delete(game->scene, game), game->scene = YNULL);
	assets_delete(game->assets, game->window_ctx);
	game->running = FALSE;
	deinit_backend(game);
	return (0);
}
