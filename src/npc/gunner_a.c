/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gunner_a.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ihhrabar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/24 13:25:13 by ihhrabar          #+#    #+#             */
/*   Updated: 2024/06/24 13:25:14 by ihhrabar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "cub3d_core.h"
#include "ydefines.h"
#include "ylist.h"
#include "ytypes.h"
#include "yvec2.h"
#include "yvec3.h"
#include <math.h>
#include <stdlib.h>

static double	move_to_node(t_npc *npc, t_ivec2 target)
{
	double	dist;
	t_dvec2	diff;

	diff = dvec2_sub(dvec2_new((double)target.x + 0.5, (double)target.y + 0.5),
			dvec3_xz(npc->actor->pos));
	dist = dvec2_length(diff);
	if (dist != 0.0)
		npc->actor->dir = dvec2_normalized(diff);
	return (dist);
}

void	npc_traverse_path(t_npc_thinker_data *td, t_scene *scene,
			t_ivec2 target)
{
	double		dist;

	td->me->inputs.move = dvec2_new(0.0, -1.0);
	if (td->path == YNULL)
		td->path = scene_find_path(scene, dvec2_floor(dvec3_xz(
						td->me->actor->pos)), target);
	if (td->path == YNULL || td->path->size == 0)
	{
		(npc_td_clear_path(td), td->me->inputs.move = dvec2_new(0.0, 0.0));
		return ;
	}
	dist = move_to_node(td->me, *(t_ivec2 *)list_get(td->path,
				td->path->size - 1));
	if (dist < 0.5 + td->me->actor->pbody.half_radius.x)
		free(list_pop(td->path));
}

static const double	g_sqrt2 = 0.70710678118655;

static t_bool	try_detect_player(t_npc_thinker_data *td,
					t_scene *scene)
{
	double		dist;
	t_dvec2		diff;
	t_rc_res	rc;

	diff = dvec2_sub(dvec3_xz(scene->player.actor->pos),
			dvec3_xz(td->me->actor->pos));
	dist = dvec2_length(diff);
	if (dist > td->detection_range)
		return (FALSE);
	if (dvec2_dot(td->me->actor->dir, dvec2_normalized(diff))
		>= g_sqrt2)
	{
		diff = dvec2_normalized(diff);
		rc = scene_cast_ray(scene, td->me->actor->pos,
				dvec3_new(diff.x, 0.0, diff.y),
				(t_pb_rc_config){td->detection_range, PBP_PLAYER | PBP_NODE});
		if (rc.hit && rc.hit_body.par_type == PBP_PLAYER)
			td->target = scene->player.actor;
		return (rc.hit && rc.hit_body.par_type == PBP_PLAYER);
	}
	return (FALSE);
}

static t_thunk	go_back_to_spawn(t_npc_thinker_data *td,
					t_scene *scene, double dt)
{
	t_inputs	inp;
	double		dist;

	inp = td->me->inputs;
	inp.move = dvec2_new(0.0, -1.0);
	dist = dvec2_length(dvec2_sub(dvec3_xz(td->me->actor->pos),
				td->spawn_pos));
	update_gunner_anim(td, scene, NSTATE_WALK, dt);
	if (dist <= td->wander_distance)
		return (npc_td_clear_path(td), inp.move = dvec2_new(0.0, 0.0),
			(t_thunk){inp, (t_think_func)gunner_random_walker});
	npc_traverse_path(td, scene, dvec2_floor(td->spawn_pos));
	if (try_detect_player(td, scene))
		return ((t_thunk){inp, (t_think_func)gunner_chase_player});
	return ((t_thunk){inp, (t_think_func)go_back_to_spawn});
}

t_thunk	gunner_random_walker(t_npc_thinker_data *think_data, t_scene *scene,
					double dt)
{
	t_inputs	inp;
	double		origin_dist;

	inp = think_data->me->inputs;
	if (think_data->timer > 0.0)
	{
		if (try_detect_player(think_data, scene))
			return ((t_thunk){inp, (t_think_func)gunner_chase_player});
		update_gunner_anim(think_data, scene, NSTATE_WALK, dt);
		think_data->timer -= dt;
		return ((t_thunk){inp, (t_think_func)gunner_random_walker});
	}
	origin_dist = dvec2_length(dvec2_sub(dvec3_xz(think_data->me->actor->pos),
				think_data->spawn_pos));
	think_data->timer = 2.0;
	if (think_data->wander_distance == 0.0)
		return ((t_thunk){inp, (t_think_func)gunner_random_walker});
	else if (origin_dist > think_data->wander_distance)
		return ((t_thunk){inp, (t_think_func)go_back_to_spawn});
	inp.look = atan2(think_data->me->actor->dir.x,
			think_data->me->actor->dir.y) + g_pi * (random_unit() - 0.5);
	think_data->me->actor->dir = dvec2_new(cos(inp.look), sin(inp.look));
	inp.move = dvec2_scale(dvec2_new(0.0, -1.0), (double)randomn(2));
	return ((t_thunk){inp, (t_think_func)gunner_random_walker});
}
