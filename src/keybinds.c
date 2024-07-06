/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keybinds.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ihhrabar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/09 16:24:33 by ihhrabar          #+#    #+#             */
/*   Updated: 2024/05/09 16:24:35 by ihhrabar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "cub3d_core.h"
#include "cub3d_generic.h"
#include "ymap.h"
#include "ystring.h"
#include <stdlib.h>

static const char	*g_bind_names[NUMBER_OF_GAME_KEYS] = {
	"none/unknown",
	"forward",
	"backward",
	"strafe_left",
	"strafe_right",
	"look_left",
	"look_right",
	"shoot",
	"use",
	"exit",
	"debug",
};

const char	*get_key_name(t_game_key key)
{
	if (key < NUMBER_OF_GAME_KEYS)
		return (g_bind_names[key]);
	return (g_bind_names[GK_NONE]);
}

t_game_key	get_key_from_string(t_string *key_str)
{
	t_game_key	key;

	key = GK_NONE;
	while (key < NUMBER_OF_GAME_KEYS)
	{
		if (string_equal_cstr(key_str, g_bind_names[key]))
			return (key);
		key++;
	}
	return (GK_NONE);
}

void	configure_keymap(t_game *game)
{
	t_game_key	k;
	t_raw_key	*ptr_r;
	t_game_key	*ptr_k;

	if (game->keymap)
		error_exit("Game keymap is already configured");
	game->keymap = map_new((t_hash_func)raw_key_hash,
			(t_equals_func)raw_key_eq);
	k = 0;
	while (k < NUMBER_OF_GAME_KEYS)
	{
		ptr_k = malloc(sizeof (t_game_key));
		ptr_r = malloc(sizeof (t_raw_key));
		*ptr_k = k;
		*ptr_r = game->config.keymap[k];
		if (!map_insert(game->keymap, ptr_r, ptr_k))
		{
			if (*ptr_r != 0)
				error_exit("Value is already in the keymap");
			(free(ptr_k), free(ptr_r));
		}
		k++;
	}
}
