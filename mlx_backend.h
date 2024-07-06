/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlx_backend.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ihhrabar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/01 17:06:07 by ihhrabar          #+#    #+#             */
/*   Updated: 2023/12/01 17:06:08 by ihhrabar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MLX_BACKEND_H
# define MLX_BACKEND_H
# if BACKEND == MLX
#  include "cub3d_core.h"
#  define WINDOW_TITLE "cub3d (MLX)"

// MLX backend specific stuff goes here.

// Events
enum e_mlx_event
{
	KEYDOWN_CODE = 2,
	KEYUP_CODE = 3,
	MOUSEDOWN_CODE = 4,
	MOUSEUP_CODE = 5,
	MOUSEMOVE_CODE = 6,
	EXPOSE_CODE = 12,
	DESTROY_CODE = 17,
};

enum e_mlx_event_mask
{
	KEYDOWN_MASK = 1L<<0,
	KEYUP_MASK = 1L<<1,
	MOUSEDOWN_MASK = 1L<<2,
	MOUSEUP_MASK = 1L<<3,
	MOUSEMOVE_MASK = 1L<<6,
	EXPOSE_MASK = 1L<<15,
};

int	mlx_key_press_event(int mlx_key, t_game *game);
int	mlx_key_release_event(int mlx_key, t_game *game);
int	mlx_mouse_event(int button, int x, int y, t_game *game);

# endif
#endif
