/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d_core.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ihhrabar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/01 17:28:19 by ihhrabar          #+#    #+#             */
/*   Updated: 2023/12/01 17:28:20 by ihhrabar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_CORE_H
# define CUB3D_CORE_H
# include "ytypes.h"
# include <pthread.h>
# if BACKEND == MLX
// MLX includes
# elif BACKEND == SDL
// SDL includes
#  include <SDL2/SDL_scancode.h>
#  include <SDL2/SDL_ttf.h>
#  include <SDL2/SDL_video.h>
# endif

// This file only contains the types used in the codebase.

// Minimum width and height of a window
# define MIN_WIN_W 320
# define MIN_WIN_H 240
# define RAYCAST_DISTANCE_LIMIT 32.0
# define MMAP_RADIUS 8
# define PHYSICS_ITER_COUNT 4
# define UPDATE_RATE 60.0
# define MAX_UPDATES 4
# define RC_DOOR_COUNT 6
# define NPC_HURT_TIME 0.8
# define PLAYER_MAX_HEALTH 10
# define PLAYER_MAX_AMMO 18
# define KC_COUNT 4

// Some forward declarations
struct					s_game;
struct					s_image;
struct					s_texture;
struct					s_scene;
struct					s_player;
struct					s_actor;
struct					s_scene_prop;

# if BACKEND == MLX

// MiniLibX window context.
typedef struct s_mlx_context
{
	void				*mlx;
	void				*mlx_win;
	struct s_texture	*disp_texture;
}	t_mlx_context;

// A simple bind to make our life easier.
typedef t_mlx_context	t_wctx;

// The underlying raw key type
typedef int				t_raw_key;

// The raw texture type. Used only in the backend-specific code.
typedef struct s_texture
{
	void	*img_ptr;
	char	*pixel_data;
	int		bpp;
	int		bpp_div_8;
	int		line_length;
	t_bool	big_endian;
	t_ivec2	size;
}	t_texture;

# elif BACKEND == SDL

typedef int				(*t_event_func)(struct s_game *);

// SDL2 window context.
typedef struct s_sdl_context
{
	SDL_Window		*win;
	SDL_Surface		*sur;
	TTF_Font		*font;
	t_event_func	game_update;
	t_event_func	game_deinit;
}	t_wctx;

// The underlying raw key type.
typedef SDL_Scancode	t_raw_key;

// The raw texture type. Used only in the backend-specific code.
typedef SDL_Surface		t_texture;

# endif

// A data structure representing a list with deferred insertion and removal
// operations.
typedef struct s_transactional_list
{
	t_list	*ins;
	t_map	*rem;
	t_list	*data;
}	t_tslist;

// A pair of 2 int 2D vectors. Usually used for rendering 2D images.
typedef struct s_ivec2_pair
{
	t_ivec2	a;
	t_ivec2	b;
}	t_ivec2_pair;

// A pair of 2 double 2D vectors. Usually used for rendering 2D images.
typedef struct s_dvec2_pair
{
	t_dvec2	a;
	t_dvec2	b;
}	t_dvec2_pair;

// Transformation information. Used to specify transformations of images.
// Rotation is currently not working at all.
typedef struct s_transform2d
{
	t_dvec2	pos;
	double	rot;
	t_dvec2	sca;
}	t_transform2d;

// Struct representing a collection of assets. Contains maps of images and
// sounds (TODO).
typedef struct s_asset_collection
{
	t_map	*images;
	t_map	*sounds;
	t_map	*frames;
	t_map	*sprites;
}	t_asset_col;

// TODO: sounds?

// A simple struct representing an RGBA color.
typedef struct s_color
{
	t_uchar	b;
	t_uchar	g;
	t_uchar	r;
	t_uchar	a;
}	t_color;

// Struct representing internal information of TGA file.
// It's only used while loading .tga files.
typedef struct s_tga_data
{
	t_uchar	idlen;
	t_uchar	colormap_type;
	t_uchar	image_data_type;
	struct s_tga_image_header
	{
		short		origin_x;
		short		origin_y;
		t_ushort	width;
		t_ushort	height;
		t_uchar		pixel_depth;
		t_uchar		descriptor;
	}	image;
}	t_tga_data;

// Struct representing the 2D image. Contains the t_texture
// as a private field, along with the size of the image and its origin
// point.
typedef struct s_image
{
	t_texture	*_data;
	t_ivec2		size;
	t_ivec2		origin;
}	t_image;

// Type of a drawing operation to perform
typedef enum e_draw_type
{
	DRAW_NORMAL,
	DRAW_WITH_DEPTH,
	DRAW_WITH_EFFECT,
}	t_draw_type;

typedef struct s_fx_context
{
	t_image		*src;
	t_image		*tgt;
	t_image		*depth;
	float		src_depth;
	void		*extra_data;
}	t_fx_context;

typedef t_color			(*t_fx_func)(t_ivec2, t_ivec2, t_fx_context);

// Contains information that is used in drawing. Contains 2 pairs of ivec2,
// representing the source and the target image quads, allowing you to
// render a part of the source image into the part of the target image.
// It also contains a function pointer, that should optionally point to
// the pixel effect function and the context pointer, that should point to the
// data, used in the pixel effect function. Pixel effect functions are
// currently not implemented.
typedef struct s_draw_info
{
	t_draw_type		type;
	t_ivec2_pair	tgt_quad;
	t_ivec2_pair	src_quad;
	t_ivec2_pair	tgt_clip;
	union u_draw_info_context
	{
		struct s_depth_draw_info
		{
			float	src_depth;
			t_image	*buffer;
		}	depth;
		struct s_fx_draw_info
		{
			t_fx_func		func;
			t_fx_context	ctx;
		}	fx;
	}				ctx;
}	t_draw_info;

// Graphics context contains all of the necessary information that is used to
// perform final image draws and multithreaded rendering. Final image should be
// drawn to the `target` image. `threads` is a list of threads that is used
// in multithreaded rendering.
typedef struct s_gfx_context
{
	t_image			*target;
	t_image			*display;
	t_transform2d	transform;
	t_list			*threads;
	t_uint			thread_count;
	t_bool			running;
}	t_gfx_ctx;

// The drawing context. Used in multithreaded rendering. Currently only
// contains the `source` and `target` images, as well as their drawing
// info.
typedef struct s_drawing_context
{
	t_image			*target;
	t_image			*source;
	t_draw_info		info;
}	t_dctx;

struct					s_drawing_thread;
typedef void			(*t_dthread_func)(struct s_drawing_thread *);

typedef enum e_dthread_type
{
	DTHREAD_NONE,
	DTHREAD_IMAGE,
	DTHREAD_SCENE
}	t_dthread_type;

typedef union u_dthread_data
{
	t_dctx			drawing;
	struct s_scene	*scene;
}	t_dthread_data;

// A structure that cantains all of the necessary information for the
// drawing thread to operate. `id` is the index of the thread.
// `min_range` and `max_range` (should rename them to something like
// begin_range and end_range tbh) represent the range of the data
// the thread should operate on. `drawing_ctx` and `gfx` are drawing
// and graphics contexts that are passed into the thread upon its
// activation. `active` is the flag, representing the activity state of the
// thread. Other than the `drawing_mutex`, `drawing_cond` and `active`,
// all variables in this structure should not be mutated during the runtime
// of the thread and within the thread at all!
typedef struct s_drawing_thread
{
	t_uint			id;
	double			min_range;
	double			max_range;
	pthread_t		pthread;
	pthread_mutex_t	drawing_mutex;
	pthread_cond_t	drawing_cond;
	t_bool			active;
	t_gfx_ctx		*gfx;
	t_dthread_type	op_type;
	t_dthread_data	data;
}	t_drawing_thread;

typedef struct s_timing_info
{
	double	delta_time;
	double	draw_interp;
	double	accumulator;
}	t_timing_info;

// An enum representing the game key.
typedef enum e_game_key
{
	GK_NONE,
	GK_FORWARD,
	GK_BACKWARD,
	GK_STRAFE_LEFT,
	GK_STRAFE_RIGHT,
	GK_LOOK_LEFT,
	GK_LOOK_RIGHT,
	GK_SHOOT,
	GK_USE,
	GK_EXIT,
	GK_DEBUG,
	NUMBER_OF_GAME_KEYS
}	t_game_key;

// Struct contains the initial config of the game.
typedef struct s_game_config
{
	t_uint		resolution_height;
	t_ivec2		window_size;
	t_raw_key	keymap[NUMBER_OF_GAME_KEYS];
	t_uint		rendering_thread_count;
	t_uint		sensitivity;
	t_uint		fov;
	t_bool		mouselook;
}	t_game_config;

// Type of the node in the grid.
typedef enum e_grid_node_type
{
	GNT_EMPTY,
	GNT_SOLID,
	GNT_FLOOR,
	GNT_EXIT,
	GNT_DOOR,
}	t_grid_node_type;

// Cardinal direction
# define DIR_COUNT 4

typedef enum e_direction
{
	DIR_EAST,
	DIR_SOUTH,
	DIR_WEST,
	DIR_NORTH
}	t_direction;

// Frame is a collection of 4 images that represents a world object
// visualization in 4 directions.
typedef t_image			*t_frame[4];

typedef struct s_sprite_player
{
	t_list	*sprite;
	double	frame;
	double	speed;
}	t_sprite_player;

// Data of the grid node that is a wall. Contains 4 textures for each cardinal
// direction, as well as the default color (TODO: remove the color, it's not
// necessary).
typedef struct s_grid_wall_node
{
	t_color	color;
	t_image	*texture[DIR_COUNT];
}	t_grid_wall_node;

// Data of the grid node that is a walkable space. Contains top/bottom colors,
// top/bottom textures and a list of visual entites, that are located within
// the node.
typedef struct s_grid_floor_node
{
	t_color	bottom_color;
	t_color	top_color;
	t_image	*bottom_texture;
	t_image	*top_texture;
}	t_grid_floor_node;

typedef enum e_door_type
{
	DOOR_NONE,
	DOOR_GREEN,
	DOOR_ORANGE,
	DOOR_PURPLE,
	DOOR_CYAN
}	t_door_type;

// Data of the grid node that is a door. Door nodes contain simillar data to
// walkable soace nodes, as well as door textures in opened/closed states,
// opened/closed state itself, and if the door is placed vertically on the map.
// Door nodes also contain the type of the door. Different door types require
// different keycards to interact with.
typedef struct s_grid_door_node
{
	t_color		bottom_color;
	t_color		top_color;
	t_image		*bottom_texture;
	t_image		*top_texture;
	t_bool		open;
	t_bool		vertical;
	t_door_type	type;
	t_image		*open_texture;
	t_image		*closed_texture;
}	t_grid_door_node;

// The grid node itself. Contains its type and the relevant data of that node.
typedef struct s_grid_node
{
	t_grid_node_type	type;
	t_list				*vis_ents;
	t_list				*phys_bodies;
	union u_grid_node_data
	{
		struct s_grid_wall_node		wall;
		struct s_grid_floor_node	floor;
		struct s_grid_door_node		door;
	}					data;
}	t_grid_node;

// Struttura del Nodo
typedef struct s_pathfind_node
{
	t_ivec2					pos;
	int						cost;
	int						dist;
	int						sum;
	struct s_pathfind_node	*parent;
}	t_pf_node;

// Visual entity is a 2D billboard sprite that exits in 3D space and represents
// some sort of an object. It contains the reference to the image, a transform
// of the image, `gpos` position in a grid, the `goff` offset within the grid
// node and a visibility flag.
// Note: pos field of the transform is used as an offset.
typedef struct s_visual_entity
{
	t_transform2d	tf;
	t_dvec3			pos;
	t_dvec3			_old_pos;
	t_bool			visible;
	t_dvec2			dir;
	t_frame			frame;
	double			radius;
}	t_vis_ent;

struct					s_phys_body;
typedef t_dvec2			(*t_pb_collision_func)(void *,
	struct s_phys_body *, t_dvec2);

typedef void			(*t_physics_cb_func)(void *, struct s_scene *);

typedef enum e_pb_parent_type
{
	PBP_NONE = 0,
	PBP_PLAYER = 1,
	PBP_ITEM = 1 << 1,
	PBP_PROP = 1 << 2,
	PBP_NPC = 1 << 3,
	PBP_NODE = 1 << 4,
	PBP_DOOR = 1 << 5,
}	t_pb_parent_type;

// Represents a physical body
typedef struct s_phys_body
{
	t_dvec2				half_radius;
	t_dvec2				pos;
	t_dvec2				vel;
	t_dvec2				_cvel;
	t_pb_collision_func	col_cb;
	t_physics_cb_func	phy_cb;
	void				*parent;
	t_pb_parent_type	par_type;
}	t_phys_body;

typedef struct s_phys_raycast_config
{
	double				max_len;
	t_pb_parent_type	type;
}	t_pb_rc_config;

// An input state. It's used to update actors state with some set of inputs.
typedef struct s_inputs
{
	t_dvec2	move;
	double	look;
	t_bool	attack;
	t_bool	use;
}	t_inputs;

// An actor is an animated world entity with a physical body, usually able to
// move with some set of provided inputs. Contains its position, looking
// direction, physics body and other various states and stats.
typedef struct s_actor
{
	t_dvec3		pos;
	t_dvec2		dir;
	t_phys_body	pbody;
	double		run_speed;
	int			health;
	t_bool		slide;
}	t_actor;

struct					s_thinker;
struct					s_thinking_result;
typedef struct s_thunk	(*t_think_func)(void *, struct s_scene*,
	double);

typedef struct s_thunk
{
	t_inputs		inputs;
	t_think_func	next;
}	t_thunk;

struct					s_npc_thinker_data;
typedef struct s_thinker
{
	t_think_func				current_state;
	struct s_npc_thinker_data	*data;
}	t_thinker;

typedef enum e_enemy_strat_type
{
	STRAT_RANDOM_WALK
}	t_enemy_strat_type;

typedef enum e_npc_type
{
	NPC_GUNNER,
}	t_npc_type;

typedef enum e_npc_state
{
	NSTATE_WALK,
	NSTATE_CHASE_TARGET,
	NSTATE_ATTACK,
	NSTATE_HIT,
}	t_npc_state;

typedef struct s_npc_spawn_data
{
	t_dvec2				pos;
	t_dvec2				dir;
	t_npc_type			type;
	t_enemy_strat_type	strat;
}	t_npc_spawn_data;

struct					s_npc;
typedef struct s_npc_thinker_data
{
	struct s_npc	*me;
	t_actor			*target;
	t_dvec2			move_target;
	t_dvec2			spawn_pos;
	double			wander_distance;
	t_list			*path;
	double			timer;
	double			detection_range;
	double			attack_range;
}	t_npc_thinker_data;

typedef struct s_npc
{
	t_inputs			inputs;
	t_actor				*actor;
	t_vis_ent			*vent;
	t_thinker			thinker;
	t_npc_thinker_data	think_data;
	t_sprite_player		splayer;
}	t_npc;

// The main player of the game. Contains its own actor, visual entity, set of
// inputs, stats and tracks the state of keys with the `keys` array.
typedef struct s_player
{
	t_inputs	inputs;
	t_bool		keys[NUMBER_OF_GAME_KEYS];
	t_actor		*actor;
	t_vis_ent	*vent;
	double		look_sensitivity;
	double		look_vector;
	t_bool		exit_status;
	t_uint		ammo;
	t_bool		keycards[KC_COUNT];
}	t_player;

typedef struct s_scene_prop
{
	t_dvec3		pos;
	t_bool		solid;
	t_phys_body	pbody;
	t_dvec2		dir;
	t_vis_ent	*vent;
}	t_prop;

typedef t_bool			(*t_pickup_func)(t_player *, t_uint tag);

typedef struct s_scene_item
{
	t_pickup_func	pfunc;
	t_phys_body		pbody;
	t_vis_ent		*vent;
	t_bool			collected;
	t_uint			tag;
}	t_item;

typedef enum e_particle_type
{
	PRT_BULLET_IMPACT,
	PRT_SMOKE,
	PRT_DEBRIS,
	PRT_BLOOD,
	PRT_EXPLOSION,
	PRT_CUSTOM,
}	t_particle_type;

typedef struct s_particle
{
	t_prop			*prop;
	t_particle_type	type;
	double			lifetime;
	double			timer;
	t_list			*sprite;
}	t_particle;

typedef struct s_camera
{
	double	plane_hwidth;
	double	fov;
	double	vfov;
	t_dvec2	dir;
	t_dvec3	pos;
}	t_camera;

typedef struct s_raycast_result
{
	t_phys_body	hit_body;
	t_bool		hit;
	t_dvec3		hit_pos;
	t_dvec3		ray_dir;
	t_dvec3		ray_pos;
}	t_rc_res;

typedef struct s_game_asset_collection
{
	struct s_sprite_asset_collection
	{
		struct s_bandit_sprite_asset_collection
		{
			t_list	*idle;
			t_list	*walk;
			t_list	*aim;
			t_list	*hurt;
			t_list	*walkg;
			t_list	*fire;
		}	bandit;
	}	sprites;
	struct s_particle_asset_collection
	{
		t_list	*bullet_impact;
	}	particles;
	struct s_hud_asset_collection
	{
		t_list	*pistol;
		t_list	*pistol_fire;
	}	hud;
}	t_gac;

// A struct representing the playable environment that is a scene. It contains
// everything necessary for the game to function - collections of objects,
// player object, the grid, where everything resides, default textures, etc...
typedef struct s_scene
{
	t_gac			gac;
	t_grid_node		*grid;
	t_ivec2			grid_size;
	t_tslist		*actors;
	t_player		player;
	int				skybox;
	t_image			*minimap;
	t_camera		camera;
	t_list			*path;
	t_tslist		*props;
	t_tslist		*phys_bodies;
	t_tslist		*npcs;
	t_list			*particles;
	t_list			*pickups;
	struct s_default_assets
	{
		t_color	top;
		t_color	bottom;
		t_image	*wall_tex[DIR_COUNT];
	}	default_assets;
	struct s_hud
	{
		t_image	*arm;
		t_list	*cur_spr;
		t_list	*aim_spr;
		t_list	*fire_spr;
		double	action_timer;
		double	action_time;
		t_image	*hp;
		t_image	*ammo;
	}				hud;
	double			res_scale_factor;
	t_image			*rc_target;
	t_image			*rc_depth;
	t_image			*final_target;
	t_timing_info	timing;
}	t_scene;

// The high-level game structure. Contains all of the necessary data needed
// to run the game, such as game config, keymap, various contexts, collections
// and the current scene.
typedef struct s_game
{
	t_wctx			window_ctx;
	t_game_config	config;
	t_map			*keymap;
	t_gfx_ctx		*gfx;
	double			elapsed_time;
	t_bool			running;
	t_asset_col		*assets;
	t_uint			rthread_count;
	t_scene			*scene;
	t_ivec2			mouse_pos;
}	t_game;

#endif
