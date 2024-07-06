/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game_mouse_callbacks.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ihhrabar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/21 14:23:57 by ihhrabar          #+#    #+#             */
/*   Updated: 2024/06/21 14:23:58 by ihhrabar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "yprint.h"

// yprintf("mouse movement: %d %d\n", delta.x, delta.y);
void	game_mouse_move(t_game *game, t_ivec2 delta)
{
	(void)game;
	(void)delta;
}

// if (game->scene)
// 	player_mouse_pressed(game->scene, (t_uint)key);
// yprintf("mouse button: %d\n", key);
void	game_mouse_pressed(t_game *game, int key)
{
	if (game->scene)
	{
		player_mouse_pressed(&game->scene->player, key);
	}
}
