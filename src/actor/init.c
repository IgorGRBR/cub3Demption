/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ihhrabar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 16:44:04 by ihhrabar          #+#    #+#             */
/*   Updated: 2024/05/21 16:44:09 by ihhrabar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "yvec3.h"
// #include "yprint.h"
#include <stdlib.h>

// yprintf("new actor\n");
t_actor	*actor_new(t_scene *scene, t_dvec3 pos, t_dvec2 dir)
{
	t_actor	*actor;

	actor = (t_actor *)malloc(sizeof (t_actor));
	actor->pos = pos;
	actor->dir = dir;
	actor->run_speed = 1.5;
	actor->health = 10;
	actor->slide = FALSE;
	phys_body_init(&actor->pbody, scene, 0.2, dvec3_xz(pos));
	return (actor);
}

// yprintf("delete actor\n");
void	actor_delete(t_actor *actor, t_scene *scene)
{
	phys_body_deinit(&actor->pbody, scene);
	free(actor);
}
