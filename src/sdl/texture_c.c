/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture_c.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ihhrabar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/24 15:16:51 by ihhrabar          #+#    #+#             */
/*   Updated: 2024/06/24 15:16:52 by ihhrabar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#if BACKEND == SDL
# include "cub3d_generic.h"
# include "yvec2.h"

t_ivec2	texture_get_size(t_texture *texture)
{
	return (ivec2_new(texture->w, texture->h));
}

int	texture_get_width(t_texture *texture)
{
	return (texture->w);
}

int	texture_get_height(t_texture *texture)
{
	return (texture->h);
}

#endif
