/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   image_colors.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ihhrabar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/09 16:26:27 by ihhrabar          #+#    #+#             */
/*   Updated: 2024/05/09 16:26:31 by ihhrabar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "cub3d_core.h"
#include "cub3d_generic.h"

void	image_set_pixel(t_image *img, int x, int y, t_color color)
{
	texture_set_pixel(img->_data, x, y, color);
}

t_color	image_get_pixel(t_image *img, int x, int y)
{
	return (texture_get_pixel(img->_data, x, y));
}

void	image_set_pixel_unsafe(t_image *img, int x, int y, t_color color)
{
	texture_set_pixel_unsafe(img->_data, x, y, color);
}

t_color	image_get_pixel_unsafe(t_image *img, int x, int y)
{
	return (texture_get_pixel_unsafe(img->_data, x, y));
}
