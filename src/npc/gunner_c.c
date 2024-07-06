/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gunner_c.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ihhrabar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/25 17:02:26 by ihhrabar          #+#    #+#             */
/*   Updated: 2024/06/25 17:02:27 by ihhrabar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "yvec2.h"

t_thunk	gunner_get_hit(t_npc_thinker_data *td, t_scene *scene,
					double dt)
{
	t_inputs	inp;

	inp = td->me->inputs;
	inp.move = dvec2_zero();
	td->me->splayer.frame = 0.0;
	if (td->timer <= 0.0)
	{
		npc_td_clear_path(td);
		td->me->actor->slide = FALSE;
		return ((t_thunk){inp, (t_think_func)gunner_chase_player});
	}
	td->me->splayer.sprite = scene->gac.sprites.bandit.hurt;
	td->target = scene->player.actor;
	return (td->timer -= dt, (t_thunk){inp, (t_think_func)gunner_get_hit});
}
