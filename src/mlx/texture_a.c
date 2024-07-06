/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture_a.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ihhrabar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/09 16:28:41 by ihhrabar          #+#    #+#             */
/*   Updated: 2024/05/09 16:28:46 by ihhrabar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#if BACKEND == MLX
# include "cub3d_generic.h"
# include "cub3d.h"
# include "ydefines.h"
# include "ytypes.h"
# include <stdlib.h>
# include "mlx.h"

t_texture	*texture_new(t_wctx wctx, t_ivec2 size)
{
	t_texture	*tex;
	int			endian;

	if (size.x < 0 || size.y < 0)
		error_exit("Invalid image size");
	tex = malloc(sizeof (t_texture));
	tex->img_ptr = mlx_new_image(wctx.mlx, size.x, size.y);
	tex->pixel_data = mlx_get_data_addr(tex->img_ptr, &tex->bpp,
			&tex->line_length, &endian);
	tex->big_endian = (t_bool)endian;
	tex->bpp_div_8 = tex->bpp / 8;
	tex->size = size;
	return (tex);
}

void	texture_delete(t_texture *texture, t_wctx wctx)
{
	mlx_destroy_image(wctx.mlx, texture->img_ptr);
	texture->img_ptr = YNULL;
	texture->pixel_data = YNULL;
	free(texture);
}

void	texture_set_pixel(t_texture *texture, int x, int y, t_color color)
{
	char	*px_ptr;

	x = x % texture->size.x;
	y = y % texture->size.y;
	x += (x < 0) * texture->size.x;
	y += (y < 0) * texture->size.y;
	px_ptr = texture->pixel_data + y * texture->line_length
		+ x * texture->bpp_div_8;
	color.a = 255 - color.a;
	*(t_color *)px_ptr = color;
}

t_color	texture_get_pixel(t_texture *texture, int x, int y)
{
	char	*px_ptr;
	t_color	color;

	x = x % texture->size.x;
	y = y % texture->size.y;
	x += (x < 0) * texture->size.x;
	y += (y < 0) * texture->size.y;
	px_ptr = texture->pixel_data + y * texture->line_length
		+ x * texture->bpp_div_8;
	color = *(t_color *)px_ptr;
	color.a = 255 - color.a;
	return (color);
}

void	texture_blit_to_screen(t_texture *texture, t_wctx wctx)
{
	mlx_put_image_to_window(
		wctx.mlx,
		wctx.mlx_win,
		texture->img_ptr,
		0, 0);
}

#endif
