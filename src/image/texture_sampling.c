/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture_sampling.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ihhrabar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/08 15:01:11 by ihhrabar          #+#    #+#             */
/*   Updated: 2024/06/08 15:01:13 by ihhrabar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "cub3d_generic.h"
#include "ymath.h"
#include <math.h>

t_color	sample_color(t_texture *img, t_dvec2 s_pos,
			t_dvec2_pair quad, t_ivec2 pd)
{
	t_ivec2	texture_pos;
	t_ivec2	tex_size;

	tex_size = texture_get_size(img);
	s_pos.x = double_clamp((s_pos.x * quad.b.x) + quad.a.x, 0.0, 1.0);
	s_pos.y = double_clamp((s_pos.y * quad.b.y) + quad.a.y, 0.0, 1.0);
	texture_pos.x = int_clamp((int)floor((double)tex_size.x * s_pos.x), 0,
			tex_size.x - 1);
	texture_pos.y = int_clamp((int)floor((double)tex_size.y * s_pos.y), 0,
			tex_size.y - 1);
	if (pd.x)
		texture_pos.x = tex_size.x - texture_pos.x - 1;
	if (pd.y)
		texture_pos.y = tex_size.y - texture_pos.y - 1;
	return (texture_get_pixel_unsafe(img, texture_pos.x, texture_pos.y));
}

t_color	sample_color_round(t_texture *img, t_dvec2 s_pos,
			t_dvec2_pair quad, t_ivec2 pd)
{
	t_ivec2	texture_pos;
	t_ivec2	tex_size;

	tex_size = texture_get_size(img);
	s_pos.x = double_clamp((s_pos.x * quad.b.x) + quad.a.x, 0.0, 1.0);
	s_pos.y = double_clamp((s_pos.y * quad.b.y) + quad.a.y, 0.0, 1.0);
	texture_pos.x = int_clamp((int)round((double)tex_size.x * s_pos.x), 0,
			tex_size.x - 1);
	texture_pos.y = int_clamp((int)round((double)tex_size.y * s_pos.y), 0,
			tex_size.y - 1);
	if (pd.x)
		texture_pos.x = tex_size.x - texture_pos.x - 1;
	if (pd.y)
		texture_pos.y = tex_size.y - texture_pos.y - 1;
	return (texture_get_pixel_unsafe(img, texture_pos.x, texture_pos.y));
}
