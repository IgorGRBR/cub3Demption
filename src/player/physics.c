/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   physics.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ihhrabar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/20 19:36:06 by ihhrabar          #+#    #+#             */
/*   Updated: 2024/06/20 19:36:07 by ihhrabar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "cub3d_core.h"
#include "yvec2.h"

void	try_collect_item(t_player *plr, t_item *item)
{
	if (item->collected)
		return ;
	item->collected = TRUE;
	if (item->pfunc)
		item->collected = item->pfunc(plr, item->tag);
}

t_dvec2	player_collided(t_player *plr, t_phys_body *other,
			t_dvec2 resp)
{
	t_grid_node	*gn;

	if (other->par_type == PBP_NODE)
	{
		if (resp.x != 0.0)
			plr->actor->pbody.vel.x = 0.0;
		if (resp.y != 0.0)
			plr->actor->pbody.vel.y = 0.0;
		gn = other->parent;
		if (gn->type == GNT_EXIT)
			plr->exit_status = TRUE;
	}
	else if (other->par_type == PBP_NPC)
	{
		if (resp.x != 0.0)
			plr->actor->pbody.vel.x = 0.0;
		if (resp.y != 0.0)
			plr->actor->pbody.vel.y = 0.0;
	}
	else if (other->par_type == PBP_ITEM)
		return (try_collect_item(plr, other->parent), dvec2_zero());
	return (resp);
}

void	player_post_physics(t_player *plr, t_scene *scene)
{
	actor_post_physics(plr->actor, scene);
	vis_ent_move(plr->vent, plr->actor->pos, scene);
}
