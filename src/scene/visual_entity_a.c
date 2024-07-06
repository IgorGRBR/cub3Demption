/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   visual_entity_a.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ihhrabar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 18:44:26 by ihhrabar          #+#    #+#             */
/*   Updated: 2024/06/04 18:44:27 by ihhrabar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "cub3d_core.h"
#include "ydefines.h"
#include "ylist.h"
#include "ytypes.h"
#include "yvec2.h"
#include "yvec3.h"
#include <stdlib.h>

static void	remove_from(t_vis_ent *vent, t_scene *scene, t_ivec2_pair tlbr)
{
	t_ivec2			i;
	t_grid_node		*gn;

	i = tlbr.a;
	while (i.x <= tlbr.b.x)
	{
		i.y = tlbr.a.y;
		while (i.y <= tlbr.b.y)
		{
			gn = scene_node_at_i2(scene, i);
			if (gn && gn->vis_ents)
			{
				list_remove(gn->vis_ents, vent);
				if (gn->vis_ents->size == 0)
					(list_delete(gn->vis_ents),
						gn->vis_ents = YNULL);
			}
			i.y++;
		}
		i.x++;
	}
}

static void	insert_to(t_vis_ent *vent, t_scene *scene, t_ivec2_pair tlbr)
{
	t_ivec2			i;
	t_grid_node		*gn;

	i = tlbr.a;
	while (i.x <= tlbr.b.x)
	{
		i.y = tlbr.a.y;
		while (i.y <= tlbr.b.y)
		{
			gn = scene_node_at_i2(scene, i);
			if (gn)
			{
				if (!gn->vis_ents)
					gn->vis_ents = list_new();
				list_insert(gn->vis_ents, vent);
			}
			i.y++;
		}
		i.x++;
	}
}

t_vis_ent	*vis_ent_new(t_scene *scene, t_dvec3 pos)
{
	t_vis_ent		*vent;
	int				d;
	t_ivec2_pair	tlbr;

	vent = (t_vis_ent *)malloc(sizeof(t_vis_ent));
	vent->tf = g_default2d;
	vent->pos = pos;
	vent->_old_pos = pos;
	vent->radius = 1.0;
	tlbr.a = dvec2_floor(dvec2_sub(dvec3_xz(pos),
				dvec2_new(vent->radius, vent->radius)));
	tlbr.b = dvec2_floor(dvec2_add(dvec3_xz(pos),
				dvec2_new(vent->radius, vent->radius)));
	insert_to(vent, scene, tlbr);
	d = -1;
	while (++d < DIR_COUNT)
		vent->frame[d] = YNULL;
	vent->visible = TRUE;
	vent->dir = dvec2_new(1.0, 0.0);
	return (vent);
}

void	vis_ent_move(t_vis_ent *vent, t_dvec3 new_pos, t_scene *scene)
{
	t_ivec2_pair	ntlbr;
	t_ivec2_pair	otlbr;

	ntlbr.a = dvec2_floor(dvec2_sub(dvec3_xz(new_pos),
				dvec2_new(vent->radius, vent->radius)));
	ntlbr.b = dvec2_floor(dvec2_add(dvec3_xz(new_pos),
				dvec2_new(vent->radius, vent->radius)));
	otlbr.a = dvec2_floor(dvec2_sub(dvec3_xz(vent->pos),
				dvec2_new(vent->radius, vent->radius)));
	otlbr.b = dvec2_floor(dvec2_add(dvec3_xz(vent->pos),
				dvec2_new(vent->radius, vent->radius)));
	vent->_old_pos = vent->pos;
	vent->pos = new_pos;
	if (ivec2_eq(otlbr.a, ntlbr.a) && ivec2_eq(otlbr.b, ntlbr.b))
		return ;
	remove_from(vent, scene, otlbr);
	insert_to(vent, scene, ntlbr);
}

void	vis_ent_delete(t_vis_ent *vent, t_scene *scene)
{
	t_ivec2_pair	tlbr;

	tlbr.a = dvec2_floor(dvec2_sub(dvec3_xz(vent->pos),
				dvec2_new(vent->radius, vent->radius)));
	tlbr.b = dvec2_floor(dvec2_add(dvec3_xz(vent->pos),
				dvec2_new(vent->radius, vent->radius)));
	remove_from(vent, scene, tlbr);
	free(vent);
}
