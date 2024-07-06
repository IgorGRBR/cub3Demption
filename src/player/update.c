/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ihhrabar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/08 16:01:53 by ihhrabar          #+#    #+#             */
/*   Updated: 2024/06/08 16:01:54 by ihhrabar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d_core.h"
#include "cub3d.h"
#include "yprint.h"
#include "ytypes.h"
#include "yvec2.h"
#include "yvec3.h"
#include <math.h>

static const double	g_knockback_force = 0.4;
static const double	g_fire_time = 0.2;
static const double	g_frlen = 24.0;

static void	handle_shooting(t_player *plr, t_scene *scene, double dt)
{
	t_rc_res	rs;
	t_dvec3		hit_pos;

	plr->keys[GK_SHOOT] = FALSE;
	if (scene->hud.action_timer > 0.0 || plr->ammo == 0)
		return ;
	plr->ammo--;
	scene->hud.action_time = g_fire_time;
	scene->hud.action_timer = g_fire_time;
	scene->hud.cur_spr = scene->hud.fire_spr;
	rs = scene_cast_ray(scene, plr->actor->pos,
			dvec3_new(plr->actor->dir.x, 0.0, plr->actor->dir.y),
			(t_pb_rc_config){.max_len = g_frlen, .type = PBP_NPC | PBP_NODE});
	if (rs.hit && rs.hit_body.par_type == PBP_NPC)
	{
		npc_get_hit(rs.hit_body.parent, 2, dvec2_scale(plr->actor->dir,
				g_knockback_force * dt), (t_think_func)gunner_get_hit);
		yprintf("Gocha!\n");
	}
	if (rs.hit)
	{
		hit_pos = dvec3_lerp(rs.ray_pos, rs.hit_pos, 0.95);
		spawn_particle(scene, hit_pos, PRT_BULLET_IMPACT);
	}
}

static void	handle_interactions(t_player *plr, t_scene *scene, double dt)
{
	t_rc_res	rs;
	t_grid_node	*gn;

	(void)dt;
	yprintf("Trying to use something!\n");
	plr->keys[GK_USE] = FALSE;
	rs = scene_cast_ray(scene, plr->actor->pos,
			dvec3_new(plr->actor->dir.x, 0.0, plr->actor->dir.y),
			(t_pb_rc_config){.max_len = 0.5, .type = PBP_DOOR});
	if (rs.hit)
	{
		gn = rs.hit_body.parent;
		if (gn->type == GNT_DOOR)
		{
			if (gn->data.door.type == DOOR_NONE
				|| (plr->keycards[(int)gn->data.door.type - 1]))
				gn->data.door.open = !gn->data.door.open;
			else
				yprintf("can't open this door without matching keycard\n");
		}
	}
}

void	player_update(t_player *plr, t_scene *scene, t_game *game, double dt)
{
	player_update_inputs(plr);
	plr->actor->dir = scene->camera.dir;
	actor_update(plr->actor, plr->inputs, scene, dt);
	if (plr->actor->health <= 0 && !plr->exit_status)
	{
		yprintf("You are dead!\n");
		plr->exit_status = TRUE;
	}
	if (plr->keys[GK_SHOOT])
		handle_shooting(plr, scene, dt);
	if (plr->keys[GK_USE])
		handle_interactions(plr, scene, dt);
	(void)game;
}

void	player_rt_update(t_player *plr, t_scene *scene, t_game *game,
			double dt)
{
	double	angle;
	t_ivec2	m_delta;

	m_delta = ivec2_zero();
	if (game->config.mouselook)
		m_delta = mouse_delta(game);
	plr->look_vector += (double)m_delta.x * plr->look_sensitivity / dt;
	if (plr->look_vector == 0.0)
		plr->inputs.look = (int)plr->keys[GK_LOOK_RIGHT]
			- (int)plr->keys[GK_LOOK_LEFT];
	else
		plr->inputs.look = plr->look_vector / 1000.0;
	angle = atan2(scene->camera.dir.y, scene->camera.dir.x);
	angle += plr->inputs.look * plr->look_sensitivity * dt;
	scene->camera.dir = dvec2_new(cos(angle), sin(angle));
	scene->camera.pos = dvec3_add(plr->vent->_old_pos,
			dvec3_scale(dvec3_sub(plr->vent->pos, plr->vent->_old_pos),
				scene->timing.draw_interp));
	plr->look_vector = 0.0;
}
