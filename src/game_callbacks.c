/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game_callbacks.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ihhrabar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/09 16:23:55 by ihhrabar          #+#    #+#             */
/*   Updated: 2024/05/09 16:23:57 by ihhrabar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "cub3d_core.h"
#include "ymath.h"
// #include "yprint.h"
// #include "ystring.h"

static const double	g_interval = 1.0 / UPDATE_RATE;

// { //DEBUG
// 	t_string *ft_str = string_format_cstr("Frame time (ms): %d",
// 		 (int)(delta_time * 1000));
// 	draw_debug(game, ft_str, ivec2_new(32, 32), g_red);
// 	string_delete(ft_str);
// }
static void	update_and_draw(t_game *game, int frames, double delta_time)
{
	game->scene->timing.delta_time = g_interval;
	while (frames--)
		scene_update(game->scene, game, g_interval);
	game->scene->timing.delta_time = delta_time;
	scene_rt_update(game->scene, game,
		double_min(delta_time, g_interval * MAX_UPDATES));
	scene_draw(game->scene, game->gfx);
	gfx_draw_to_screen(game->gfx, game->window_ctx);
	image_clear(game->gfx->target, g_black);
}

int	game_update(t_game *game)
{
	double			delta_time;
	double			time;
	int				frames;

	time = get_time();
	delta_time = time - game->elapsed_time;
	game->elapsed_time = time;
	frames = 0;
	if (game->running && game->scene)
	{
		game->scene->timing.accumulator += delta_time;
		while (game->scene->timing.accumulator > g_interval)
		{
			game->scene->timing.accumulator -= g_interval;
			frames++;
		}
		frames = int_min(frames, MAX_UPDATES);
		game->scene->timing.draw_interp = game->scene->timing.accumulator
			/ g_interval;
		update_and_draw(game, frames, delta_time);
	}
	else
		game_deinit(game);
	return (game->running);
}

// yprintf("key %s got pressed\n", get_key_name(key));
void	game_key_pressed(t_game *game, t_game_key key)
{
	if (game->scene)
		player_key_pressed(&game->scene->player, key);
	if (key == GK_EXIT)
		game_deinit(game);
}

// yprintf("key %s got released\n", get_key_name(key));
void	game_key_released(t_game *game, t_game_key key)
{
	if (game->scene)
		player_key_released(&game->scene->player, key);
}
