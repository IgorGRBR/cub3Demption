/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   image_init.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ihhrabar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/09 16:27:04 by ihhrabar          #+#    #+#             */
/*   Updated: 2024/05/09 16:27:09 by ihhrabar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "cub3d_core.h"
#include "cub3d_generic.h"
#include "ytypes.h"
#include "yvec2.h"
#include <stdlib.h>

t_image	*image_new(t_wctx wctx, t_ivec2 size)
{
	return (image_new_color(wctx, size, g_white));
}

t_image	*image_new_color(t_wctx wctx, t_ivec2 size, t_color color)
{
	t_image	*sur;

	sur = malloc(sizeof(t_image));
	sur->_data = texture_new(wctx, size);
	sur->size = size;
	sur->origin = ivec2_new(0, 0);
	image_clear(sur, color);
	return (sur);
}

void	image_delete(t_image *img, t_wctx wctx)
{
	texture_delete(img->_data, wctx);
	free(img);
}

t_image	*image_copy(t_wctx wctx, t_image *img)
{
	t_image	*res;
	t_ivec2	i;

	res = malloc(sizeof(t_image));
	res->_data = texture_new(wctx, img->size);
	res->size = img->size;
	res->origin = img->origin;
	i = ivec2_zero();
	while (i.y < res->size.y)
	{
		i.x = 0;
		while (i.x < res->size.x)
		{
			image_set_pixel(res, i.x, i.y,
				image_get_pixel(img, i.x, i.y));
			i.x++;
		}
		i.y++;
	}
	return (res);
}
