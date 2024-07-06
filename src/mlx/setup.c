/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ihhrabar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/01 17:06:33 by ihhrabar          #+#    #+#             */
/*   Updated: 2024/05/09 16:28:32 by ihhrabar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#if BACKEND == MLX
# include "cub3d.h"
# include "yprint.h"
# include "ytypes.h"
# include <stdlib.h>
# include "cub3d_generic.h"

t_bool	init_backend(t_game *game)
{
	(void)game;
	yprintf("Initializing MLX backend...\n");
	game->window_ctx = make_window(game->config.window_size);
	game->gfx = gfx_new(game->window_ctx, game->config.window_size,
			game->rthread_count);
	set_cursor_visibile(game->window_ctx, !game->config.mouselook);
	return (TRUE);
}

t_bool	deinit_backend(t_game *game)
{
	yprintf("Deinitializing MLX backend...\n");
	set_cursor_visibile(game->window_ctx, game->config.mouselook);
	gfx_delete(game->gfx, game->window_ctx);
	destroy_window(game->window_ctx);
	free(game->window_ctx.mlx);
	exit(0);
	return (TRUE);
}

#endif
