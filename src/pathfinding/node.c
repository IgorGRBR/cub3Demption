/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   node.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ihhrabar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/20 18:21:59 by ihhrabar          #+#    #+#             */
/*   Updated: 2024/06/20 18:22:00 by ihhrabar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "ydefines.h"
#include "ylist.h"
#include "yvec2.h"
#include <stdlib.h>

// Funzione per creare un nuovo nodo
t_pf_node	*create_pf_node(t_ivec2 pos, t_pf_node *parent)
{
	t_pf_node	*node;

	node = (t_pf_node *)malloc(sizeof(t_pf_node));
	node->pos = pos;
	node->cost = 0;
	node->dist = 0;
	node->sum = 0;
	node->parent = parent;
	return (node);
}

// Node position equality function
t_bool	pf_node_equals_vec(t_pf_node *a, t_ivec2 *b)
{
	return (ivec2_eq(a->pos, *b));
}

// Verifica se un nodo è nella lista chiusa
t_bool	pf_node_in_list(t_list *list, t_ivec2 pos)
{
	return (list_find_eq(list, (t_equals_func)pf_node_equals_vec, &pos) >= 0);
}

// Verifica se un nodo è nella lista aperta
t_pf_node	*find_pf_node_in_list(t_list *list, t_ivec2 pos)
{
	int	i;

	i = list_find_eq(list, (t_equals_func)pf_node_equals_vec, &pos);
	if (i < 0)
		return (YNULL);
	return (list_get(list, i));
}
