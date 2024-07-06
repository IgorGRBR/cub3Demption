/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ihhrabar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/08 16:00:15 by ihhrabar          #+#    #+#             */
/*   Updated: 2024/06/08 16:00:16 by ihhrabar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "ydefines.h"
#include "ylist.h"
#include "ymap.h"
#include <stdlib.h>

t_tslist	*tslist_new(void)
{
	t_tslist	*ts;

	ts = (t_tslist *)malloc(sizeof (t_tslist));
	ts->ins = list_new();
	ts->rem = map_new(YNULL, YNULL);
	ts->data = list_new();
	return (ts);
}

t_tslist	*tslist_from_list(t_list *list)
{
	t_tslist	*ts;
	t_list_iter	it;

	ts = tslist_new();
	it = list_iter(list);
	while (list_iter_next(&it))
		list_insert(ts->data, it.value);
	return (ts);
}

void	tslist_delete(t_tslist *ts)
{
	list_delete(ts->ins);
	map_delete(ts->rem);
	list_delete(ts->data);
	free(ts);
}

void	tslist_insert(t_tslist *ts, void *item)
{
	list_insert(ts->ins, item);
}

void	tslist_remove(t_tslist *ts, void *item)
{
	map_insert(ts->rem, item, item);
}
