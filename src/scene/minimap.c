/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ihhrabar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/08 16:02:07 by ihhrabar          #+#    #+#             */
/*   Updated: 2024/06/08 16:02:08 by ihhrabar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "cub3d_core.h"
#include "ylist.h"
#include "ytypes.h"
#include "yvec2.h"

static const int	g_rad = MMAP_RADIUS;

static void	draw_pixel(t_image *minimap, t_ivec2 rc,
				t_grid_node *gn)
{
	if (ivec2_eq(rc, ivec2_zero()))
		image_set_pixel(minimap, rc.x + g_rad, rc.y + g_rad, g_white);
	else if (!gn || gn->type == GNT_EMPTY)
		image_set_pixel(minimap, rc.x + g_rad, rc.y + g_rad, g_black);
	else if (gn->type == GNT_SOLID)
		image_set_pixel(minimap, rc.x + g_rad, rc.y + g_rad, g_blue);
	else if (gn->type == GNT_FLOOR)
		image_set_pixel(minimap, rc.x + g_rad, rc.y + g_rad, g_gray);
	else if (gn->type == GNT_EXIT)
		image_set_pixel(minimap, rc.x + g_rad, rc.y + g_rad, g_green);
}

static void	draw_path(t_image *minimap, t_list *path, t_ivec2 ppos)
{
	t_list_iter	it;
	t_ivec2		*v;
	t_ivec2		p;

	it = list_iter(path);
	while (list_iter_next(&it))
	{
		v = it.value;
		p = ivec2_sub(*v, ppos);
		if (p.x <= MMAP_RADIUS && p.x > -MMAP_RADIUS
			&& p.y <= MMAP_RADIUS && p.y > -MMAP_RADIUS)
			image_set_pixel(minimap, p.x + MMAP_RADIUS, p.y + MMAP_RADIUS,
				color_new(128, 192, 192, 255));
	}
}

void	minimap_update(t_image *minimap, t_scene *scene)
{
	t_ivec2				ppos;
	t_ivec2				c;
	t_grid_node			*gn;

	image_clear(minimap, g_black);
	ppos = dvec2_floor(dvec2_new(scene->player.actor->pos.x,
				scene->player.actor->pos.z));
	c = ivec2_new(-MMAP_RADIUS, -MMAP_RADIUS);
	while (c.x <= MMAP_RADIUS)
	{
		c.y = -MMAP_RADIUS;
		while (c.y <= MMAP_RADIUS)
		{
			gn = scene_node_at(scene, ppos.x + c.x, ppos.y + c.y);
			draw_pixel(minimap, c, gn);
			c.y++;
		}
		c.x++;
	}
	if (scene->path)
		draw_path(minimap, scene->path, ppos);
}

void	minimap_draw(t_image *minimap, t_image *target)
{
	t_transform2d	tf;

	tf.pos = dvec2_new(32.0, 32.0);
	tf.rot = 0;
	tf.sca = dvec2_scale(dvec2_new(4.0, 4.0),
			(double)target->size.y / (double)MIN_WIN_H);
	image_draw_into_tf(target, minimap, tf);
}
