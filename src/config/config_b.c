/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config_b.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ihhrabar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/09 16:27:47 by ihhrabar          #+#    #+#             */
/*   Updated: 2024/05/09 16:27:53 by ihhrabar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "cub3d_core.h"
#include "ycstr.h"
#include "ylist.h"
#include "ytypes.h"
#include "yvec2.h"

t_bool	configure_window(t_list_iter *tokens, t_game_config *cfg)
{
	int			width;
	int			height;
	t_string	*str;

	list_iter_next(tokens);
	str = tokens->value;
	if (!cstr_try_atoi(str->cstr, &width))
		return (FALSE);
	list_iter_next(tokens);
	str = tokens->value;
	if (!cstr_try_atoi(str->cstr, &height))
		return (FALSE);
	cfg->window_size = ivec2_new(width, height);
	return (TRUE);
}

t_bool	configure_keybind(t_list_iter *tokens, t_game_config *cfg)
{
	t_string	*str;
	t_string	*bind_name;
	int			bind_value;
	t_game_key	key;

	list_iter_next(tokens);
	bind_name = tokens->value;
	list_iter_next(tokens);
	str = tokens->value;
	if (!cstr_try_atoi(str->cstr, &bind_value))
		return (FALSE);
	key = get_key_from_string(bind_name);
	if (key == GK_NONE)
		return (FALSE);
	cfg->keymap[key] = bind_value;
	return (TRUE);
}

t_bool	configure_rthread_count(t_list_iter *tokens, t_game_config *cfg)
{
	t_string	*str;
	int			count;

	list_iter_next(tokens);
	str = tokens->value;
	if (!cstr_try_atoi(str->cstr, &count))
		return (FALSE);
	cfg->rendering_thread_count = count;
	return (TRUE);
}

t_bool	configure_sensitivity(t_list_iter *tokens, t_game_config *cfg)
{
	t_string	*str;
	int			sens;

	list_iter_next(tokens);
	str = tokens->value;
	if (!cstr_try_atoi(str->cstr, &sens))
		return (FALSE);
	if (sens < 0)
		return (FALSE);
	cfg->sensitivity = (t_uint)sens;
	return (TRUE);
}

t_bool	configure_resolution_height(t_list_iter *tokens, t_game_config *cfg)
{
	t_string	*str;
	int			height;

	list_iter_next(tokens);
	str = tokens->value;
	if (!cstr_try_atoi(str->cstr, &height))
		return (FALSE);
	if (height < MIN_WIN_H)
		return (FALSE);
	cfg->resolution_height = (t_uint)height;
	return (TRUE);
}
