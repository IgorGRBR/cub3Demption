/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gac_bandit.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ihhrabar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/24 13:25:25 by ihhrabar          #+#    #+#             */
/*   Updated: 2024/06/24 13:25:27 by ihhrabar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "cub3d_core.h"

void	scene_init_bandit_assets(t_scene *scene, t_game *game)
{
	t_gac	*a;

	a = &scene->gac;
	a->sprites.bandit.idle = assets_get_sprite(game->assets, "bandit_idle");
	a->sprites.bandit.walk = assets_get_sprite(game->assets, "bandit_walk");
	a->sprites.bandit.aim = assets_get_sprite(game->assets, "bandit_aim");
	a->sprites.bandit.hurt = assets_get_sprite(game->assets, "bandit_hit");
	a->sprites.bandit.walkg = assets_get_sprite(game->assets,
			"bandit_walk_gun");
	a->sprites.bandit.fire = assets_get_sprite(game->assets, "bandit_fire");
}
