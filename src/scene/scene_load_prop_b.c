/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene_spawn_prop_b.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ihhrabar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/08 16:02:44 by ihhrabar          #+#    #+#             */
/*   Updated: 2024/06/08 16:02:45 by ihhrabar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	scene_load_dummy(t_prop *prop, t_game *game)
{
	prop->vent->frame[DIR_NORTH] = assets_get_image(game->assets,
			"dummy_img");
	prop->vent->frame[DIR_EAST] = assets_get_image(game->assets,
			"dummy_img");
	prop->vent->frame[DIR_SOUTH] = assets_get_image(game->assets,
			"dummy_img");
	prop->solid = FALSE;
}
