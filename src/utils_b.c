/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_b.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ihhrabar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/09 16:24:14 by ihhrabar          #+#    #+#             */
/*   Updated: 2024/05/09 16:24:16 by ihhrabar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "ydefines.h"
#include "ystring.h"
#include "ytypes.h"
#include "yvec2.h"
#include <fcntl.h>
#include <math.h>
#include <unistd.h>

static char	nl_map(char c, t_uint i)
{
	(void)i;
	if (c == '\n')
		return (' ');
	return (c);
}

static t_string	*remove_comments_and_nls(t_string *text)
{
	int			s;
	int			e;
	t_string	*new_text;
	t_string	*temp;

	s = 0;
	e = string_find_char(text, '#');
	if (e == -1)
	{
		new_text = string_imap(text, nl_map);
		return (new_text);
	}
	new_text = string_substring(text, s, e);
	s = string_find_char_next(text, '\n', e);
	while (s > 0)
	{
		e = string_find_char_next(text, '#', s);
		(void)((e == -1) && (e = text->size));
		temp = string_substring(text, s, e);
		(string_append(new_text, temp), string_delete(temp));
		s = string_find_char_next(text, '\n', e);
	}
	text = new_text;
	new_text = string_imap(text, nl_map);
	return (string_delete(text), new_text);
}

t_list	*tokenize_str(t_string *text)
{
	t_list	*tokens;

	text = remove_comments_and_nls(text);
	tokens = string_split(text, ' ');
	return (string_delete(text), tokens);
}

t_draw_info	draw_info_new(t_draw_type type)
{
	t_draw_info	di;

	di.type = type;
	di.tgt_quad = full_quad(ivec2_zero());
	di.src_quad = full_quad(ivec2_zero());
	di.tgt_clip = full_quad(ivec2_zero());
	if (type == DRAW_WITH_DEPTH)
	{
		di.ctx.depth.src_depth = 0.0f;
		di.ctx.depth.buffer = YNULL;
	}
	else if (type == DRAW_WITH_EFFECT)
	{
		di.ctx.fx.func = YNULL;
		di.ctx.fx.ctx = (t_fx_context){YNULL, YNULL, YNULL, 0.0f, YNULL};
	}
	return (di);
}

t_draw_info	draw_info_from_tf2d(t_draw_type type, t_image *img,
				t_transform2d tf)
{
	t_draw_info		dinfo;
	t_ivec2_pair	corners;
	t_ivec2			pos;

	dinfo = draw_info_new(type);
	pos.x = trunc(tf.pos.x);
	pos.y = trunc(tf.pos.y);
	corners.a.x = pos.x + trunc(-img->origin.x * tf.sca.x);
	corners.a.y = pos.y + trunc(-img->origin.y * tf.sca.y);
	corners.b.x = pos.x + trunc((img->size.x - img->origin.x) * tf.sca.x);
	corners.b.y = pos.y + trunc((img->size.y - img->origin.y) * tf.sca.y);
	dinfo.tgt_quad = corners;
	dinfo.src_quad = full_quad(img->size);
	dinfo.tgt_clip = full_quad(img->size);
	return (dinfo);
}
