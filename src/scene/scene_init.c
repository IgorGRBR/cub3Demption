/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene_init.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ihhrabar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 16:44:50 by ihhrabar          #+#    #+#             */
/*   Updated: 2024/05/21 16:44:54 by ihhrabar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include <math.h>
#include <stdlib.h>
#include "cub3d_core.h"
#include "ydefines.h"
#include "ylist.h"
#include "ytypes.h"
#include "yvec2.h"

static void	scene_init2(t_scene *scene, t_game *game)
{
	t_ivec2	rc_res;

	(void)game;
	scene->timing = (t_timing_info){0.0, 0.0, 0.0};
	scene->res_scale_factor = (double)game->config.window_size.y
		/ (double)game->config.resolution_height;
	rc_res.x = ceil(game->config.window_size.x / scene->res_scale_factor);
	rc_res.y = ceil(game->config.window_size.y / scene->res_scale_factor);
	scene->rc_target = image_new(game->window_ctx, rc_res);
	scene->rc_depth = image_copy(game->window_ctx, scene->rc_target);
	scene->final_target = image_copy(game->window_ctx, scene->rc_target);
	init_camera(&scene->camera, game->config.fov,
		(double)scene->rc_target->size.x / (double)scene->rc_target->size.y);
	scene->particles = list_new();
	scene->hud.action_timer = 0.0;
	scene->hud.hp = image_new_color(game->window_ctx,
			ivec2_new(PLAYER_MAX_HEALTH, 1), g_red);
	scene->hud.ammo = image_new_color(game->window_ctx,
			ivec2_new(PLAYER_MAX_AMMO, 1), g_yellow);
}

void	scene_init(t_scene *scene, t_game *game)
{
	scene->skybox = 0;
	scene->actors = tslist_new();
	scene->grid_size = ivec2_zero();
	scene->grid = YNULL;
	scene->minimap = image_new_color(game->window_ctx, ivec2_new(2
				* MMAP_RADIUS + 1, 2 * MMAP_RADIUS + 1), g_black);
	scene->player = (t_player){
		.actor = YNULL,
	};
	scene->default_assets = (struct s_default_assets){
		color_new(12, 18, 223, 255), color_new(64, 64, 64, 255),
	{YNULL, YNULL, YNULL, YNULL}
	};
	scene->path = YNULL;
	scene->props = tslist_new();
	scene->phys_bodies = tslist_new();
	scene->npcs = tslist_new();
	scene->pickups = list_new();
	scene_init2(scene, game);
}

static void	scene_deinit_pt2(t_scene *scene, t_game *game)
{
	t_uint	i;

	i = 0;
	while (i < DIR_COUNT)
	{
		if (scene->default_assets.wall_tex[i]
			&& scene->default_assets.wall_tex[i]
			!= assets_get_image(game->assets, "default_wall"))
			image_delete(scene->default_assets.wall_tex[i], game->window_ctx);
		i++;
	}
	(tslist_refresh(scene->npcs));
	list_capply(scene->npcs->data, (t_capply_lfn)npc_delete,
		scene);
	(tslist_delete(scene->npcs));
	list_capply(scene->actors->data, (t_capply_lfn)actor_delete, scene);
	(tslist_delete(scene->actors), tslist_refresh(scene->phys_bodies));
	list_capply(scene->phys_bodies->data, (t_capply_lfn)phys_body_deinit,
		scene);
	tslist_delete(scene->phys_bodies);
	scene_deinit_assets(scene, game);
}

void	scene_deinit(t_scene *scene, t_game *game)
{
	(delete_particles(scene), list_delete(scene->particles));
	scene_refresh(scene, game);
	if (scene->path)
	{
		list_apply(scene->path, (t_apply_lfn)free);
		list_delete(scene->path);
	}
	if (scene->player.vent)
		vis_ent_delete(scene->player.vent, scene);
	tslist_refresh(scene->props);
	list_capply(scene->props->data, (t_capply_lfn)prop_delete, scene);
	tslist_delete(scene->props);
	list_capply(scene->pickups, (t_capply_lfn)pickup_delete, scene);
	list_delete(scene->pickups);
	scene_deinit_pt2(scene, game);
	image_delete(scene->minimap, game->window_ctx);
	image_delete(scene->rc_target, game->window_ctx);
	image_delete(scene->rc_depth, game->window_ctx);
	image_delete(scene->final_target, game->window_ctx);
	(free(scene->grid), scene->grid = YNULL);
}

void	scene_delete(t_scene *scene, t_game *game)
{
	scene_deinit(scene, game);
	free(scene);
}
