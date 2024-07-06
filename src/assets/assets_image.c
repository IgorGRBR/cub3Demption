/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   assets_image.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ihhrabar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/09 16:24:54 by ihhrabar          #+#    #+#             */
/*   Updated: 2024/05/09 16:24:56 by ihhrabar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "cub3d_core.h"
#include "ycstr.h"
#include "ymap.h"
#include "ytypes.h"
#include <stdlib.h>

t_bool	assets_load_image(t_asset_col *col, t_wctx wctx,
			const char *chs[2], t_ivec2 og)
{
	t_image		*image;
	t_kv_pair	pair;

	image = image_from_tga(wctx, chs[1]);
	if (!image)
		return (FALSE);
	pair = map_get_pair(col->images, (char *)chs[0]);
	if (pair.key)
	{
		free(pair.key);
		image_delete(pair.value, wctx);
	}
	image->origin = og;
	map_set(col->images, cstr_dup(chs[0]), image);
	return (TRUE);
}

t_image	*assets_get_image(t_asset_col *col, const char *name)
{
	return (map_get(col->images, (char *)name));
}
