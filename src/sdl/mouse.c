/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mouse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ihhrabar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/21 14:31:12 by ihhrabar          #+#    #+#             */
/*   Updated: 2024/06/21 14:31:13 by ihhrabar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#if BACKEND == SDL
# include "cub3d_generic.h"
# include "yvec2.h"
# include <SDL2/SDL_mouse.h>

t_ivec2	get_mouse_pos(t_wctx wctx)
{
	int	x;
	int	y;

	(void)wctx;
	SDL_GetMouseState(&x, &y);
	return (ivec2_new(x, y));
}

void	set_mouse_pos(t_wctx wctx, t_ivec2 pos)
{
	SDL_WarpMouseInWindow(wctx.win, pos.x, pos.y);
}

void	set_cursor_visibile(t_wctx wctx, t_bool visible)
{
	(void)wctx;
	SDL_ShowCursor((int)visible);
}

#endif
