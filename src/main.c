/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ihhrabar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/01 17:06:19 by ihhrabar          #+#    #+#             */
/*   Updated: 2023/12/01 17:06:20 by ihhrabar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "cub3d_core.h"
#include "cub3d_generic.h"
#include "yprint.h"
#include "ystring.h"
#include "ytypes.h"
#include "yvec2.h"

#if BACKEND == MLX
# ifdef LINUX

static const char	*g_cfg_path = "config_mlx_lx.txt";

# endif

# ifdef OSX

static const char	*g_cfg_path = "config_mlx_osx.txt";

# endif
#elif BACKEND == SDL

static const char	*g_cfg_path = "config_sdl.txt";

#endif

static const char	*g_assets_path = "assets.txt";

static t_game_config	default_config(void)
{
	t_game_config	gcfg;
	t_uint			i;

	gcfg.resolution_height = 480;
	gcfg.window_size = ivec2_new(640, 480);
	i = 0;
	while (i < NUMBER_OF_GAME_KEYS)
	{
		gcfg.keymap[i] = 0;
		i++;
	}
	gcfg.rendering_thread_count = 2;
	gcfg.sensitivity = 100;
	gcfg.fov = 75;
	gcfg.mouselook = FALSE;
	return (gcfg);
}

// A very important function, apparently...
static t_bool	check_if_file_name_is_good(const char *fn)
{
	t_string	str;
	int			i;
	t_bool		result;

	string_init_from_cstr(&str, fn);
	i = string_find_cstr(&str, ".cub");
	result = i == (int)str.size - 4;
	string_deinit(&str);
	return (result);
}

int	main(int argc, char *argv[])
{
	t_game			game;
	t_game_config	gcfg;

	gcfg = default_config();
	if (argc < 2)
		return (yprintf("no map specified\n"), 1);
	if (!check_if_file_name_is_good(argv[1]))
		return (yprintf("file doesn't end with .cub extension\n"));
	if (!try_load_config(g_cfg_path, &gcfg))
		return (yprintf("failed to load config\n"), 1);
	game = game_new(gcfg);
	exit_data(&game, TRUE);
	if (!init_backend(&game))
		return (yprintf("failed to initalize backend\n"), 1);
	yprintf("Loading assets...\n");
	assets_load_from_file(game.assets, game.window_ctx, g_assets_path);
	setup_hooks(game.window_ctx, &game);
	game.scene = scene_from_cub_file(&game, argv[1]);
	if (!game.scene)
		error_exit("could not load a scene for some reason\n");
	update_loop(&game);
	return (0);
}
