/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   frame.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ihhrabar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/20 19:36:31 by ihhrabar          #+#    #+#             */
/*   Updated: 2024/06/20 19:36:32 by ihhrabar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "ydefines.h"
#include "yvec2.h"
#include <stdlib.h>

t_frame	*frame_new(void)
{
	t_frame		*frame;
	t_direction	i;

	frame = (t_frame *)malloc(sizeof (t_frame));
	i = DIR_EAST;
	while (i < DIR_COUNT)
	{
		(*frame)[i] = YNULL;
		i++;
	}
	return (frame);
}

void	frame_set(t_frame *target, t_frame *source)
{
	t_direction	i;

	if (!target || !source)
		return ;
	i = DIR_EAST;
	while (i < DIR_COUNT)
	{
		(*target)[i] = (*source)[i];
		i++;
	}
}

static const double	g_sqrt2 = 0.70710678118655;

t_image	*frame_get_image(t_frame *frame, t_camera *cam,
			t_dvec2 own_dir, int *xscale)
{
	double	det;
	double	dot;

	*xscale = 1;
	dot = dvec2_dot(cam->dir, own_dir);
	det = cam->dir.x * own_dir.y - cam->dir.y * own_dir.x;
	if (dot > g_sqrt2)
		return ((*frame)[DIR_NORTH]);
	else if (det > g_sqrt2)
	{
		if ((*frame)[DIR_EAST])
			return ((*frame)[DIR_EAST]);
		*xscale = -1;
		return ((*frame)[DIR_WEST]);
	}
	else if (det < -g_sqrt2)
	{
		if ((*frame)[DIR_WEST])
			return ((*frame)[DIR_WEST]);
		*xscale = -1;
		return ((*frame)[DIR_EAST]);
	}
	return ((*frame)[DIR_SOUTH]);
}

void	frame_delete(t_frame *frame)
{
	free(frame);
}
