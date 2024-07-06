/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_a.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ihhrabar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/01 17:41:42 by ihhrabar          #+#    #+#             */
/*   Updated: 2023/12/01 17:41:43 by ihhrabar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "cub3d_core.h"
#include "ycstr.h"
#include "ydefines.h"
#include "yio.h"
#include "yvec2.h"
#include <stdlib.h>
#include <unistd.h>

void	error_exit(char *msg)
{
	char	*msg_nl;
	t_game	*game;

	msg_nl = cstr_join(msg, "\n");
	write_cstr_fd(STDERR_FILENO, "Error\n");
	write_cstr_fd(STDERR_FILENO, msg_nl);
	free(msg_nl);
	game = exit_data(YNULL, FALSE);
	if (game)
		game_deinit(game);
	exit(1);
}

t_bool	iaabb_test(t_ivec2 apos, t_ivec2 asize, t_ivec2 bpos, t_ivec2 bsize)
{
	if (apos.x <= bpos.x + bsize.x && apos.x + asize.x >= bpos.x
		&& apos.y <= bpos.y + bsize.y && apos.y + asize.y >= bpos.y)
		return (TRUE);
	return (FALSE);
}

t_bool	daabb_test(t_dvec2 apos, t_dvec2 asize, t_dvec2 bpos, t_dvec2 bsize)
{
	if (apos.x <= bpos.x + bsize.x && apos.x + asize.x >= bpos.x
		&& apos.y <= bpos.y + bsize.y && apos.y + asize.y >= bpos.y)
		return (TRUE);
	return (FALSE);
}

void	inputs_clear(t_inputs *inputs)
{
	inputs->move = dvec2_zero();
	inputs->attack = FALSE;
	inputs->use = FALSE;
}

t_ivec2_pair	full_quad(t_ivec2 size)
{
	t_ivec2_pair	p;

	p.a = ivec2_zero();
	p.b = size;
	return (p);
}
