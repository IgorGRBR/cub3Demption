/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   events.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ihhrabar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/09 16:28:07 by ihhrabar          #+#    #+#             */
/*   Updated: 2024/05/09 16:28:15 by ihhrabar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#if BACKEND == MLX
# include "cub3d_core.h"
# include "yvec2.h"
# include "cub3d.h"
# include "ymap.h"
// # include "yprint.h"

static t_game_key	get_game_key(int mlx_key, t_map *keymap)
{
	t_game_key	*key;

	key = map_get(keymap, &mlx_key);
	if (!key)
		return (GK_NONE);
	return (*key);
}

// yprintf("mlx pressed %d\n", mlx_key);
int	mlx_key_press_event(int mlx_key, t_game *game)
{
	t_game_key	game_key;

	game_key = get_game_key(mlx_key, game->keymap);
	game_key_pressed(game, game_key);
	return (1);
}

int	mlx_key_release_event(int mlx_key, t_game *game)
{
	t_game_key	game_key;

	game_key = get_game_key(mlx_key, game->keymap);
	game_key_released(game, game_key);
	return (1);
}

int	mlx_mouse_event(int button, int x, int y, t_game *game)
{
	(void)x;
	(void)y;
	if (button > 0)
		game_mouse_pressed(game, button);
	return (1);
}

#endif
