/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ihhrabar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 18:33:20 by ihhrabar          #+#    #+#             */
/*   Updated: 2024/05/21 18:33:22 by ihhrabar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "cub3d_core.h"
#include "ystring.h"
#include "ytypes.h"

t_bool	node_is_solid(t_grid_node *gn)
{
	return (gn->type == GNT_SOLID || gn->type == GNT_EXIT
		|| (gn->type == GNT_DOOR && !gn->data.door.open));
}

t_bool	node_is_physical(t_grid_node *gn)
{
	return (gn->type == GNT_SOLID || gn->type == GNT_EXIT
		|| gn->type == GNT_DOOR);
}

static const char	*g_dt_alias[KC_COUNT] = {
	"g",
	"o",
	"p",
	"c",
};

t_door_type	door_type_from_char_str(t_string *d)
{
	t_door_type	dt;
	t_uint		i;

	dt = DOOR_NONE;
	if (!d)
		return (DOOR_NONE);
	i = 0;
	while (i < KC_COUNT)
	{
		if (string_equal_cstr(d, g_dt_alias[i]))
			dt = (t_door_type)(i + 1);
		i++;
	}
	return (dt);
}
