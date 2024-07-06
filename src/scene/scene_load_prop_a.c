/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene_load_prop_a.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ihhrabar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/08 16:02:38 by ihhrabar          #+#    #+#             */
/*   Updated: 2024/06/08 16:02:40 by ihhrabar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "cub3d_core.h"
#include "ystring.h"
#include "ytypes.h"

static	void	spawn_orb(t_game *game, t_prop *prop)
{
	prop->solid = FALSE;
	prop->vent->frame[DIR_SOUTH] = assets_get_image(game->assets,
			"orb");
}

t_bool	scene_load_prop(t_game *game, t_prop *prop, t_string *type)
{
	if (string_equal_cstr(type, "dummy"))
		return (scene_load_dummy(prop, game), TRUE);
	else if (string_equal_cstr(type, "orb"))
		return (spawn_orb(game, prop), TRUE);
	return (FALSE);
}
