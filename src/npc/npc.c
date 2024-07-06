/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   npc.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ihhrabar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/20 19:35:21 by ihhrabar          #+#    #+#             */
/*   Updated: 2024/06/20 19:35:22 by ihhrabar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "cub3d_core.h"
#include "ydefines.h"
// #include "yprint.h"
#include "yvec2.h"
#include "yvec3.h"
// #include <math.h>
#include <stdlib.h>

t_npc	*npc_new(t_scene *scene, t_actor *actor)
{
	t_npc	*npc;

	npc = (t_npc *)malloc(sizeof (t_npc));
	thinker_init(&npc->thinker, &npc->think_data, YNULL);
	npc->actor = actor;
	tslist_insert(scene->actors, npc->actor);
	npc->vent = vis_ent_new(scene, npc->actor->pos);
	npc->think_data.timer = 1.0;
	npc->think_data.me = npc;
	npc->think_data.spawn_pos = dvec3_xz(actor->pos);
	npc->think_data.path = YNULL;
	npc->think_data.target = YNULL;
	npc->think_data.wander_distance = 1.0;
	npc->think_data.detection_range = 6.0;
	npc->think_data.attack_range = 4.0;
	inputs_clear(&npc->inputs);
	npc->inputs.look = 0.0;
	npc->inputs.move = dvec2_zero();
	spr_plr_init(&npc->splayer, YNULL);
	return (npc);
}

	// yprintf("updating npc\n");
		// npc->actor->dir = dvec2_new(cos(npc->inputs.look),
		// 	sin(npc->inputs.look));
	// inputs_clear(&npc->inputs);
void	npc_update(t_npc *npc, t_scene *scene, t_game *game)
{
	(void)game;
	npc->inputs = thinker_update(&npc->thinker, scene,
			scene->timing.delta_time);
	if (npc->actor)
	{
		actor_update(npc->actor, npc->inputs, scene,
			scene->timing.delta_time);
		if (npc->actor->health <= 0)
			tslist_remove(scene->npcs, npc);
		npc->vent->dir = npc->actor->dir;
		spr_plr_update(&npc->splayer, scene->timing.delta_time);
		if (npc->vent)
			frame_set(&npc->vent->frame, spr_plr_get_frame(&npc->splayer));
	}
}

static const double	g_hurt_time = 0.4;

void	npc_get_hit(t_npc *npc, t_uint damage, t_dvec2 knockback,
			t_think_func hurt_func)
{
	npc->actor->pbody.vel = knockback;
	npc->actor->health -= damage;
	npc->think_data.timer = g_hurt_time;
	npc->thinker.current_state = hurt_func;
	npc->actor->slide = TRUE;
}

void	npc_post_physics(t_npc *npc, t_scene *scene)
{
	if (npc->actor)
	{
		actor_post_physics(npc->actor, scene);
		if (npc->vent)
			vis_ent_move(npc->vent, npc->actor->pos, scene);
	}
}

void	npc_delete(t_npc *npc, t_scene *scene)
{
	thinker_deinit(&npc->thinker);
	if (npc->actor)
		tslist_remove(scene->actors, npc->actor);
	if (npc->vent)
		vis_ent_delete(npc->vent, scene);
	if (npc->think_data.path)
		npc_td_clear_path(&npc->think_data);
	free(npc);
}
