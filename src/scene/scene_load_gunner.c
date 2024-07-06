/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene_load_gunner.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ihhrabar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 19:24:36 by ihhrabar          #+#    #+#             */
/*   Updated: 2024/06/11 19:24:37 by ihhrabar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "cub3d_core.h"
#include "yvec2.h"
// #include "yvec2.h"

static t_dvec2	_collided(t_npc *plr, t_phys_body *other,
			t_dvec2 resp)
{
	if (other->par_type == PBP_NODE
		|| other->par_type == PBP_NPC
		|| other->par_type == PBP_PLAYER)
	{
		if (resp.x != 0.0)
			plr->actor->pbody.vel.x = 0.0;
		if (resp.y != 0.0)
			plr->actor->pbody.vel.y = 0.0;
		return (resp);
	}
	return (dvec2_zero());
}

static void	_post_physics(t_npc *npc, t_scene *scene)
{
	npc_post_physics(npc, scene);
}

void	scene_load_gunner(t_npc *npc, t_scene *scene, t_game *game)
{
	(void)scene;
	npc->thinker.current_state = (t_think_func)gunner_random_walker;
	npc->thinker.data = &npc->think_data;
	npc->actor->pbody.col_cb = (t_pb_collision_func)_collided;
	npc->actor->pbody.phy_cb = (t_physics_cb_func)_post_physics;
	npc->actor->pbody.parent = npc;
	npc->actor->pbody.par_type = PBP_NPC;
	npc->splayer.sprite = assets_get_sprite(game->assets, "defbody_idle");
	frame_set(&npc->vent->frame, spr_plr_get_frame(&npc->splayer));
}
