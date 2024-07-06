/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene_defaults.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ihhrabar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 17:20:41 by ihhrabar          #+#    #+#             */
/*   Updated: 2024/05/21 17:20:42 by ihhrabar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "ymap.h"
#include "ystring.h"
#include "ytypes.h"

static t_list	*spl(const char *cstr)
{
	t_string	*str;
	t_list		*list;

	str = string_from_cstr(cstr);
	list = string_split(str, ' ');
	string_delete(str);
	return (list);
}

// Dont tell anyone
static const char	*g_oof
	= "cub 1 wall _ _ _ _";
	// = "cub 1 wall default_east default_south default_west default_north";

t_map	*_scene_default_binds(void)
{
	t_map	*binds;

	binds = map_new((t_hash_func)string_hash, (t_equals_func)string_equal);
	map_insert(binds, string_from_cstr("0"),
		spl("cub 0 space default_wall default_wall"));
	map_insert(binds, string_from_cstr("1"), spl(
			g_oof));
	map_insert(binds, string_from_cstr("E"), spl(
			"cub E space default_wall default_wall object player 0"));
	map_insert(binds, string_from_cstr("S"), spl(
			"cub S space default_wall default_wall object player 90"));
	map_insert(binds, string_from_cstr("W"), spl(
			"cub W space default_wall default_wall object player 180"));
	map_insert(binds, string_from_cstr("N"), spl(
			"cub N space default_wall default_wall object player 270"));
	return (binds);
}
