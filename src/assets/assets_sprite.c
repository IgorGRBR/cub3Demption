/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   assets_sprite.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ihhrabar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/20 19:34:38 by ihhrabar          #+#    #+#             */
/*   Updated: 2024/06/20 19:34:39 by ihhrabar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "ycstr.h"
#include "ylist.h"
#include "ymap.h"
#include <stdlib.h>

t_bool	_assets_load_sprite(t_list_iter *it, t_asset_col *col)
{
	t_string	*name;
	t_string	*fname;
	t_frame		*frame;
	t_list		*sprite;
	t_kv_pair	kv;

	(list_iter_next(it), name = it->value);
	sprite = list_new();
	(list_iter_next(it), fname = it->value);
	frame = assets_get_frame(col, fname->cstr);
	while (frame)
	{
		list_insert(sprite, frame);
		(list_iter_next(it), fname = it->value);
		if (!fname)
			break ;
		frame = assets_get_frame(col, fname->cstr);
	}
	(void)((it->value) && (list_iter_prev(it)));
	kv = map_get_pair(col->sprites, name->cstr);
	if (kv.key)
		(free(kv.key), list_delete(kv.value));
	map_set(col->sprites, cstr_dup(name->cstr), sprite);
	return (TRUE);
}

t_frame	*assets_get_frame(t_asset_col *col, const char *name)
{
	return (map_get(col->frames, (char *)name));
}

t_list	*assets_get_sprite(t_asset_col *col, const char *name)
{
	return (map_get(col->sprites, (char *)name));
}
