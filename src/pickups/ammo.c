/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ammo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ihhrabar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/27 15:38:01 by ihhrabar          #+#    #+#             */
/*   Updated: 2024/06/27 15:38:02 by ihhrabar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "ymath.h"
#include "yprint.h"

static t_bool	collect(t_player *plr, t_uint tag)
{
	(void)tag;
	if (plr->ammo >= PLAYER_MAX_AMMO)
		return (FALSE);
	plr->ammo = int_min(plr->ammo + 4, PLAYER_MAX_AMMO);
	yprintf("player ammo now is %d\n", plr->ammo);
	return (TRUE);
}

t_bool	setup_ammopack(t_item *item, t_game *game)
{
	t_uint	d;

	d = 0;
	while (d < DIR_COUNT)
	{
		item->vent->frame[d] = assets_get_image(game->assets, "ammopack");
		d++;
	}
	item->pfunc = collect;
	return (TRUE);
}
