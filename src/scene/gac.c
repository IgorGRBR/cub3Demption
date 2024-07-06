/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gac.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ihhrabar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/18 18:07:46 by ihhrabar          #+#    #+#             */
/*   Updated: 2024/06/18 18:07:47 by ihhrabar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "cub3d_core.h"
#include "ylist.h"

static void	init_hud(t_scene *scene, t_game *game)
{
	scene->gac.hud.pistol = list_new();
	list_insert(scene->gac.hud.pistol,
		assets_get_image(game->assets, "fp_pistol_aim"));
	scene->gac.hud.pistol_fire = list_new();
	list_insert(scene->gac.hud.pistol_fire,
		assets_get_image(game->assets, "fp_pistol_fire_0"));
	list_insert(scene->gac.hud.pistol_fire,
		assets_get_image(game->assets, "fp_pistol_fire_1"));
	list_insert(scene->gac.hud.pistol_fire,
		assets_get_image(game->assets, "fp_pistol_fire_2"));
	scene->hud.fire_spr = scene->gac.hud.pistol_fire;
	scene->hud.aim_spr = scene->gac.hud.pistol;
	scene->hud.cur_spr = scene->hud.aim_spr;
}

void	scene_init_assets(t_scene *scene, t_game *game)
{
	t_gac	*gac;

	gac = &scene->gac;
	gac->particles.bullet_impact = assets_get_sprite(game->assets,
			"bullet_impact");
	scene_init_bandit_assets(scene, game);
	init_hud(scene, game);
}

void	scene_deinit_assets(t_scene *scene, t_game *game)
{
	t_gac	*gac;

	gac = &scene->gac;
	list_delete(gac->hud.pistol);
	list_delete(gac->hud.pistol_fire);
	image_delete(scene->hud.hp, game->window_ctx);
	image_delete(scene->hud.ammo, game->window_ctx);
}
