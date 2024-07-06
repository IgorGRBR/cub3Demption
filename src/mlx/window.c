/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   window.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ihhrabar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/01 17:30:05 by ihhrabar          #+#    #+#             */
/*   Updated: 2023/12/01 17:30:06 by ihhrabar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#if BACKEND == MLX
# include "mlx_backend.h"
# include "cub3d.h"
# include "ytypes.h"
# include "mlx.h"
# include <unistd.h>
# include "cub3d_generic.h"

t_wctx	make_window(t_ivec2 size)
{
	t_wctx	wctx;

	if (size.x < MIN_WIN_W && size.y < MIN_WIN_H)
		error_exit("Invalid window size");
	wctx.mlx = mlx_init();
	wctx.mlx_win = mlx_new_window(wctx.mlx, size.x, size.y,
			WINDOW_TITLE);
	wctx.disp_texture = texture_new(wctx, size);
	return (wctx);
}

void	destroy_window(t_wctx wctx)
{
	if (wctx.mlx || wctx.mlx_win)
	{
		texture_delete(wctx.disp_texture, wctx);
		mlx_destroy_window(wctx.mlx, wctx.mlx_win);
	}
}

void	setup_hooks(t_wctx wctx, t_game *game)
{
	mlx_loop_hook(wctx.mlx, game_update, game);
	mlx_hook(wctx.mlx_win, DESTROY_CODE, 0, game_deinit, game);
	mlx_hook(wctx.mlx_win, KEYDOWN_CODE, KEYDOWN_MASK,
		mlx_key_press_event, game);
	mlx_hook(wctx.mlx_win, KEYUP_CODE, KEYUP_MASK,
		mlx_key_release_event, game);
	mlx_mouse_hook(wctx.mlx_win, mlx_mouse_event, game);
}

void	update_loop(t_game *game)
{
	mlx_loop(game->window_ctx.mlx);
}

#endif
