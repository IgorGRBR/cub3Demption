/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config_c.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ihhrabar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 18:46:33 by ihhrabar          #+#    #+#             */
/*   Updated: 2024/06/04 18:46:34 by ihhrabar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "ycstr.h"
#include "ylist.h"
#include "ytypes.h"

t_bool	configure_field_of_view(t_list_iter *tokens,
			t_game_config *cfg)
{
	t_string	*str;
	int			fov;

	list_iter_next(tokens);
	str = tokens->value;
	if (!cstr_try_atoi(str->cstr, &fov))
		return (FALSE);
	if (fov < 0 || fov >= 180)
		return (FALSE);
	cfg->fov = (t_uint)fov;
	return (TRUE);
}

t_bool	configure_mouselook(t_list_iter *tokens, t_game_config *cfg)
{
	t_string	*str;
	int			ml;

	list_iter_next(tokens);
	str = tokens->value;
	if (!cstr_try_atoi(str->cstr, &ml))
		return (FALSE);
	if (ml != 0 && ml != 1)
		return (FALSE);
	cfg->mouselook = (t_bool)ml;
	return (TRUE);
}
