/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ihhrabar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/24 10:29:30 by ihhrabar          #+#    #+#             */
/*   Updated: 2024/06/24 10:29:32 by ihhrabar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "cub3d_core.h"
#include "ylist.h"
#include "ydefines.h"
#include <stdlib.h>

void	npc_td_clear_path(t_npc_thinker_data *td)
{
	if (td->path)
	{
		list_apply(td->path, (t_apply_lfn)free);
		list_delete(td->path);
		td->path = YNULL;
	}
}
