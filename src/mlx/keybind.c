/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keybind.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ihhrabar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/09 16:29:09 by ihhrabar          #+#    #+#             */
/*   Updated: 2024/05/09 16:29:14 by ihhrabar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#if BACKEND == MLX
# include "cub3d_generic.h"
# include "ytypes.h"

t_uint	raw_key_hash(t_raw_key *i)
{
	return ((t_raw_key) * i);
}

t_bool	raw_key_eq(t_raw_key *a, t_raw_key *b)
{
	return (*a == *b);
}

#endif
