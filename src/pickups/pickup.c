/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pickup.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ihhrabar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/27 15:38:17 by ihhrabar          #+#    #+#             */
/*   Updated: 2024/06/27 15:38:18 by ihhrabar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "ydefines.h"
#include "yvec3.h"
#include <stdlib.h>

t_item	*pickup_new(t_scene *scene, t_dvec2 pos)
{
	t_item	*item;

	item = (t_item *)malloc(sizeof (t_item));
	item->pfunc = YNULL;
	item->collected = FALSE;
	phys_body_init(&item->pbody, scene, 0.15, pos);
	item->pbody.par_type = PBP_ITEM;
	item->pbody.parent = item;
	item->vent = vis_ent_new(scene, dvec3_new(pos.x, 0.0, pos.y));
	item->tag = 0;
	return (item);
}

void	pickup_delete(t_item *item, t_scene *scene)
{
	phys_body_deinit(&item->pbody, scene);
	vis_ent_delete(item->vent, scene);
	free(item);
}

t_bool	pickup_filter(t_item *item, t_scene *scene)
{
	t_bool	result;

	result = item->collected;
	if (item->collected)
		pickup_delete(item, scene);
	return (result);
}
