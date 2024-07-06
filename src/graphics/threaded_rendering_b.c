/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threaded_rendering_b.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ihhrabar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/09 16:26:06 by ihhrabar          #+#    #+#             */
/*   Updated: 2024/05/09 16:26:12 by ihhrabar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "cub3d_core.h"
#include "ydefines.h"
#include "ylist.h"
#include "ytypes.h"
#include <pthread.h>
#include <unistd.h>

// dinfo.tgt_quad.a.y += thread->id * 32;
static void	draw_func(t_drawing_thread *thread)
{
	if (thread->op_type == DTHREAD_IMAGE)
		mt_draw_image_into(thread);
	else if (thread->op_type == DTHREAD_SCENE)
		mt_draw_scene(thread);
}

void	*_draw_thread_func(t_drawing_thread *thread)
{
	while (thread->gfx->running)
	{
		pthread_mutex_lock(&thread->drawing_mutex);
		while (!thread->active)
			pthread_cond_wait(&thread->drawing_cond, &thread->drawing_mutex);
		if (!thread->gfx->running)
			return (0);
		if (thread->op_type == DTHREAD_NONE)
			error_exit("drawing operation type is DTHREAD_NONE");
		draw_func(thread);
		thread->active = FALSE;
		pthread_mutex_unlock(&thread->drawing_mutex);
	}
	return (YNULL);
}

static void	deactivate_threads(t_list_iter *it)
{
	t_bool	active;

	list_iter_begin(it);
	while (list_iter_next(it))
	{
		active = TRUE;
		while (active)
		{
			pthread_mutex_lock(
				&((t_drawing_thread *)it->value)->drawing_mutex);
			active = ((t_drawing_thread *)it->value)->active;
			pthread_mutex_unlock(
				&((t_drawing_thread *)it->value)->drawing_mutex);
		}
		((t_drawing_thread *)it->value)->op_type = DTHREAD_NONE;
	}
}

void	draw_multithreaded(t_gfx_ctx *gfx, t_dthread_type type,
			t_dthread_data data)
{
	t_list_iter			it;

	it = list_iter(gfx->threads);
	while (list_iter_next(&it))
	{
		pthread_mutex_lock(&((t_drawing_thread *)it.value)->drawing_mutex);
		((t_drawing_thread *)it.value)->op_type = type;
		((t_drawing_thread *)it.value)->data = data;
		((t_drawing_thread *)it.value)->active = TRUE;
		pthread_cond_signal(&((t_drawing_thread *)it.value)->drawing_cond);
		pthread_mutex_unlock(&((t_drawing_thread *)it.value)->drawing_mutex);
	}
	deactivate_threads(&it);
}
