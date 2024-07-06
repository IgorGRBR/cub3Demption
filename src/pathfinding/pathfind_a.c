/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pathfind_a.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ihhrabar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/20 19:13:08 by ihhrabar          #+#    #+#             */
/*   Updated: 2024/06/20 19:13:09 by ihhrabar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "cub3d_core.h"
#include "cub3d.h"
#include "ylist.h"
#include "ydefines.h"
#include "ytypes.h"
#include "yvec2.h"

// Ricostruisci il percorso e restituiscilo come array di t_ivec2
		// yprintf("result step: %d %d\n", vec2->x, vec2->y);
t_list	*reconstruct_path(t_pf_node *node)
{
	t_list	*path;
	t_ivec2	*vec2;

	path = list_new();
	while (node)
	{
		vec2 = malloc(sizeof (t_ivec2));
		*vec2 = node->pos;
		list_insert_at(path, 0, vec2);
		node = node->parent;
	}
	return (path);
}

// Funzione per liberare la memoria dei nodi
void	free_nodes(t_list *olist, t_list *clist)
{
	list_apply(olist, (t_apply_lfn)free);
	list_apply(clist, (t_apply_lfn)free);
	list_delete(olist);
	list_delete(clist);
}

// Implementazione dell'algoritmo A*
t_list	*a_star(int **grid, t_ivec2 start, t_ivec2 end, t_ivec2 size)
{
	t_list		*olist;
	t_list		*clist;
	t_pf_node	*node;
	t_list		*path;

	olist = list_new();
	clist = list_new();
	node = create_pf_node(start, YNULL);
	node->dist = pf_node_dist(start, end);
	node->sum = node->cost + node->dist;
	list_insert(olist, node);
	while (olist->size > 0)
	{
		node = _get_next_node(olist);
		if (ivec2_eq(node->pos, end))
			return (path = reconstruct_path(node),
				free_nodes(olist, clist), path);
		(list_remove(olist, node), list_insert(clist, node));
		_a_star_step(grid, (t_ivec2 []){size, end}, node,
			(t_list *[]){olist, clist});
	}
	free_nodes(olist, clist);
	return (NULL);
}

	// yprintf("\n");
static int	**make_grid(t_scene *scene)
{
	int			**grid;
	t_ivec2		i;
	t_grid_node	*gn;

	grid = (int **)malloc(scene->grid_size.x * sizeof(int *));
	i.x = -1;
	while (++i.x < scene->grid_size.x)
		grid[i.x] = (int *)malloc(scene->grid_size.y * sizeof(int));
	i = ivec2_zero();
	while (i.y < scene->grid_size.y)
	{
		i.x = 0;
		while (i.x < scene->grid_size.x)
		{
			gn = scene_node_at_i2(scene, i);
			grid[i.x][i.y] = (gn && (gn->type == GNT_SOLID));
			i.x++;
		}
		i.y++;
	}
	return (grid);
}

t_list	*scene_find_path(t_scene *scene, t_ivec2 start, t_ivec2 end)
{
	int			**grid;
	t_list		*result;
	t_list		*rev;
	t_list_iter	it;
	t_uint		i;

	grid = make_grid(scene);
	rev = a_star(grid, start, end, scene->grid_size);
	i = 0;
	while (i < (t_uint)scene->grid_size.x)
	{
		free(grid[i]);
		i++;
	}
	free(grid);
	if (!rev)
		return (YNULL);
	result = list_new();
	it = list_iter_last(rev);
	while (list_iter_prev(&it))
		list_insert(result, it.value);
	list_delete(rev);
	return (result);
}
