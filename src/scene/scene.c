/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ihhrabar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 16:44:32 by ihhrabar          #+#    #+#             */
/*   Updated: 2024/05/21 16:44:37 by ihhrabar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ydefines.h"
#include "ytypes.h"
#include "cub3d.h"
#include "cub3d_core.h"
#include "yvec2.h"

t_grid_node	*scene_node_at(t_scene *scene, int x, int y)
{
	if (x < scene->grid_size.x && y < scene->grid_size.y
		&& x >= 0 && y >= 0)
		return (&scene->grid[x + y * scene->grid_size.x]);
	else
		return (YNULL);
}

t_grid_node	*scene_node_at_i2(t_scene *scene, t_ivec2 pos)
{
	return (scene_node_at(scene, pos.x, pos.y));
}

t_grid_node	*scene_node_at_d2(t_scene *scene, t_dvec2 pos)
{
	return (scene_node_at_i2(scene, dvec2_floor(pos)));
}
