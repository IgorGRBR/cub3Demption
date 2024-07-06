/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   context_drawing.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ihhrabar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/09 16:25:46 by ihhrabar          #+#    #+#             */
/*   Updated: 2024/05/09 16:25:53 by ihhrabar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "cub3d_core.h"
#include "cub3d_generic.h"

static void	swap_image(t_gfx_ctx *gfx)
{
	t_image	*temp;

	temp = gfx->display;
	gfx->display = gfx->target;
	gfx->target = temp;
}

void	gfx_draw_to_screen(t_gfx_ctx *gfx, t_wctx wctx)
{
	swap_image(gfx);
	texture_blit_to_screen(gfx->display->_data, wctx);
}

// TODO
void	gfx_draw_to_screen_threaded(t_gfx_ctx *gfx, t_wctx wctx)
{
	swap_image(gfx);
	(void)wctx;
}
