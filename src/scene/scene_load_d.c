/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene_load_d.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ihhrabar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 16:45:32 by ihhrabar          #+#    #+#             */
/*   Updated: 2024/05/21 16:45:34 by ihhrabar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "cub3d_core.h"
#include "ytypes.h"
#include "ylist.h"
#include "ystring.h"
#include "yvec2.h"
#include "ycstr.h"
#include "yvec3.h"
#include <math.h>

static void	handle_item(t_scene_init_args args, t_list_iter *it,
					t_ivec2 coords)
{
	t_item	*item;
	t_bool	status;

	item = pickup_new(args.scene,
			dvec2_new((double)coords.x + 0.5, (double)coords.y + 0.5));
	list_iter_next(it);
	status = FALSE;
	if (string_equal_cstr(it->value, "health"))
		status = setup_healthpack(item, args.game);
	else if (string_equal_cstr(it->value, "ammo"))
		status = setup_ammopack(item, args.game);
	else if (string_equal_cstr(it->value, "keycard"))
	{
		list_iter_next(it);
		status = setup_keycard(item, args.game, it->value);
	}
	if (!status)
		error_exit("unknown item type/misconfigured item definition");
	list_insert(args.scene->pickups, item);
}

static void	handle_player(t_scene_init_args args, t_list_iter *it,
				t_ivec2 coords)
{
	int			angle;
	double		rad;

	list_iter_next(it);
	if (!cstr_try_atoi(((t_string *)it->value)->cstr, &angle))
		error_exit("player first parameter must be an angle");
	scene_load_player(args.scene, args.game, coords);
	rad = g_pi / 180.0 * (double)angle;
	args.scene->camera.dir = dvec2_new(cos(rad), sin(rad));
}

// prop = prop_new(args.scene, dvec3_new(coords.x + 0.5, 0.0, coords.y + 0.5),
static void	handle_prop(t_scene_init_args args, t_list_iter *it,
				t_ivec2 coords)
{
	t_prop		*prop;
	int			angle;
	double		rad;
	t_string	*type;

	list_iter_next(it);
	if (!cstr_try_atoi(((t_string *)it->value)->cstr, &angle))
		error_exit("prop first parameter must be an angle");
	rad = g_pi / 180.0 * (double)angle;
	prop = prop_new(args.scene, dvec3_new(coords.x, 0.0, coords.y),
			dvec2_new(cos(rad), sin(rad)));
	list_iter_next(it);
	type = it->value;
	if (!scene_load_prop(args.game, prop, type))
		error_exit("unknown prop type");
	tslist_insert(args.scene->props, prop);
}

static void	handle_npc(t_scene_init_args args, t_list_iter *it,
				t_ivec2 coords)
{
	t_npc_spawn_data	sdata;
	int					angle;
	t_npc				*npc;

	sdata.pos = dvec2_new((double)coords.x + 0.5, (double)coords.y + 0.5);
	list_iter_next(it);
	sdata.type = get_npc_type(it->value);
	list_iter_next(it);
	if (!cstr_try_atoi(((t_string *)it->value)->cstr, &angle))
		error_exit("npc second parameter must be an angle");
	sdata.dir = dvec2_new(cos(g_pi / 180.0 * (double)angle),
			sin(g_pi / 180.0 * (double)angle));
	npc = npc_new(args.scene, actor_new(args.scene, dvec3_new(
					sdata.pos.x, 0.0, sdata.pos.y), sdata.dir));
	scene_load_npc(args.game, args.scene, npc, sdata);
	tslist_insert(args.scene->npcs, npc);
	list_iter_next(it);
	if (!cstr_try_atoi(((t_string *)it->value)->cstr, &angle))
		error_exit("npc third parameter must be wandering distance");
	npc->think_data.wander_distance = (double)angle;
}

t_bool	_handle_object(t_scene_init_args args, t_list_iter *b_it,
			t_ivec2 coords)
{
	t_bool	cont;

	cont = TRUE;
	while (cont && list_iter_next(b_it))
	{
		if (string_equal_cstr(b_it->value, "player"))
			handle_player(args, b_it, coords);
		else if (string_equal_cstr(b_it->value, "prop"))
			handle_prop(args, b_it, coords);
		else if (string_equal_cstr(b_it->value, "npc"))
			handle_npc(args, b_it, coords);
		else if (string_equal_cstr(b_it->value, "item"))
			handle_item(args, b_it, coords);
		else
			cont = FALSE;
	}
	return (list_iter_prev(b_it), TRUE);
}
