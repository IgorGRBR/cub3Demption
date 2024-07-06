/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene_validate_a.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ihhrabar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 16:45:50 by ihhrabar          #+#    #+#             */
/*   Updated: 2024/05/21 16:45:51 by ihhrabar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "cub3d_core.h"
#include "ytypes.h"
#include "yvec2.h"
#include "yvec3.h"

static t_bool	check_node(t_scene *scene, t_ivec2 pos)
{
	t_grid_node	*n;
	t_grid_node	*m;

	n = scene_node_at(scene, pos.x, pos.y);
	if (n->type == GNT_EMPTY)
	{
		m = scene_node_at(scene, pos.x + 1, pos.y);
		if (m && m->type != GNT_EMPTY && !node_is_solid(m))
			return (FALSE);
		m = scene_node_at(scene, pos.x - 1, pos.y);
		if (m && m->type != GNT_EMPTY && !node_is_solid(m))
			return (FALSE);
		m = scene_node_at(scene, pos.x, pos.y + 1);
		if (m && m->type != GNT_EMPTY && !node_is_solid(m))
			return (FALSE);
		m = scene_node_at(scene, pos.x, pos.y - 1);
		if (m && m->type != GNT_EMPTY && !node_is_solid(m))
			return (FALSE);
	}
	else if (!node_is_solid(n)
		&& (pos.x == 0 || pos.y == 0 || pos.x == scene->grid_size.x - 1
			|| pos.y == scene->grid_size.y - 1))
		return (FALSE);
	return (TRUE);
}

static void	check_player(t_scene *scene)
{
	t_grid_node	*n;

	if (!scene->player.actor)
		error_exit("a map should contain a player");
	n = scene_node_at_d2(scene, dvec3_xz(scene->player.actor->pos));
	if (!n || n->type != GNT_FLOOR)
		error_exit("player should exist within the bounds of the map");
}

static void	construct_path(t_scene *scene)
{
	t_ivec2		end_pos;
	t_ivec2		i;
	t_grid_node	*gn;

	i = ivec2_zero();
	end_pos = i;
	while (i.x < scene->grid_size.x)
	{
		i.y = 0;
		while (i.y < scene->grid_size.y)
		{
			gn = scene_node_at_i2(scene, i);
			if (gn && gn->type == GNT_EXIT)
				end_pos = i;
			i.y++;
		}
		i.x++;
	}
	scene->path = scene_find_path(scene, dvec2_floor(
				dvec3_xz(scene->player.actor->pos)), end_pos);
}

void	_scene_validate(t_scene *scene)
{
	t_ivec2		i;

	if (!scene)
		error_exit("scene construction failed, check the .cub file");
	i = ivec2_new(-1, -1);
	while (++i.x < scene->grid_size.x)
	{
		i.y = -1;
		while (++i.y < scene->grid_size.y)
		{
			if (!check_node(scene, i))
				error_exit("map must be surrounded by walls");
		}
	}
	check_player(scene);
	construct_path(scene);
}
