/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ihhrabar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/20 19:36:38 by ihhrabar          #+#    #+#             */
/*   Updated: 2024/06/20 19:36:39 by ihhrabar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "ylist.h"
#include "ydefines.h"
#include <math.h>

void	spr_plr_init(t_sprite_player *sp, t_list *sprite)
{
	sp->sprite = sprite;
	sp->frame = 0.0;
	sp->speed = 1.0;
}

void	spr_plr_update(t_sprite_player *sp, double dt)
{
	if (!sp->sprite)
		return ;
	sp->frame += dt * sp->speed;
	while ((t_uint)floor(sp->frame) >= sp->sprite->size)
		sp->frame -= floor(sp->frame / (double)sp->sprite->size)
			* (double)sp->sprite->size;
}

t_frame	*spr_plr_get_frame(t_sprite_player *sp)
{
	if (sp->sprite)
		return (list_get(sp->sprite, (int)floor(sp->frame)));
	else
		return (YNULL);
}

void	spr_plr_deinit(t_sprite_player *sp)
{
	sp->sprite = YNULL;
	sp->frame = 0.0;
	sp->speed = 0.0;
}
