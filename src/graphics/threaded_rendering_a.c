/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threaded_rendering_a.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ihhrabar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/09 16:25:58 by ihhrabar          #+#    #+#             */
/*   Updated: 2024/05/09 16:26:02 by ihhrabar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "ydefines.h"
#include "ylist.h"
#include "ytypes.h"
#include "yprint.h"
#include <bits/pthreadtypes.h>
#include <pthread.h>

void	init_rendering_threads(t_gfx_ctx *gfx)
{
	t_list_iter			it;
	t_drawing_thread	*thread;
	t_uint				i;

	it = list_iter(gfx->threads);
	i = 0;
	while (list_iter_next(&it))
	{
		thread = it.value;
		thread->id = i;
		thread->op_type = DTHREAD_NONE;
		thread->active = FALSE;
		thread->min_range = (double)i / (double)gfx->thread_count;
		thread->max_range = ((double)i + 1.0) / (double)gfx->thread_count;
		thread->gfx = gfx;
		pthread_mutex_init(&thread->drawing_mutex, YNULL);
		pthread_cond_init(&thread->drawing_cond, 0);
		pthread_create(&thread->pthread, 0,
			(void *(*)(void *))_draw_thread_func, thread);
		i++;
	}
}

void	deinit_rendering_threads(t_gfx_ctx *gfx)
{
	t_list_iter			it;
	t_drawing_thread	*thread;

	it = list_iter(gfx->threads);
	yprintf("deinitializing threads\n");
	while (list_iter_next(&it))
	{
		thread = it.value;
		thread->active = TRUE;
		pthread_mutex_unlock(&thread->drawing_mutex);
		pthread_cond_signal(&thread->drawing_cond);
		pthread_join(thread->pthread, YNULL);
		pthread_mutex_destroy(&thread->drawing_mutex);
		pthread_cond_destroy(&thread->drawing_cond);
	}
}
