/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keybind.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ihhrabar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/09 16:29:32 by ihhrabar          #+#    #+#             */
/*   Updated: 2024/05/09 16:29:37 by ihhrabar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#if BACKEND == SDL
# include "cub3d.h"
# include "ylib.h"
# include "ytypes.h"
# include <stdlib.h>

t_uint	raw_key_hash(t_raw_key *i)
{
	return ((t_raw_key)(*i));
}

t_bool	raw_key_eq(t_raw_key *a, t_raw_key *b)
{
	return (*a == *b);
}

#endif
