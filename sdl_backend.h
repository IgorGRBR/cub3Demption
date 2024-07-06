/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sdl_backend.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ihhrabar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/01 17:06:12 by ihhrabar          #+#    #+#             */
/*   Updated: 2023/12/01 17:06:13 by ihhrabar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SDL_BACKEND_H
# define SDL_BACKEND_H
# if BACKEND == SDL
#  include "cub3d_core.h"
#  define WINDOW_TITLE "cub3d (SDL2)"

// SDL backend specific functions go here.

# endif
#endif
