/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ihhrabar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/08 16:00:53 by ihhrabar          #+#    #+#             */
/*   Updated: 2024/06/08 16:00:54 by ihhrabar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d_core.h"
#include "yvec2.h"
#include "yvec3.h"

static t_dvec3	move_dir(t_dvec2 inp, t_dvec2 dir)
{
	t_dvec2	sdir;

	inp = dvec2_normalized(inp);
	sdir.x = -dir.y;
	sdir.y = dir.x;
	dir = dvec2_scale(dir, -inp.y);
	sdir = dvec2_scale(sdir, inp.x);
	sdir = dvec2_add(dir, sdir);
	return (dvec3_new(sdir.x, 0, sdir.y));
}

void	actor_update(t_actor *actor, t_inputs inp, t_scene *scene, double dt)
{
	if (actor->health <= 0)
		return ;
	if (!actor->slide)
	{
		if (dvec2_length_sq(inp.move) > 0.0)
		{
			actor->pbody.vel = dvec3_xz(dvec3_scale(
						move_dir(inp.move, actor->dir),
						actor->run_speed * dt));
		}
		else
			actor->pbody.vel = dvec2_zero();
	}
	(void)scene;
}

void	actor_post_physics(t_actor *actor)
{
	actor->pos.x = actor->pbody.pos.x;
	actor->pos.z = actor->pbody.pos.y;
}
