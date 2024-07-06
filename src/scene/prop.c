/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ihhrabar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/20 19:36:12 by ihhrabar          #+#    #+#             */
/*   Updated: 2024/06/20 19:36:14 by ihhrabar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include <stdlib.h>
// #include "yprint.h"

t_prop	*prop_new(t_scene *scene, t_dvec3 pos, t_dvec2 dir)
{
	t_prop	*prop;

	prop = (t_prop *)malloc(sizeof (t_prop));
	prop->pos = pos;
	prop->dir = dir;
	prop->vent = vis_ent_new(scene, pos);
	prop->solid = FALSE;
	return (prop);
}

void	prop_move(t_prop *prop, t_dvec3 new_pos, t_scene *scene)
{
	vis_ent_move(prop->vent, new_pos, scene);
	prop->pos = new_pos;
}

void	prop_delete(t_prop *prop, t_scene *scene)
{
	vis_ent_delete(prop->vent, scene);
	free(prop);
}
