/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   collision.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ihhrabar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 19:21:39 by ihhrabar          #+#    #+#             */
/*   Updated: 2024/06/20 19:35:45 by ihhrabar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "ymath.h"
#include "yvec2.h"

static t_dvec2	resolve(t_dvec2 pre, t_ivec2 dir, t_dvec2 cur)
{
	if (cur.x > 0 && cur.y > 0)
	{
		if (pre.x > 0 && pre.y <= 0)
			return (dvec2_new(0.0, cur.y * (double)dir.y));
		else if (pre.x <= 0 && pre.y > 0)
			return (dvec2_new(cur.x * (double)dir.x, 0.0));
		else if (pre.x > 0 && pre.y > 0)
		{
			if (cur.x < cur.y)
				return (dvec2_new(cur.x * (double)dir.x, 0.0));
			else
				return (dvec2_new(0.0, cur.y * (double)dir.y));
		}
	}
	return (dvec2_zero());
}

t_dvec2	resolve_collision(t_phys_body *pba, t_phys_body *pbb, t_dvec2 vel)
{
	t_dvec2	pre;
	t_ivec2	dir;
	t_dvec2	cur;

	pre.x = pba->half_radius.x + pbb->half_radius.x
		- double_abs(pba->pos.x - pbb->pos.x);
	pre.y = pba->half_radius.y + pbb->half_radius.y
		- double_abs(pba->pos.y - pbb->pos.y);
	dir.x = double_sign(pba->pos.x + vel.x - pbb->pos.x);
	dir.y = double_sign(pba->pos.y + vel.y - pbb->pos.y);
	cur.x = pba->half_radius.x + pbb->half_radius.x
		- double_abs(pba->pos.x + vel.x - pbb->pos.x);
	cur.y = pba->half_radius.y + pbb->half_radius.y
		- double_abs(pba->pos.y + vel.y - pbb->pos.y);
	return (resolve(pre, dir, cur));
}
