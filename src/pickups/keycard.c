/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keycard.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ihhrabar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/27 15:38:12 by ihhrabar          #+#    #+#             */
/*   Updated: 2024/06/27 15:38:13 by ihhrabar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "ydefines.h"
#include "yprint.h"

static const char	*g_kc_names[KC_COUNT] = {
	"green keycard",
	"orange keycard",
	"purple keycard",
	"cyan keycard",
};

static t_bool	collect(t_player *plr, t_uint tag)
{
	plr->keycards[tag] = TRUE;
	yprintf("player has collected %s\n", g_kc_names[tag]);
	return (TRUE);
}

static const char	*g_kc_textures[KC_COUNT] = {
	"green_keycard",
	"orange_keycard",
	"purple_keycard",
	"cyan_keycard",
};

static const char	*get_kc_name(t_string *kc)
{
	t_uint		i;
	t_door_type	dt;

	dt = door_type_from_char_str(kc);
	if (dt == DOOR_NONE)
		return (YNULL);
	i = (t_uint)dt - 1;
	return (g_kc_textures[i]);
}

t_bool	setup_keycard(t_item *item, t_game *game, t_string *kc)
{
	t_uint		d;
	const char	*kcn;

	d = 0;
	kcn = get_kc_name(kc);
	if (!kcn)
		return (FALSE);
	item->tag = (t_uint)door_type_from_char_str(kc) - 1;
	while (d < DIR_COUNT)
	{
		item->vent->frame[d] = assets_get_image(game->assets, kcn);
		d++;
	}
	item->pfunc = collect;
	return (TRUE);
}
