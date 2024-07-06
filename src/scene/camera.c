/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ihhrabar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 18:45:21 by ihhrabar          #+#    #+#             */
/*   Updated: 2024/06/04 18:45:22 by ihhrabar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "yprint.h"
#include "yvec2.h"
#include "yvec3.h"
#include <math.h>

void	init_camera(t_camera *camera, double fov, double aspect_ratio)
{
	double	plane_height;

	fov = g_pi / 180.0 * fov;
	camera->plane_hwidth = tan(fov / 2.0);
	camera->fov = fov;
	plane_height = camera->plane_hwidth / aspect_ratio;
	camera->vfov = atan(plane_height) * 2.0;
	camera->dir = dvec2_new(1.0, 0.0);
	camera->pos = dvec3_zero();
	yprintf("cam vfov: %d\n", (int)(camera->vfov * (180.0 / g_pi)));
}

t_dvec2_pair	camera_get_lr_worldspace_nn(t_camera *cam, t_dvec2 dir)
{
	t_dvec2_pair	lr;
	t_dvec2			plane;

	plane = dvec2_scale(dvec2_new(-dir.y, dir.x), cam->plane_hwidth);
	lr.a = dvec2_sub(dir, plane);
	lr.b = dvec2_add(dir, plane);
	return (lr);
}

t_dvec2_pair	camera_get_lr_worldspace(t_camera *cam, t_dvec2 dir)
{
	t_dvec2_pair	lr;

	lr = camera_get_lr_worldspace_nn(cam, dir);
	lr.a = dvec2_normalized(lr.a);
	lr.b = dvec2_normalized(lr.b);
	return (lr);
}
