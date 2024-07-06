/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thinker.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ihhrabar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 19:22:12 by ihhrabar          #+#    #+#             */
/*   Updated: 2024/06/20 19:36:45 by ihhrabar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "ydefines.h"

void	thinker_init(t_thinker *think, void *data,
			t_think_func init_state)
{
	think->current_state = init_state;
	think->data = data;
}

void	thinker_deinit(t_thinker *think)
{
	think->current_state = YNULL;
	think->data = YNULL;
}

t_inputs	thinker_update(t_thinker *think, t_scene *scene, double dt)
{
	t_thunk	result;

	result = think->current_state(think->data, scene, dt);
	think->current_state = result.next;
	return (result.inputs);
}
