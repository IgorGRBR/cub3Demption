/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   refresh.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ihhrabar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/08 16:00:24 by ihhrabar          #+#    #+#             */
/*   Updated: 2024/06/08 16:00:25 by ihhrabar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "ydefines.h"
#include "ylist.h"
#include "ymap.h"

t_list_iter	tslist_iter(t_tslist *ts)
{
	return (list_iter(ts->data));
}

static t_bool	rem_filter(void *v, t_map *to_remove)
{
	return (map_has(to_remove, v));
}

void	tslist_refresh(t_tslist *ts)
{
	t_list_iter	it;

	list_cfilter(ts->data, (t_cfilter_lfn)rem_filter, ts->rem);
	map_deinit(ts->rem);
	map_init(ts->rem, YNULL, YNULL);
	it = list_iter(ts->ins);
	while (list_iter_next(&it))
		list_insert(ts->data, it.value);
	list_deinit(ts->ins);
	list_init(ts->ins);
}
