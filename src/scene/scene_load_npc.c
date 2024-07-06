/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene_load_npc.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ihhrabar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/20 19:36:24 by ihhrabar          #+#    #+#             */
/*   Updated: 2024/06/20 19:36:25 by ihhrabar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "cub3d_core.h"

void	scene_load_npc(t_game *game, t_scene *scene, t_npc *npc,
					t_npc_spawn_data sinfo)
{
	(void)game;
	if (sinfo.type == NPC_GUNNER)
		scene_load_gunner(npc, scene, game);
}
