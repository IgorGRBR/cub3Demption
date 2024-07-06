/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_c.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ihhrabar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 19:31:22 by ihhrabar          #+#    #+#             */
/*   Updated: 2024/06/11 19:31:23 by ihhrabar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "cub3d_core.h"
#include "cub3d_generic.h"
#include "ystring.h"
#include "yvec2.h"
#include "ydefines.h"
#include <stdlib.h>

static const int	g_unit = (1 << 31);

double	random_unit(void)
{
	return (((double)randomn(g_unit)) / (double)g_unit);
}

int	randomn(int n)
{
	return (random() % n);
}

t_npc_type	get_npc_type(t_string *type)
{
	if (string_equal_cstr(type, "gunner"))
		return (NPC_GUNNER);
	error_exit("unknown npc type provided");
	return (0);
}

// delta = ivec2_sub(get_mouse_pos(game->window_ctx), window_hs);
// set_mouse_pos(game->window_ctx, window_hs);
t_ivec2	mouse_delta(t_game *game)
{
	t_ivec2	delta;
	t_ivec2	window_hs;
	t_ivec2	window_qs;
	t_ivec2	mouse_pos;

	mouse_pos = get_mouse_pos(game->window_ctx);
	delta = ivec2_sub(mouse_pos, game->mouse_pos);
	game->mouse_pos = mouse_pos;
	window_hs = ivec2_divide(game->gfx->display->size, 2);
	window_qs = ivec2_divide(window_hs, 2);
	if (mouse_pos.x < window_hs.x - window_qs.x
		|| mouse_pos.x > window_hs.x + window_qs.x
		|| mouse_pos.y < window_hs.y - window_qs.y
		|| mouse_pos.y > window_hs.y + window_qs.y)
	{
		set_mouse_pos(game->window_ctx, window_hs);
		game->mouse_pos = ivec2_sub(window_hs, delta);
	}
	return (delta);
}

t_game	*exit_data(t_game *addr, t_bool write)
{
	static t_game	*game = YNULL;

	if (write)
		game = addr;
	return (game);
}
