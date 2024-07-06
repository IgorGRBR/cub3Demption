/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ihhrabar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/09 16:23:46 by ihhrabar          #+#    #+#             */
/*   Updated: 2024/05/09 16:23:48 by ihhrabar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

t_color	color_new(t_uchar r, t_uchar g, t_uchar b, t_uchar a)
{
	t_color	color;

	color.r = r;
	color.g = g;
	color.b = b;
	color.a = a;
	return (color);
}

float	color_as_depth(t_color col)
{
	return (*(float *)&col);
}

t_color	depth_as_color(float d)
{
	t_color	*c;

	c = (t_color *)&d;
	return (*c);
}
