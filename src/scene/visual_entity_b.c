/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   visual_entity_b.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ihhrabar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/24 13:25:36 by ihhrabar          #+#    #+#             */
/*   Updated: 2024/06/24 13:25:37 by ihhrabar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "yvec3.h"

t_dvec3	vis_ent_get_pos(t_vis_ent *vent, double interp)
{
	return (dvec3_add(vent->_old_pos, dvec3_scale(
				dvec3_sub(vent->pos, vent->_old_pos), interp)));
}
