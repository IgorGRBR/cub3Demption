/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture_c.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ihhrabar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/08 16:01:30 by ihhrabar          #+#    #+#             */
/*   Updated: 2024/06/08 16:01:31 by ihhrabar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#if BACKEND == MLX
# include "cub3d_generic.h"

t_ivec2	texture_get_size(t_texture *texture)
{
	return (texture->size);
}

int	texture_get_width(t_texture *texture)
{
	return (texture->size.x);
}

int	texture_get_height(t_texture *texture)
{
	return (texture->size.y);
}

#endif
