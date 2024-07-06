/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   text_drawing.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ihhrabar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/09 16:29:18 by ihhrabar          #+#    #+#             */
/*   Updated: 2024/05/09 16:29:22 by ihhrabar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#if BACKEND == MLX
# include "cub3d_core.h"
# include "cub3d_generic.h"
# include "mlx.h"

void	draw_debug(t_game *game, t_string *text, t_ivec2 pos, t_color color)
{
	draw_debug_cstr(game, text->cstr, pos, color);
}

void	draw_debug_cstr(t_game *game, const char *text, t_ivec2 pos,
			t_color color)
{
	int	*c;

	c = (int *)&color;
	mlx_string_put(game->window_ctx.mlx, game->window_ctx.mlx_win,
		pos.x, pos.y, *c, (char *)text);
}

#endif
