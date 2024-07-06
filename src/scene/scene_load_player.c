/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene_load_player.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ihhrabar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 16:45:41 by ihhrabar          #+#    #+#             */
/*   Updated: 2024/06/08 16:02:26 by ihhrabar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "yvec2.h"
#include "yvec3.h"

// TODO: this
static void	scene_load_player2(t_player *plr, t_game *game)
{
	int	i;

	(void)game;
	i = -1;
	while (++i < KC_COUNT)
		plr->keycards[i] = FALSE;
	(void)plr;
	plr->ammo = PLAYER_MAX_AMMO;
	plr->actor->health = PLAYER_MAX_HEALTH;
}

void	scene_load_player(t_scene *scene, t_game *game, t_ivec2 pos)
{
	int	i;

	if (scene->player.actor)
		error_exit("Scene already has a player set up!");
	scene->player.actor = actor_new(scene, dvec3_new(
				(double)pos.x + 0.5, 0.0, (double)pos.y + 0.5),
			dvec2_new(1.0, 0.0));
	tslist_insert(scene->actors, scene->player.actor);
	scene->player.vent = vis_ent_new(scene, scene->player.actor->pos);
	scene->player.vent->visible = FALSE;
	i = -1;
	while (++i < NUMBER_OF_GAME_KEYS)
		scene->player.keys[i] = FALSE;
	scene->player.look_sensitivity = (double)game->config.sensitivity
		/ 100.0;
	inputs_clear(&scene->player.inputs);
	scene->player.actor->pbody.parent = &scene->player;
	scene->player.actor->pbody.par_type = PBP_PLAYER;
	scene->player.actor->pbody.col_cb = (t_pb_collision_func)player_collided;
	scene->player.actor->pbody.phy_cb = (t_physics_cb_func)player_post_physics;
	scene->player.actor->run_speed = 2.0;
	scene->camera.pos = scene->player.actor->pos;
	scene->camera.dir = scene->player.actor->dir;
	scene->player.exit_status = FALSE;
	scene_load_player2(&scene->player, game);
}
