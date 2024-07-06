/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   health.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ihhrabar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/27 15:38:08 by ihhrabar          #+#    #+#             */
/*   Updated: 2024/06/27 15:38:09 by ihhrabar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "ymath.h"
#include "yprint.h"

static t_bool	collect_healthpack(t_player *plr, t_uint tag)
{
	(void)tag;
	if (plr->actor->health >= PLAYER_MAX_HEALTH)
		return (FALSE);
	plr->actor->health = int_min(plr->actor->health + 3, PLAYER_MAX_HEALTH);
	yprintf("player health now is %d\n", plr->actor->health);
	return (TRUE);
}

t_bool	setup_healthpack(t_item *item, t_game *game)
{
	t_uint	d;

	d = 0;
	while (d < DIR_COUNT)
	{
		item->vent->frame[d] = assets_get_image(game->assets, "healthpack");
		d++;
	}
	item->pfunc = collect_healthpack;
	return (TRUE);
}
