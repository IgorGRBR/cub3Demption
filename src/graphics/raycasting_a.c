/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycasting_a.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ihhrabar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 18:47:26 by ihhrabar          #+#    #+#             */
/*   Updated: 2024/06/04 18:47:27 by ihhrabar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "cub3d_core.h"
// #include "ydefines.h"
// #include "ymath.h"
// #include "ytypes.h"
// #include <math.h>

#ifdef NO_MT_RENDERING

static const double		g_rmin = 0.1;
static const double		g_rmax = 0.9;

void	raycast_scene(t_scene *scene, t_gfx_ctx *gfx)
{
	(void)gfx;
	draw_ceil_floor_colors(scene, g_rmin, g_rmax);
	image_clear(scene->rc_depth, depth_as_color(200.0f));
	raycast_region(scene, g_rmin, g_rmax, scene->timing.draw_interp);
	draw_floors_and_ceilings(scene, g_rmin, g_rmax);
}

#else

void	raycast_scene(t_scene *scene, t_gfx_ctx *gfx)
{
	image_clear(scene->rc_depth, depth_as_color(200.0f));
	draw_multithreaded(gfx, DTHREAD_SCENE, (t_dthread_data)scene);
}

#endif
