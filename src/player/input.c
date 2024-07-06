/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ihhrabar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/08 16:01:39 by ihhrabar          #+#    #+#             */
/*   Updated: 2024/06/08 16:01:40 by ihhrabar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "cub3d_core.h"
#include "yprint.h"
#include "yvec2.h"
#include <stdio.h>

void	player_key_pressed(t_player *plr, t_game_key key)
{
	plr->keys[key] = TRUE;
}

void	player_key_released(t_player *plr, t_game_key key)
{
	plr->keys[key] = FALSE;
}

void	player_update_inputs(t_player *plr)
{
	t_dvec2	dir;

	dir = dvec2_zero();
	dir.x = (int)plr->keys[GK_STRAFE_RIGHT] - (int)plr->keys[GK_STRAFE_LEFT];
	dir.y = (int)plr->keys[GK_BACKWARD] - (int)plr->keys[GK_FORWARD];
	plr->inputs.move = dvec2_normalized(dir);
	plr->inputs.attack = plr->keys[GK_SHOOT];
	plr->inputs.use = plr->keys[GK_USE];
}

void	player_mouse_pressed(t_player *plr, int btn)
{
	if (btn == 1)
		plr->keys[GK_SHOOT] = TRUE;
}
