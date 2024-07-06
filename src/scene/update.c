/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ihhrabar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 18:44:43 by ihhrabar          #+#    #+#             */
/*   Updated: 2024/06/04 18:44:44 by ihhrabar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "ytypes.h"
#include "ylist.h"
#include "ymap.h"

void	scene_refresh(t_scene *scene, t_game *game)
{
	t_map_iter	mit;

	mit = map_iter(scene->npcs->rem);
	while (map_iter_next(&mit))
		npc_delete(mit.value, scene);
	tslist_refresh(scene->npcs);
	mit = map_iter(scene->actors->rem);
	while (map_iter_next(&mit))
		actor_delete(mit.value, scene);
	tslist_refresh(scene->actors);
	mit = map_iter(scene->props->rem);
	while (map_iter_next(&mit))
		prop_delete(mit.value, scene);
	tslist_refresh(scene->props);
	list_cfilter(scene->pickups, (t_cfilter_lfn)pickup_filter, scene);
	tslist_refresh(scene->phys_bodies);
	(void)game;
}

void	scene_update(t_scene *scene, t_game *game, double dt)
{
	t_list_iter	it;

	scene_refresh(scene, game);
	player_update(&scene->player, scene, game, dt);
	minimap_update(scene->minimap, scene);
	physics_update(scene);
	it = tslist_iter(scene->npcs);
	while (list_iter_next(&it))
		npc_update(it.value, scene, game);
	update_particles(scene);
	hud_update(scene, game, dt);
}

void	scene_rt_update(t_scene *scene, t_game *game, double dt)
{
	player_rt_update(&scene->player, scene, game, dt);
	if (scene->player.exit_status)
		game->running = FALSE;
}
