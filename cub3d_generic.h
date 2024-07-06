/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d_generic.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ihhrabar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/09 16:13:07 by ihhrabar          #+#    #+#             */
/*   Updated: 2024/05/09 16:13:09 by ihhrabar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_GENERIC_H
# define CUB3D_GENERIC_H
# include "cub3d_core.h"
# include "ytypes.h"
// # if BACKEND == MLX
// #  include "mlx_backend.h"
// # elif BACKEND == SDL
// #  include "sdl_backend.h"
// # else
// #  error "BACKEND preprocessor variable was not provided"
// # endif

/// Backend stuff:
// Don't touch it. 

// All of the following functions should have backend-specific implementations.
// Their implementations are located under `src/mlx` or `src/sdl`, depending on
// which backend you're compiling with.

// Initializes the backend and configures backend-specific internals
// in the game.
t_bool		init_backend(t_game *game);

// Deinitializes backend.
t_bool		deinit_backend(t_game *game);

// Creates a new window.
t_wctx		make_window(t_ivec2 size);

// Closes and destroys a window.
void		destroy_window(t_wctx wctx);

// Setups the hooks for a window.
void		setup_hooks(t_wctx wctx, t_game *game);

// Runs the update loop of the game.
void		update_loop(t_game *game);

// Set keybind from int (TODO: implement maybe?).
void		set_keybind_int(t_game_key game_key, int i);

// Set keybind from string (TODO: implement maybe?).
void		set_keybind_string(t_game_key game_key, t_string *bind);

// Returns the keybind.
t_raw_key	get_keybind(t_game_key game_key);

t_ivec2		get_mouse_pos(t_wctx wctx);

void		set_mouse_pos(t_wctx wctx, t_ivec2 pos);

void		set_cursor_visibile(t_wctx wctx, t_bool visible);

// Compute a hash of a raw keycode.
t_uint		raw_key_hash(t_raw_key *i);

// Compare 2 raw keys.
t_bool		raw_key_eq(t_raw_key *a, t_raw_key *b);

t_texture	*texture_new(t_wctx wctx, t_ivec2 size);
void		texture_delete(t_texture *texture, t_wctx wctx);
void		texture_set_pixel(t_texture *texture, int x, int y, t_color color);
t_color		texture_get_pixel(t_texture *texture, int x, int y);
void		texture_set_pixel_unsafe(t_texture *texture, int x, int y,
				t_color color);
t_color		texture_get_pixel_unsafe(t_texture *texture, int x, int y);
t_ivec2		texture_get_size(t_texture *texture);
int			texture_get_width(t_texture *texture);
int			texture_get_height(t_texture *texture);

/// Image painting handling
typedef struct s__paint_info
{
	t_ivec2			tl;
	t_ivec2			br;
	t_ivec2			pd;
	t_dvec2_pair	quad;
	t_dvec2			inv_size;
}	t__paint_info;

typedef struct s__depth_info
{
	t_texture	*depth;
	float		threshold;
}	t__depth_info;

typedef struct s__fx_info
{
	t_fx_context	ctx;
	t_fx_func		fx;
}	t__fx_info;

typedef t__paint_info	t__pinfo;

t__pinfo	_prepare_paint_info(t_draw_info dinfo, t_texture *tx);
void		texture_blit_to_screen(t_texture *texture, t_wctx wctx);

/// Text drawing
void		draw_debug(t_game *game, t_string *text, t_ivec2 pos,
				t_color color);
void		draw_debug_cstr(t_game *game, const char *text, t_ivec2 pos,
				t_color color);

#endif
