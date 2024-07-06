/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hud_a.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ihhrabar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/25 17:45:05 by ihhrabar          #+#    #+#             */
/*   Updated: 2024/06/25 17:45:05 by ihhrabar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "cub3d_core.h"
#include "ylist.h"
#include "ytypes.h"
#include "yvec2.h"
#include "ymath.h"
#include <math.h>

static void	update_hp_ammo(t_scene *scene)
{
	int	i;

	image_clear(scene->hud.hp, g_black);
	image_clear(scene->hud.ammo, g_black);
	i = -1;
	while (++i < scene->player.actor->health)
		image_set_pixel(scene->hud.hp, i, 0, g_red);
	i = -1;
	while (++i < (int)scene->player.ammo)
		image_set_pixel(scene->hud.ammo, i, 0, g_yellow);
}

void	hud_update(t_scene *scene, t_game *game, double dt)
{
	int	frame;

	(void)game;
	frame = 0;
	if (!scene->hud.cur_spr)
		return ;
	if (scene->hud.cur_spr == scene->hud.fire_spr)
	{
		scene->hud.action_timer -= dt;
		if (scene->hud.action_timer < 0)
			scene->hud.cur_spr = scene->hud.aim_spr;
		frame = int_clamp((int)floor((double)scene->hud.cur_spr->size
					* (1.0 - scene->hud.action_timer
						/ scene->hud.action_time)),
				0, scene->hud.cur_spr->size - 1);
	}
	scene->hud.arm = list_get(scene->hud.cur_spr, frame);
	update_hp_ammo(scene);
}

void	hud_draw(t_scene *scene)
{
	t_transform2d	tf;
	double			factor;

	tf = g_default2d;
	factor = (double)scene->final_target->size.y / (double)MIN_WIN_H;
	tf.pos = dvec2_new((double)scene->final_target->size.x / 2.0,
			(double)scene->final_target->size.y);
	tf.sca = dvec2_scale(dvec2_new(1.5, 1.5), factor);
	if (scene->hud.arm)
		image_draw_into_tf(scene->final_target, scene->hud.arm, tf);
	tf.pos.x = (double)scene->final_target->size.x * 0.1;
	tf.pos.y = (double)scene->final_target->size.y * 0.88;
	tf.sca = dvec2_scale(dvec2_new(4.0, 4.0), factor);
	if (scene->hud.hp)
		image_draw_into_tf(scene->final_target, scene->hud.hp, tf);
	tf.pos.y = (double)scene->final_target->size.y * 0.9;
	if (scene->hud.ammo)
		image_draw_into_tf(scene->final_target, scene->hud.ammo, tf);
}
