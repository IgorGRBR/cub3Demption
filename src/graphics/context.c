/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   context.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ihhrabar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/09 16:25:39 by ihhrabar          #+#    #+#             */
/*   Updated: 2024/05/09 16:25:42 by ihhrabar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "cub3d_core.h"
#include "ylist.h"
#include <stdlib.h>

t_gfx_ctx	*gfx_new(t_wctx wctx, t_ivec2 size, t_uint tcount)
{
	t_gfx_ctx	*gfx;

	gfx = malloc(sizeof(t_gfx_ctx));
	gfx->target = image_new(wctx, size);
	gfx->display = image_new(wctx, size);
	gfx->transform = g_default2d;
	gfx->thread_count = tcount;
	gfx->threads = list_new();
	while (tcount > 0)
	{
		tcount--;
		list_insert(gfx->threads, malloc(sizeof (t_drawing_thread)));
	}
	gfx->running = TRUE;
	init_rendering_threads(gfx);
	return (gfx);
}

void	gfx_delete(t_gfx_ctx *gfx, t_wctx wctx)
{
	gfx->running = FALSE;
	deinit_rendering_threads(gfx);
	list_apply(gfx->threads, free);
	list_delete(gfx->threads);
	if (gfx->target)
		image_delete(gfx->target, wctx);
	if (gfx->display)
		image_delete(gfx->display, wctx);
	free(gfx);
}
