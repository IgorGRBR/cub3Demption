/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ihhrabar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/09 16:23:35 by ihhrabar          #+#    #+#             */
/*   Updated: 2024/05/09 16:23:37 by ihhrabar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <time.h>
#include "cub3d.h"

double	get_time(void)
{
	struct timespec	tm;
	double			res;

	clock_gettime(CLOCK_MONOTONIC, &tm);
	res = (double)tm.tv_sec;
	res += (double)tm.tv_nsec / 1000000000.0;
	return (res);
}
