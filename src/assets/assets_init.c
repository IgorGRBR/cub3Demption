/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   assets_init.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ihhrabar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/09 16:25:08 by ihhrabar          #+#    #+#             */
/*   Updated: 2024/05/09 16:25:13 by ihhrabar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "cub3d_core.h"
#include "ycstr.h"
#include "ydefines.h"
#include "ylist.h"
#include "ymap.h"
#include "yprint.h"
#include "ytypes.h"
#include <stdlib.h>

t_asset_col	*assets_new(void)
{
	t_asset_col	*assets;

	assets = malloc(sizeof (t_asset_col));
	assets->sounds = YNULL;
	assets->images = map_new(
			(t_hash_func)cstr_hash, (t_equals_func)cstr_equal);
	assets->frames = map_new(
			(t_hash_func)cstr_hash, (t_equals_func)cstr_equal);
	assets->sprites = map_new(
			(t_hash_func)cstr_hash, (t_equals_func)cstr_equal);
	return (assets);
}

void	assets_delete(t_asset_col *col, t_wctx wctx)
{
	t_map_iter	it;

	yprintf("deleting asset collection\n");
	it = map_iter(col->sprites);
	while (map_iter_next(&it))
		(free(it.key), list_delete(it.value));
	map_delete(col->sprites);
	it = map_iter(col->frames);
	while (map_iter_next(&it))
		(free(it.key), frame_delete(it.value));
	map_delete(col->frames);
	it = map_iter(col->images);
	while (map_iter_next(&it))
		(free(it.key), image_delete(it.value, wctx));
	map_delete(col->images);
	free(col);
}
