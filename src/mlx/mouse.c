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

#if BACKEND == MLX
# include "cub3d_generic.h"
# include "mlx.h"
# include "yvec2.h"

t_ivec2	get_mouse_pos(t_wctx wctx)
{
	int	x;
	int	y;

	mlx_mouse_get_pos(wctx.mlx, wctx.mlx_win, &x, &y);
	return (ivec2_new(x, y));
}

void	set_mouse_pos(t_wctx wctx, t_ivec2 pos)
{
	mlx_mouse_move(wctx.mlx, wctx.mlx_win, pos.x, pos.y);
}

void	set_cursor_visibile(t_wctx wctx, t_bool visible)
{
	if (visible)
		mlx_mouse_show(wctx.mlx, wctx.mlx_win);
	else
		mlx_mouse_hide(wctx.mlx, wctx.mlx_win);
}

#endif
