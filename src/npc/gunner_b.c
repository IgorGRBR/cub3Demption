/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gunner_b.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ihhrabar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/24 14:42:04 by ihhrabar          #+#    #+#             */
/*   Updated: 2024/06/24 14:42:05 by ihhrabar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "cub3d_core.h"
#include "ytypes.h"
#include "yvec2.h"
#include "yvec3.h"
#include "yprint.h"
#include <math.h>

static const double	g_fire_time = 0.5;
static const double	g_aim_time = 0.2;

void	update_gunner_anim(t_npc_thinker_data *td,
			t_scene *scene, t_npc_state nstate, double dt)
{
	td->me->splayer.speed = 0.0;
	if (nstate == NSTATE_WALK)
	{
		td->me->splayer.sprite = scene->gac.sprites.bandit.idle;
		if (td->me->inputs.move.x != 0.0 || td->me->inputs.move.y != 0.0)
			td->me->splayer.sprite = scene->gac.sprites.bandit.walk;
		td->me->splayer.speed = dvec2_length(td->me->actor->pbody.vel) * 6.0
			/ dt;
	}
	else if (nstate == NSTATE_CHASE_TARGET)
	{
		td->me->splayer.sprite = scene->gac.sprites.bandit.aim;
		if (td->me->inputs.move.x != 0.0 || td->me->inputs.move.y != 0.0)
			td->me->splayer.sprite = scene->gac.sprites.bandit.walkg;
		td->me->splayer.speed = dvec2_length(td->me->actor->pbody.vel) * 6.0
			/ dt;
	}
	else if (nstate == NSTATE_ATTACK)
	{
		td->me->splayer.sprite = scene->gac.sprites.bandit.fire;
		td->me->splayer.frame = (int)floor((1.0 - td->timer / g_fire_time)
				* (double)td->me->splayer.sprite->size);
	}
	else if (nstate == NSTATE_HIT)
		td->me->splayer.sprite = scene->gac.sprites.bandit.hurt;
}

static void	process_hit(t_npc_thinker_data *td, t_scene *scene, t_rc_res rc)
{
	t_player	*plr;
	t_dvec3		hit_pos;

	(void)td;
	if (rc.hit_body.par_type & PBP_PLAYER)
	{
		plr = (t_player *)rc.hit_body.parent;
		plr->actor->health -= 1;
		yprintf("spawn blood particle\n");
	}
	hit_pos = dvec3_lerp(rc.ray_pos, rc.hit_pos, 0.95);
	spawn_particle(scene, hit_pos, PRT_BULLET_IMPACT);
}

t_thunk	shoot_player(t_npc_thinker_data *td, t_scene *scene, double dt)
{
	t_inputs	inp;
	t_rc_res	rc;

	update_gunner_anim(td, scene, NSTATE_ATTACK, dt);
	inp = td->me->inputs;
	inp.move = dvec2_zero();
	if (td->timer == g_fire_time)
	{
		rc = scene_cast_ray(scene, td->me->actor->pos,
				dvec3_new(td->me->actor->dir.x, 0.0, td->me->actor->dir.y),
				(t_pb_rc_config){12.0, PBP_PLAYER | PBP_NODE});
		if (rc.hit)
			process_hit(td, scene, rc);
	}
	else if (td->timer <= 0.0)
		return (td->timer = 0.0,
			(t_thunk){inp, (t_think_func)gunner_chase_player});
	return (td->timer -= dt, (t_thunk){inp, (t_think_func)shoot_player});
}

static t_thunk	aim_at_target(t_npc_thinker_data *td, t_scene *scene, double dt)
{
	t_inputs	inp;

	inp = td->me->inputs;
	inp.move = dvec2_zero();
	td->me->splayer.frame = 0.0;
	if (td->timer <= 0.0)
	{
		td->timer = g_fire_time;
		return ((t_thunk){inp, (t_think_func)shoot_player});
	}
	td->me->splayer.sprite = scene->gac.sprites.bandit.aim;
	return (td->timer -= dt, (t_thunk){inp, (t_think_func)aim_at_target});
}

// yprintf("trying to attack the player\n");
t_thunk	gunner_chase_player(t_npc_thinker_data *td, t_scene *scene, double dt)
{
	t_inputs	inp;
	t_dvec2		diff;
	t_rc_res	rc;
	double		dist;

	inp = td->me->inputs;
	inp.move = dvec2_new(0.0, -1.0);
	(update_gunner_anim(td, scene, NSTATE_CHASE_TARGET, dt), diff = dvec2_sub
		(dvec3_xz(td->target->pos), dvec3_xz(td->me->actor->pos)));
	dist = dvec2_length(diff);
	diff = dvec2_normalized(diff);
	rc = scene_cast_ray(scene, td->me->actor->pos,
			dvec3_new(diff.x, 0.0, diff.y),
			(t_pb_rc_config){12.0, PBP_PLAYER | PBP_NODE});
	if (rc.hit && rc.hit_body.par_type == PBP_PLAYER)
	{
		td->me->actor->dir = diff;
		td->timer = g_aim_time;
		if (dist < td->attack_range && random_unit() < (td->attack_range - dist)
			/ td->attack_range)
			return ((t_thunk){inp, (t_think_func)aim_at_target});
	}
	else
		npc_traverse_path(td, scene, dvec2_floor(dvec3_xz(td->target->pos)));
	return ((t_thunk){inp, (t_think_func)gunner_chase_player});
}
