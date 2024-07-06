/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   phys_update.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ihhrabar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 19:21:52 by ihhrabar          #+#    #+#             */
/*   Updated: 2024/06/20 19:35:54 by ihhrabar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "cub3d_core.h"
#include "ydefines.h"
#include "ylist.h"
#include "ytypes.h"
#include "yvec2.h"

static t_phys_body	get_node_box(t_ivec2 gpos, t_grid_node *gn)
{
	t_dvec2	hr;

	hr = dvec2_new(0.5, 0.5);
	if (gn->type == GNT_DOOR && !gn->data.door.open)
	{
		hr.x = 0.5 - 0.4 * gn->data.door.vertical;
		hr.y = 0.5 - 0.4 * !gn->data.door.vertical;
	}
	return ((t_phys_body){
		.half_radius = hr,
		.pos = dvec2_new((double)gpos.x + 0.5, (double)gpos.y + 0.5),
		.vel = dvec2_zero(),
		._cvel = dvec2_zero(),
		.col_cb = YNULL,
		.phy_cb = YNULL,
		.parent = gn,
		.par_type = PBP_NODE
	});
}

static t_bool	phys_body_vs_the_world(t_phys_body *pb, t_scene *scene,
					t_ivec2 i)
{
	t_grid_node	*gn;
	t_phys_body	gn_box;
	t_dvec2		resp;
	t_bool		result;

	result = FALSE;
	while (++i.x <= 1)
	{
		i.y = -2;
		while (++i.y <= 1)
		{
			gn = scene_node_at_i2(scene, ivec2_add(dvec2_floor(pb->pos), i));
			if (!gn || !node_is_solid(gn))
				continue ;
			gn_box = get_node_box(ivec2_add(dvec2_floor(pb->pos), i), gn);
			resp = resolve_collision(pb, &gn_box, pb->_cvel);
			if (resp.x == 0.0 && resp.y == 0.0)
				continue ;
			resp = pb->col_cb(pb->parent, &gn_box, resp);
			pb->_cvel = dvec2_add(pb->_cvel, resp);
			result = TRUE;
		}
	}
	return (result);
}

static t_list	*get_near_bodies(t_scene *scene, t_ivec2 gpos)
{
	t_list		*near;
	t_ivec2		i;
	t_grid_node	*gn;
	t_list_iter	it;

	near = list_new();
	i = ivec2_new(-1, -1);
	while (i.x <= 1)
	{
		i.y = -1;
		while (i.y <= 1)
		{
			gn = scene_node_at_i2(scene, ivec2_add(gpos, i));
			if (gn && gn->phys_bodies)
			{
				it = list_iter(gn->phys_bodies);
				while (list_iter_next(&it))
					list_insert(near, it.value);
			}
			i.y++;
		}
		i.x++;
	}
	return (near);
}

static t_bool	phys_body_update(t_phys_body *pb, t_scene *scene)
{
	t_list		*near;
	t_list_iter	it;
	t_dvec2		resp;
	t_bool		result;

	if (!pb->phy_cb)
		return (FALSE);
	near = get_near_bodies(scene, dvec2_floor(pb->pos));
	it = list_iter(near);
	result = FALSE;
	while (list_iter_next(&it))
	{
		if (it.value != pb)
		{
			resp = resolve_collision(pb, it.value, pb->_cvel);
			if (resp.x != 0.0 || resp.y != 0.0)
			{
				resp = pb->col_cb(pb->parent, it.value, resp);
				pb->_cvel = dvec2_add(pb->_cvel, resp);
				result = (resp.x != 0.0 || resp.y != 0.0);
			}
		}
	}
	list_delete(near);
	return (result || phys_body_vs_the_world(pb, scene, ivec2_new(-2, -2)));
}

void	physics_update(t_scene *scene)
{
	t_list_iter	it;
	t_phys_body	*pb;
	t_uint		iters;

	it = tslist_iter(scene->phys_bodies);
	while (list_iter_next(&it))
	{
		iters = 0;
		((t_phys_body *)it.value)->_cvel = ((t_phys_body *)it.value)->vel;
		while (iters < PHYSICS_ITER_COUNT
			&& phys_body_update(it.value, scene))
			iters++;
	}
	list_iter_begin(&it);
	while (list_iter_next(&it))
	{
		pb = it.value;
		if (pb->_cvel.x != 0.0 && pb->_cvel.y != 0.0)
		{
			phys_body_move(pb, scene, dvec2_add(pb->pos, pb->_cvel));
			pb->_cvel = dvec2_zero();
		}
		if (pb->phy_cb)
			pb->phy_cb(pb->parent, scene);
	}
}
