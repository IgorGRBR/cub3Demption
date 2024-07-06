/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tga_loader.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ihhrabar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/09 16:27:13 by ihhrabar          #+#    #+#             */
/*   Updated: 2024/05/09 16:27:18 by ihhrabar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "cub3d_core.h"
#include "ydefines.h"
#include "yio.h"
#include "ystring.h"
#include "yvec2.h"
#include <fcntl.h>
#include <unistd.h>

static void	set_pixel(t_image *image, t_ivec2 coord, t_ivec2 inv,
		t_uchar color_data[4])
{
	coord.y = image->size.y - coord.y - 1;
	if (inv.x)
		coord.x = image->size.x - coord.x - 1;
	if (inv.y)
		coord.y = image->size.y - coord.y - 1;
	image_set_pixel(image, coord.x, coord.y, *(t_color *)color_data);
}

static void	read_tga_data(int ifd, t_tga_data tga, t_image *img)
{
	t_ivec2	a;
	t_ivec2	inverse;
	t_uchar	color[4];

	inverse.x = tga.image.descriptor >> 4 & 1;
	inverse.y = tga.image.descriptor >> 5 & 1;
	a = ivec2_zero();
	while (a.y < tga.image.height)
	{
		a.x = 0;
		while (a.x < tga.image.width)
		{
			if (read(ifd, color, 4) != 4)
				error_exit("Error while reading TGA image data");
			set_pixel(img, a, inverse, color);
			a.x++;
		}
		a.y++;
	}
}

// This function is still absolutely awful
static void	read_rl_tga_data(int ifd, t_tga_data tga, t_image *img,
				t_ivec2	inverse)
{
	t_ivec2	a;
	t_uchar	i;
	t_uchar	color[4];
	t_bool	raw;

	i = 1;
	raw = FALSE;
	a = ivec2_new(-1, -1);
	while (++a.y < tga.image.height)
	{
		a.x = -1;
		while (++a.x < tga.image.width)
		{
			if (--i == 0)
			{
				((void) !read(ifd, &i, 1), raw = i <= 127);
				i += 1 - 128 * !raw;
				(void)(!raw && read(ifd, color, 4) != 4);
			}
			(void)(raw && read(ifd, color, 4));
			set_pixel(img, a, inverse, color);
		}
	}
}

static t_tga_data	parse_header(int ifd)
{
	t_tga_data	tga;
	t_bool		success;
	char		dummy[5];

	success = read(ifd, &tga.idlen, 1) == 1;
	success *= read(ifd, &tga.colormap_type, 1) == 1;
	success *= read(ifd, &tga.image_data_type, 1) == 1;
	success *= read(ifd, dummy, 5) == 5;
	success *= read(ifd, &tga.image, 10) == 10;
	if (!success)
		error_exit("TGA header reading error");
	if (tga.colormap_type != 0 || (tga.image_data_type != 2
			&& tga.image_data_type != 10))
		error_exit("TGA loader only supports"
			" uncompressed or run-length encoded truecolor images"
			"  without colormap");
	return (tga);
}

t_image	*image_from_tga(t_wctx wctx, const char *path)
{
	t_string	*path_str;
	int			ifd;
	t_tga_data	tga;
	t_image		*img;
	char		dummy[256];

	path_str = string_from_cstr(path);
	if (!try_open_file(path_str, O_RDONLY, &ifd))
		return (string_delete(path_str), nil());
	string_delete(path_str);
	tga = parse_header(ifd);
	img = image_new(wctx, ivec2_new(tga.image.width, tga.image.height));
	if (tga.idlen > 0 && !(read(ifd, dummy, tga.idlen) == tga.idlen))
		error_exit("TGA error when skipping unused data");
	if (tga.image_data_type == 2)
		read_tga_data(ifd, tga, img);
	else if (tga.image_data_type == 10)
		read_rl_tga_data(ifd, tga, img, ivec2_new(
				tga.image.descriptor >> 4 & 1,
				tga.image.descriptor >> 5 & 1));
	close(ifd);
	return (img);
}
