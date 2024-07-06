/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pathfind_b.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ihhrabar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/20 18:51:45 by ihhrabar          #+#    #+#             */
/*   Updated: 2024/06/20 18:51:46 by ihhrabar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "cub3d_core.h"
#include "ydefines.h"
#include "ylist.h"
#include "ytypes.h"
#include "yvec2.h"

// Direzioni di movimento (destra, sinistra, su, giù)
static const t_ivec2	g_d[4] = {
	(t_ivec2){1, 0},
	(t_ivec2){-1, 0},
	(t_ivec2){0, 1},
	(t_ivec2){0, -1},
};

static void	a_star_sub_step(t_list *olist, t_pf_node *node,
				t_ivec2 npos, t_ivec2 end)
{
	int			tentative_cost;
	t_pf_node	*neighbor_node;

	tentative_cost = node->cost + 1;
	neighbor_node = find_pf_node_in_list(olist, npos);
	if (neighbor_node == YNULL)
	{
		neighbor_node = create_pf_node(npos, node);
		neighbor_node->cost = tentative_cost;
		neighbor_node->dist = pf_node_dist(npos, end);
		neighbor_node->sum = neighbor_node->cost + neighbor_node->dist;
		list_insert(olist, neighbor_node);
	}
	else if (tentative_cost < neighbor_node->cost)
	{
		neighbor_node->parent = node;
		neighbor_node->cost = tentative_cost;
		neighbor_node->sum = neighbor_node->cost + neighbor_node->dist;
	}
}

void	_a_star_step(int **grid, t_ivec2 v2[2], t_pf_node *node,
			t_list *lists[2])
{
	int		i;
	t_ivec2	npos;

	i = 0;
	while (i < 4)
	{
		npos = ivec2_sub(node->pos, g_d[i]);
		if (npos.x >= 0 && npos.x < v2[0].x && npos.y >= 0
			&& npos.y < v2[0].y && grid[npos.x][npos.y] == 0)
		{
			if (!pf_node_in_list(lists[1], npos))
				a_star_sub_step(lists[0], node, npos, v2[1]);
		}
		i++;
	}
}

t_pf_node	*_get_next_node(t_list *olist)
{
	t_pf_node	*current_node;
	t_pf_node	*other_node;
	t_list_iter	it;

	current_node = list_get(olist, 0);
	it = list_iter(olist);
	while (list_iter_next(&it))
	{
		other_node = it.value;
		if (other_node->sum < current_node->sum)
			current_node = other_node;
	}
	return (current_node);
}
