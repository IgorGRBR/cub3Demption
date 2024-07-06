/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   phys_body.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ihhrabar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/08 17:05:37 by ihhrabar          #+#    #+#             */
/*   Updated: 2024/06/08 17:05:38 by ihhrabar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "cub3d_core.h"
#include "ydefines.h"
#include "ylist.h"
#include "yvec2.h"

void	phys_body_init(t_phys_body *pb, t_scene *scene, double half_radius,
			t_dvec2 pos)
{
	t_grid_node	*gn;

	pb->half_radius = dvec2_new(half_radius, half_radius);
	pb->pos = pos;
	pb->vel = dvec2_zero();
	pb->_cvel = dvec2_zero();
	gn = scene_node_at_i2(scene, dvec2_floor(pos));
	if (gn)
	{
		if (!gn->phys_bodies)
			gn->phys_bodies = list_new();
		list_insert(gn->phys_bodies, pb);
	}
	tslist_insert(scene->phys_bodies, pb);
	pb->parent = YNULL;
	pb->col_cb = YNULL;
	pb->phy_cb = YNULL;
	pb->par_type = PBP_NONE;
}

void	phys_body_move(t_phys_body *pb, t_scene *scene,
			t_dvec2 new_pos)
{
	t_grid_node	*gn;
	t_ivec2		new_gpos;
	t_dvec2		old_pos;

	new_gpos = dvec2_floor(new_pos);
	old_pos = pb->pos;
	pb->pos = new_pos;
	if (ivec2_eq(dvec2_floor(old_pos), new_gpos))
		return ;
	gn = scene_node_at_i2(scene, dvec2_floor(old_pos));
	if (gn && gn->phys_bodies)
	{
		list_remove(gn->phys_bodies, pb);
		if (gn->phys_bodies->size == 0)
			(list_delete(gn->phys_bodies),
				gn->phys_bodies = YNULL);
	}
	gn = scene_node_at_i2(scene, new_gpos);
	if (gn)
	{
		if (!gn->phys_bodies)
			gn->phys_bodies = list_new();
		list_insert(gn->phys_bodies, pb);
	}
}

void	phys_body_deinit(t_phys_body *pb, t_scene *scene)
{
	t_grid_node	*gn;

	gn = scene_node_at_i2(scene, dvec2_floor(pb->pos));
	if (gn)
	{
		list_remove(gn->phys_bodies, pb);
		if (gn->phys_bodies->size == 0)
			(list_delete(gn->phys_bodies),
				gn->phys_bodies = YNULL);
	}
	tslist_remove(scene->phys_bodies, pb);
}
