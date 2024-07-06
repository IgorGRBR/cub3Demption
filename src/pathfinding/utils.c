/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ihhrabar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 19:58:34 by ihhrabar          #+#    #+#             */
/*   Updated: 2024/06/04 19:58:35 by ihhrabar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "cub3d_core.h"
#include "ylist.h"
#include "ymath.h"
#include "ytypes.h"
#include <stdlib.h>

static t_ivec2	add_dir(t_ivec2 v, t_direction d)
{
	if (d == DIR_EAST)
		v.x++;
	else if (d == DIR_SOUTH)
		v.y++;
	else if (d == DIR_WEST)
		v.x--;
	else if (d == DIR_NORTH)
		v.y--;
	return (v);
}

t_list	*directions_to_vecs(t_list *dirs, t_ivec2 start)
{
	t_list		*vecs;
	t_list_iter	it;
	t_ivec2		*v;

	vecs = list_new();
	it = list_iter(dirs);
	while (list_iter_next(&it))
	{
		v = (t_ivec2 *)malloc(sizeof (t_ivec2));
		*v = add_dir(start, *(t_direction *)it.value);
		start = *v;
		list_insert(vecs, v);
		free(it.value);
	}
	list_delete(dirs);
	return (vecs);
}

// Funzione euristica (distanza di Manhattan)
int	pf_node_dist(t_ivec2 a, t_ivec2 b)
{
	return (int_abs(a.x - b.x) + int_abs(a.y - b.y));
}
