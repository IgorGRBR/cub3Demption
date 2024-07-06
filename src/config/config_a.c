/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config_a.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ihhrabar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/09 16:27:36 by ihhrabar          #+#    #+#             */
/*   Updated: 2024/05/09 16:27:43 by ihhrabar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "cub3d_core.h"
#include "yio.h"
#include "ylist.h"
#include "ystring.h"
#include "ytypes.h"
#include <fcntl.h>
#include <unistd.h>

t_bool	try_load_config(const char *cfg_path, t_game_config *cfg)
{
	int			cfg_fd;
	t_string	*cfg_str;
	t_list		*tokens;
	t_bool		result;

	if (!cfg || !cfg_path || !try_open_file_cstr(cfg_path, O_RDONLY, &cfg_fd))
		return (FALSE);
	cfg_str = read_file_to_string(cfg_fd);
	close(cfg_fd);
	if (!cfg_str)
		return (FALSE);
	tokens = tokenize_str(cfg_str);
	string_delete(cfg_str);
	result = parse_config(tokens, cfg);
	(list_apply(tokens, (t_apply_lfn)string_delete), list_delete(tokens));
	return (result);
}

t_bool	handle_conf_unit(t_list_iter *it, t_game_config *cfg)
{
	if (string_equal_cstr(it->value, "window_size"))
		return (configure_window(it, cfg));
	else if (string_equal_cstr(it->value, "keybind"))
		return (configure_keybind(it, cfg));
	else if (string_equal_cstr(it->value, "rendering_threads"))
		return (configure_rthread_count(it, cfg));
	else if (string_equal_cstr(it->value, "sensitivity"))
		return (configure_sensitivity(it, cfg));
	else if (string_equal_cstr(it->value, "resolution_height"))
		return (configure_resolution_height(it, cfg));
	else if (string_equal_cstr(it->value, "field_of_view"))
		return (configure_field_of_view(it, cfg));
	else if (string_equal_cstr(it->value, "mouselook"))
		return (configure_mouselook(it, cfg));
	return (FALSE);
}

t_bool	parse_config(t_list *cfg_tokens, t_game_config *cfg)
{
	t_list_iter	it;

	it = list_iter(cfg_tokens);
	while (list_iter_next(&it))
	{
		if (!handle_conf_unit(&it, cfg))
			return (FALSE);
	}
	return (TRUE);
}
