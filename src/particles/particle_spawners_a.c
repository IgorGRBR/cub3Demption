/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   particle_spawners_a.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ihhrabar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/20 19:35:05 by ihhrabar          #+#    #+#             */
/*   Updated: 2024/06/20 19:35:07 by ihhrabar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "cub3d_core.h"

void	configure_bullet(t_particle *prt, t_scene *scene)
{
	prt->type = PRT_BULLET_IMPACT;
	prt->lifetime = 0.25;
	prt->timer = prt->lifetime;
	prt->sprite = scene->gac.particles.bullet_impact;
}

void	_configure_particle(t_particle *prt, t_scene *scene,
			t_particle_type type)
{
	if (type == PRT_CUSTOM)
		return ;
	else if (type == PRT_BULLET_IMPACT)
		configure_bullet(prt, scene);
}
