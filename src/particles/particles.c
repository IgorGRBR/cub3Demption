/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   particles.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ihhrabar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/20 19:35:28 by ihhrabar          #+#    #+#             */
/*   Updated: 2024/06/20 19:35:30 by ihhrabar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "ydefines.h"
#include "ylist.h"
#include "yvec2.h"
#include <math.h>
#include <stdlib.h>

t_particle	*spawn_particle(t_scene *scene, t_dvec3 pos,
					t_particle_type type)
{
	t_particle	*result;

	result = (t_particle *)malloc(sizeof (t_particle));
	result->prop = prop_new(scene, pos, dvec2_new(1.0, 0.0));
	result->lifetime = 1.0;
	result->timer = result->lifetime;
	result->type = PRT_CUSTOM;
	result->sprite = YNULL;
	tslist_insert(scene->props, result->prop);
	_configure_particle(result, scene, type);
	list_insert(scene->particles, result);
	return (result);
}

t_bool	prt_filter(t_particle *prt, t_scene *scene)
{
	t_bool	dead;

	dead = prt->timer <= 0.0;
	if (dead)
	{
		tslist_remove(scene->props, prt->prop);
		free(prt);
	}
	return (dead);
}

void	prt_update(t_particle *prt, t_scene *scene)
{
	int	frame;

	prt->timer -= scene->timing.delta_time;
	if (prt->prop->vent && prt->sprite)
	{
		frame = (int)floor((double)prt->sprite->size * (1.0
					- (prt->timer / prt->lifetime)));
		frame_set(&prt->prop->vent->frame, list_get(prt->sprite, frame));
	}
}

void	update_particles(t_scene *scene)
{
	list_capply(scene->particles, (t_capply_lfn)prt_update, scene);
	list_cfilter(scene->particles, (t_cfilter_lfn)prt_filter, scene);
}

void	delete_particles(t_scene *scene)
{
	t_list_iter	it;

	it = list_iter(scene->particles);
	while (list_iter_next(&it))
	{
		tslist_remove(scene->props, ((t_particle *)it.value)->prop);
		free(it.value);
	}
}
