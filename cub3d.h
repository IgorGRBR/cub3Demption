/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ihhrabar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/01 17:06:01 by ihhrabar          #+#    #+#             */
/*   Updated: 2023/12/01 17:06:02 by ihhrabar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H
# include "ytypes.h"
# include "cub3d_core.h"

/// Constants

// The PI
static const double			g_pi = 3.14159265358979323846;

// Colors 
static const t_color		g_blank = {0, 0, 0, 0};
static const t_color		g_black = {0, 0, 0, 255};
static const t_color		g_gray = {128, 128, 128, 255};
static const t_color		g_white = {255, 255, 255, 255};
static const t_color		g_red = {0, 0, 255, 255};
static const t_color		g_green = {0, 255, 0, 255};
static const t_color		g_blue = {255, 0, 0, 255};
static const t_color		g_yellow = {0, 255, 255, 255};

// Transforms
static const t_transform2d	g_default2d = {
{0.0, 0.0},
	0.0,
{1.0, 1.0}
};

/// Transactional list

// Construct a transactional list.
t_tslist		*tslist_new(void);

// Construct transactional list from a noraml one.
t_tslist		*tslist_from_list(t_list *list);

// Delete transactional list.
void			tslist_delete(t_tslist *ts);

// Queue up element for the insertion into transactional list.
void			tslist_insert(t_tslist *ts, void *item);

// Queue up element for removal from transactional list.
void			tslist_remove(t_tslist *ts, void *item);

// Construct an iterator over transactional lists current state.
t_list_iter		tslist_iter(t_tslist *ts);

// Apply changes to transactional list.
void			tslist_refresh(t_tslist *ts);

/// Colors

// Construct a new color from specified RGBA parameters.
t_color			color_new(t_uchar r, t_uchar g, t_uchar b, t_uchar a);

// Converts color information into depth.
float			color_as_depth(t_color col);

// Converts depth information into color.
t_color			depth_as_color(float d);

/// Game config handling

// Attempts to load and parse game config file specified at `cfg_path` into
// `t_game_config` structure. Returns TRUE upon success, FALSE otherwise.
t_bool			try_load_config(const char *cfg_path, t_game_config *cfg);

// Parses list of tokens into game config structure, where `cfg_tokens` is
// a t_list of t_strings. Returns TRUE upon success, FALSE otherwise.
t_bool			parse_config(t_list *cfg_tokens, t_game_config *cfg);

// A helper function used during the configuration loading step to configure
// window resolution. Takes a token iterator (which is an iterator over a
// t_list of t_strings), collects window size out of it and writes it into
// the game config struct. Returns TRUE upon success, FALSE otherwise.
t_bool			configure_window(t_list_iter *tokens, t_game_config *cfg);

// A helper function used during the configuration loading step to configure
// keyboard controls (keybinds). Takes a token iterator, collects keybind
// out of it and writes it into the game config struct. Returns TRUE upon
// success, FALSE otherwise.
t_bool			configure_keybind(t_list_iter *tokens, t_game_config *cfg);

// A helper function used during the configuration loading step to configure
// the number of rendering threads. Takes a token iterator, collects number
// of threads out of it and writes it into the game config struct. Returns
// TRUE upon success, FALSE otherwise.
t_bool			configure_rthread_count(t_list_iter *tokens,
					t_game_config *cfg);

// A helper function used during the configuration loading step to configure
// players' turning sensitivity. Takes a token iterator, collects sensitivity
// value out of it and writes it into the game config struct. Returns TRUE upon
// success, FALSE otherwise.
t_bool			configure_sensitivity(t_list_iter *tokens, t_game_config *cfg);

// A helper function used during the configuration loading step to configure
// the rendering resolution height. Takes a token iterator, collects height
// value out of it and writes it into the game config struct. Returns TRUE upon
// success, FALSE otherwise. 
t_bool			configure_resolution_height(t_list_iter *tokens,
					t_game_config *cfg);

// A helper function used during the configuration loading step to configure
// player's field of view. Takes a token iterator, collects field of view angle
// value out of it and writes it into the game config struct. Returns TRUE upon
// success, FALSE otherwise. 
t_bool			configure_field_of_view(t_list_iter *tokens,
					t_game_config *cfg);

// A helper function used during the configuration loading step to toggle the
// ability to look with mouse. Takes a token iterator, collects boolean (0/1)
// value out of it and writes it into the game config struct. Returns TRUE upon
// success, FALSE otherwise. 
t_bool			configure_mouselook(t_list_iter *tokens, t_game_config *cfg);

/// Images and drawing

// Constructs a new image on the heap with white pixels. Requires window
// context and the size of the image.
t_image			*image_new(t_wctx wctx, t_ivec2 size);

// Constructs a new image on the heap with pixels of specified color. Requires
// window context and the size of the image.
t_image			*image_new_color(t_wctx wctx, t_ivec2 size, t_color color);

// Constructs a copy of the provided image.
t_image			*image_copy(t_wctx wctx, t_image *img);

// Deinitializes the image and frees up the memory. Requires the window context
// to be passed along.
void			image_delete(t_image *img, t_wctx wctx);

// Sets the pixel of an image at specified coordinates to the specified color.
// If coordinates are larger than the image, they are wrapped around.
void			image_set_pixel(t_image *img, int x, int y, t_color color);

// Returns the color of a pixel of an image at specified coordinates.
// If coordinates are larger than the image, they are wrapped around.
t_color			image_get_pixel(t_image *img, int x, int y);

// Unsafe variant of `image_set_pixel`. Does not perform coordinate wraps
// and bound checks.
void			image_set_pixel_unsafe(t_image *img, int x, int y,
					t_color color);

// Unsafe variant of `image_get_pixel`. Does not perform coordinate wraps
// and bound checks.
t_color			image_get_pixel_unsafe(t_image *img, int x, int y);

// Draws the `img` source image into the `target` image with applied
// `tf2d` transformations. Returns TRUE if at least one pixel was
// drawn, FALSE otherwise.
t_bool			image_draw_into_tf(t_image *target, t_image *img,
					t_transform2d tf2d);

// Draws the `img` source image into the `target` image with specified
// drawing information, such as source/target quads, pixel effect function and
// user-provided context. Returns TRUE if at least one pixel was
// drawn, FALSE otherwise.
t_bool			image_draw_into(t_image *target, t_image *img,
					t_draw_info dinfo);

// Draws the `img` source image into the `target` image with applied
// `tf2d` transformations by using multiple threads. Returns TRUE if at least
// one pixel was drawn, FALSE otherwise.
t_bool			image_draw_into_mt(t_gfx_ctx *gfx, t_image *target,
					t_image *src, t_transform2d tf2d);

// Draws the `img` source image into the `target` image at specified position.
// Returns TRUE if at least one pixel was drawn, FALSE otherwise.
t_bool			image_draw_into_pos(t_image *target, t_image *img,
					t_ivec2 pos);

// Clears the image with specified color.
void			image_clear(t_image *image, t_color color);

// Constructs an image out of TGA file at the specified path.
t_image			*image_from_tga(t_wctx wctx, const char *path);

// Draws the image to the specified graphics context.
// Returns TRUE if at least one pixel was drawn, FALSE otherwise.
t_bool			image_draw(t_gfx_ctx *gfx, t_image *img, t_transform2d tf2d);

// Draws the image to the specified graphics context at the specified position.
// Returns TRUE if at least one pixel was drawn, FALSE otherwise.
t_bool			image_draw_pos(t_gfx_ctx *gfx, t_image *img, t_ivec2 pos);

t_bool			_image_draw_normal_into(t_image *target, t_image *source,
					t_draw_info dinfo);

t_bool			_image_draw_depth_into(t_image *target, t_image *source,
					t_draw_info dinfo);

t_bool			_image_draw_fx_into(t_image *target, t_image *source,
					t_draw_info dinfo);

t_color			sample_color(t_texture *img, t_dvec2 s_pos,
					t_dvec2_pair quad, t_ivec2 pd);

t_color			sample_color_round(t_texture *img, t_dvec2 s_pos,
					t_dvec2_pair quad, t_ivec2 pd);

/// Sprites and frames

// Constructs a new frame with all images set to YNULL.
t_frame			*frame_new(void);

// Updates `target` frame with images from `source`.
void			frame_set(t_frame *target, t_frame *source);

// Retrieves an image and it's horizontal scale value, depending on the
// camera viewing angle and relative direction.
t_image			*frame_get_image(t_frame *frame, t_camera *cam,
					t_dvec2 own_dir, int *xscale);

// Deletes the frame structure (but not the images).
void			frame_delete(t_frame *frame);

// Initializes sprite player.
void			spr_plr_init(t_sprite_player *sp, t_list *sprite);

// Advances sprite player state.
void			spr_plr_update(t_sprite_player *sp, double dt);

// Retrieves current sprite player state.
t_frame			*spr_plr_get_frame(t_sprite_player *sp);

// Deinitializes sprite player.
void			spr_plr_deinit(t_sprite_player *sp);

/// Graphics context

// Constructs a new graphics context. Requires window context, the size of
// the final drawing buffer (usually equals to the size of the window),
// and the amount of rendering threads to spawn and utilize.
t_gfx_ctx		*gfx_new(t_wctx wctx, t_ivec2 size, t_uint tcount);

// Deinitalizes and deletes the graphics context. Requires window context.
void			gfx_delete(t_gfx_ctx *gfx, t_wctx wctx);

// Draws the final drawing buffer to the screen. Flips the `display` and
// `target` image pointers internally.
void			gfx_draw_to_screen(t_gfx_ctx *gfx, t_wctx wctx);

/// Multithreaded rendering

// Initializes rendering threads from the graphics context.
void			init_rendering_threads(t_gfx_ctx *gfx);

// Deinitializes rendering threads from the graphics context.
void			deinit_rendering_threads(t_gfx_ctx *gfx);

// Private function!!! (Don't use it. Ever.). The main function of
// a rendering thread.
void			*_draw_thread_func(t_drawing_thread *thread);

// Performs the multithreaded rendering task. Requires the drawing
// context, which contains source image, its transform and the
// target image.
void			draw_multithreaded(t_gfx_ctx *gfx, t_dthread_type type,
					t_dthread_data data);

// Image drawing routine, used within the drawing thread.
void			mt_draw_image_into(t_drawing_thread *thread);

// Scene drawing routine, used within the drawing thread.
void			mt_draw_scene(t_drawing_thread *thread);

/// Cub parsing
// Configures one of the default textures in the game by loading it from
// the path, specified in `str_data` t_list of t_strings.
t_bool			cub_configure_texture(t_scene *scene, t_game *game,
					t_list *str_data);

/// Scene

// Constructs a t_scene out of the .cub file at the specified `cub_path`.
// Crashes the game with the relevant information if any misconfiguration
// was found.
t_scene			*scene_from_cub_file(t_game *game, const char *cub_path);

// Private function!!! Initializes a scene from the specified collection of
// character-definition bindings and iterator of the scene grid data string
// itself. Returns TRUE upon success, FALSE otherwise.
t_bool			_scene_load_grid(t_scene *scene, t_game *game, t_map *binds,
					t_list_iter cub_it);

// A helper structure that is used in some cases where 4 arguments is just
// not enough. It is only used during the .cub loading, and nowhere else..
typedef struct s_i_need_more_args
{
	t_scene	*scene;
	t_game	*game;
	t_map	*binds;
}	t_scene_init_args;

// Private function!!! Parses the bind character and runs the instruction
// specified in that bind at the specified coordinates.
t_bool			_handle_bind(t_scene_init_args args,
					t_grid_node *node, t_string *bind, t_ivec2 coords);

// Helper function for `_handle_bind`.
t_bool			_handle_bind2(t_scene_init_args args, t_grid_node *node,
					t_list_iter *it, t_ivec2 coords);

// Private function!!! Constructs an object from the specifed bind string
// iterator at the specified coordinates.
t_bool			_handle_object(t_scene_init_args args,
					t_list_iter *b_it, t_ivec2 coords);

// Private function!!! Loads a custom asset collection from the specified
// instruction in .cub file.
t_bool			_load_custom_assets(t_game *game, t_list *tokens);

// Initializes the scene to some default values. Requires game reference
void			scene_init(t_scene *scene, t_game *game);

// Private function!!! Constructs and returns the binds that implement the
// default behavior specified in the subject document.
t_map			*_scene_default_binds(void);

// Private function!!! Performs the scene validation according to the rules
// specified in the subject document, as well as some custom rules.
void			_scene_validate(t_scene *scene);

void			scene_refresh(t_scene *scene, t_game *game);

// Updates the state of the scene. Internally updates the player, updates
// the state of other objects, performs physics updates and updates the
// minimap. Requires the game struct reference and the delta time.
void			scene_update(t_scene *scene, t_game *game, double dt);

// Performs an update of scene components, that require to be updated in
// real-time.
void			scene_rt_update(t_scene *scene, t_game *game, double dt);

// Draws the scene to the screen by performing raycasting and drawing
// various GUI elements (TODO).
void			scene_draw(t_scene *scene, t_gfx_ctx *gfx);

// Perform an A* algorithm to find a shortest path between `start` and `end`
// grid cells in a scene.
t_list			*scene_find_path(t_scene *scene, t_ivec2 start, t_ivec2 end);

t_pf_node		*create_pf_node(t_ivec2 pos, t_pf_node *parent);

t_bool			pf_node_equals_vec(t_pf_node *a, t_ivec2 *b);

t_bool			pf_node_in_list(t_list *list, t_ivec2 pos);

t_pf_node		*find_pf_node_in_list(t_list *list, t_ivec2 pos);

int				pf_node_dist(t_ivec2 a, t_ivec2 b);

void			_a_star_step(int **grid, t_ivec2 v2[2], t_pf_node *node,
					t_list *lists[2]);

t_pf_node		*_get_next_node(t_list *olist);

// Converts a t_list of t_direction values into t_list of t_ivec2 values.
// Consumes and deletes the initial list!
t_list			*directions_to_vecs(t_list *dirs, t_ivec2 start);

// Deinitializes the internal state of the scene. Deletes various internal
// allocated resources.
void			scene_deinit(t_scene *scene, t_game *game);

// Deinitializes and frees the internal state of the scene.
void			scene_delete(t_scene *scene, t_game *game);

// Retrieves the reference to the grid node of the scene at the specified
// `x` and `y` coordinates. Returns `YNULL` if the specified coordinates
// are out of bounds.
t_grid_node		*scene_node_at(t_scene *scene, int x, int y);

// Retrieves the reference to the grid node of the scene at the specified
// `pos` t_ivec2 coordinates. Returns `YNULL` if the specified coordinates
// are out of bounds.
t_grid_node		*scene_node_at_i2(t_scene *scene, t_ivec2 pos);

// Retrieves the reference to the grid node of the scene at the specified
// `pos` t_dvec2 coordinates. Returns `YNULL` if the specified coordinates
// are out of bounds.
t_grid_node		*scene_node_at_d2(t_scene *scene, t_dvec2 pos);

t_rc_res		scene_cast_ray(t_scene *scene, t_dvec3 pos, t_dvec3 dir,
					t_pb_rc_config cfg);

/// Camera

void			init_camera(t_camera *camera, double fov, double aspect_ratio);

t_dvec2_pair	camera_get_lr_worldspace_nn(t_camera *cam, t_dvec2 dir);

t_dvec2_pair	camera_get_lr_worldspace(t_camera *cam, t_dvec2 dir);

/// Raycasting

void			raycast_scene(t_scene *scene, t_gfx_ctx *gfx);
void			raycast_region(t_scene *scene, double begin,
					double end, double interp);
void			draw_ceil_floor_colors(t_scene *scene, double begin,
					double end);

typedef struct s_raycast_query
{
	t_map		*vis_ent_lists;
	double		hit_dist;
	t_dvec2		hit_pos;
	t_direction	hit_dir;
	t_grid_node	*hit_node;
	double		tex_offset;
	double		cos_angle;
	t_uint		door_count;
	struct s_raycast_doors
	{
		t_grid_node	*node;
		t_dvec2		pos;
		double		tex_offset;
		t_bool		visible;
	}	doors[RC_DOOR_COUNT];
}	t_raycast_query;

typedef struct s_raycast
{
	t_dvec2	ray_len;
	t_dvec2	ray_dir;
	t_ivec2	step;
	t_ivec2	map_check;
	t_dvec2	ray_unit_step;
	t_bool	vertical;
	double	ray_dist;
}	t_raycast;

void			draw_query_result(t_scene *scene, t_raycast_query query,
					int pos);

t_raycast_query	cast_ray(t_scene *scene, t_dvec2 ray);

void			_door_ray_rc(t_scene *scene, t_raycast *rc,
					t_raycast_query *rq, t_grid_node *gn);

void			compute_texture_scaling(double col[6], t_dvec2 limits,
					double scale_factor, double tex_size_ratio);

void			collect_vis_ents(t_map *ents, t_map *new_ents);

void			vis_ent_draw(t_vis_ent *vent, t_scene *scene,
					t_dvec2 minmax, double interp);

// Another struct, the sole point of existence is to adhere to the norm.
typedef struct s__floor_draw_data
{
	double			start;
	double			end;
	t_dvec2			floor_step;
	t_dvec2			floor_pos;
	t_dvec2_pair	tl;
	double			row_dist;
	t_ivec2			px;
}	t__fdd;

void			draw_floors_and_ceilings(t_scene *scene, double start,
					double end);

t_grid_node		*_sample_floor_pos(t_scene *scene, t_dvec2 spos[3],
					t_ivec2 *gpos);

/// Physics

void			phys_body_init(t_phys_body *pb, t_scene *scene,
					double half_radius, t_dvec2 pos);

void			phys_body_deinit(t_phys_body *pb, t_scene *scene);

void			phys_body_move(t_phys_body *pb, t_scene *scene,
					t_dvec2 new_pos);

void			physics_update(t_scene *scene);

t_dvec2			resolve_collision(t_phys_body *pba, t_phys_body *pbb,
					t_dvec2 vel);

/// Grid utils

// Checks if provided t_grid_node_type represents a solid, unpassable node.
t_bool			node_is_solid(t_grid_node *gn);

t_bool			node_is_physical(t_grid_node *gn);

t_door_type		door_type_from_char_str(t_string *d);

/// Minimap utils

// Updates the minimap with the provided scene state.
void			minimap_update(t_image *minimap, t_scene *scene);

// Draws the minimap into the provided context.
void			minimap_draw(t_image *minimap, t_image *target);

/// Visual entity component

// Constructs a new visual entity at the specified position.
// Inserts itself into a scene node grid.
t_vis_ent		*vis_ent_new(t_scene *scene, t_dvec3 pos);

// Moves the entity into the new position. If specified position
// does not match entity's old grid node position, updates the
// scene node grid accordingly.
void			vis_ent_move(t_vis_ent *vent, t_dvec3 new_pos, t_scene *scene);

t_dvec3			vis_ent_get_pos(t_vis_ent *vent, double interp);

// Removes visual entity out of the scene node grid and deletes the entity.
void			vis_ent_delete(t_vis_ent *vent, t_scene *scene);

/// Thinker (NPC logic state machine)

void			thinker_init(t_thinker *think, void *data,
					t_think_func init_state);

// TODO: Do we even need this?
void			thinker_deinit(t_thinker *think);

t_inputs		thinker_update(t_thinker *think, t_scene *scene, double dt);

/// NPC

t_npc			*npc_new(t_scene *scene, t_actor *actor);

void			npc_update(t_npc *npc, t_scene *scene, t_game *game);

void			npc_post_physics(t_npc *npc, t_scene *scene);

void			npc_delete(t_npc *npc, t_scene *scene);

void			npc_td_clear_path(t_npc_thinker_data *td);

void			npc_get_hit(t_npc *npc, t_uint damage, t_dvec2 dir,
					t_think_func hurt_func);

/// Gunner
void			npc_traverse_path(t_npc_thinker_data *td, t_scene *scene,
					t_ivec2 target);

void			update_gunner_anim(t_npc_thinker_data *think_data,
					t_scene *scene, t_npc_state nstate, double dt);

t_thunk			gunner_random_walker(t_npc_thinker_data *think_data,
					t_scene *scene, double dt);

t_thunk			gunner_chase_player(t_npc_thinker_data *think_data,
					t_scene *scene, double dt);

t_thunk			gunner_get_hit(t_npc_thinker_data *td, t_scene *scene,
					double dt);

/// Actor component

// Constructs a new actor with the provided position and looking direction,
// and places it into the scene.
t_actor			*actor_new(t_scene *scene, t_dvec3 pos, t_dvec2 dir);

// Removes the provided actor from the scene and deletes it.
void			actor_delete(t_actor *actor, t_scene *scene);

// An actor update event. Updates the actor with the provided inputs.
void			actor_update(t_actor *actor, t_inputs inp, t_scene *scene,
					double dt);

void			actor_post_physics(t_actor *actor, t_scene *scene);

/// Props

t_prop			*prop_new(t_scene *scene, t_dvec3 pos, t_dvec2 dir);

void			prop_move(t_prop *prop, t_dvec3 new_pos, t_scene *scene);

void			prop_delete(t_prop *prop, t_scene *scene);

/// Pickups
t_item			*pickup_new(t_scene *scene, t_dvec2 pos);

void			pickup_delete(t_item *pickup, t_scene *scene);

t_bool			pickup_filter(t_item *item, t_scene *scene);

t_bool			setup_healthpack(t_item *item, t_game *game);

t_bool			setup_ammopack(t_item *item, t_game *game);

t_bool			setup_keycard(t_item *item, t_game *game, t_string *kc);

/// Object loaders (for .cub loading)

// Player constructor that initializes the player at the specified position,
// looking at the east.
void			scene_load_player(t_scene *scene, t_game *game, t_ivec2 pos);

t_bool			scene_load_prop(t_game *game, t_prop *prop, t_string *type);

void			scene_load_dummy(t_prop *prop, t_game *game);

void			scene_load_npc(t_game *game, t_scene *scene, t_npc *npc,
					t_npc_spawn_data sinfo);

void			scene_load_gunner(t_npc *npc, t_scene *scene, t_game *game);

/// Particles
t_particle		*spawn_particle(t_scene *scene, t_dvec3 pos,
					t_particle_type type);

void			update_particles(t_scene *scene);

void			delete_particles(t_scene *scene);

void			_configure_particle(t_particle *prt, t_scene *scene,
					t_particle_type type);

/// Asset reference initialization

// Following set of functions performs a one-time lookup for various game
// assets after the scene initialization stage.

void			scene_init_assets(t_scene *scene, t_game *game);

void			scene_deinit_assets(t_scene *scene, t_game *game);

void			scene_init_bandit_assets(t_scene *scene, t_game *game);

/// Player events

// Player's update event.
void			player_update(t_player *plr, t_scene *scene, t_game *game,
					double dt);

// Player's key press event. Updates the key value of players keymap state to
// TRUE.
void			player_key_pressed(t_player *plr, t_game_key key);

// Player's key released event. Updates the key value of players keymap state
// to FALSE.
void			player_key_released(t_player *plr, t_game_key key);

void			player_mouse_pressed(t_player *plr, int btn);

// Player's inputs update logic.
void			player_update_inputs(t_player *plr);

t_dvec2			player_collided(t_player *plr, t_phys_body *other,
					t_dvec2 resp);

void			player_rt_update(t_player *plr, t_scene *scene, t_game *game,
					double dt);

void			player_post_physics(t_player *plr, t_scene *scene);

/// Game callbacks

// Initializes and returns a t_game struct with the specified game config. 
t_game			game_new(t_game_config gcfg);

// Deinitializes the entire game, by unloading the scene, deinitializing game
// data (assets) and deinitializing the backend at last. 
int				game_deinit(t_game *game);

// A game update callback. Calculates delta time since the last frame and
// updates the scene accordingly.
int				game_update(t_game *game);

// A key pressed callback. Invokes the scene key pressed event.
void			game_key_pressed(t_game *game, t_game_key key);

// A key released callback. Invokes the scene key released event.
void			game_key_released(t_game *game, t_game_key key);

// (I dont know, honestly)
// void			game_load(t_game *game);

// Mouse movement callback. Not implemented yet.
void			game_mouse_pressed(t_game *game, int key);

/// HUD

void			hud_update(t_scene *scene, t_game *game, double dt);

void			hud_draw(t_scene *scene);

/// Asset management

// Constructs and returns a new empty assets collection.
t_asset_col		*assets_new(void);

// Deletes an asset collection. Requires a window context.
void			assets_delete(t_asset_col *col, t_wctx wctx);

// Loads asset collection from the specified asset collection text file.
// Crashes with the relevant crash message upon misconfiguration.
void			assets_load_from_file(t_asset_col *col, t_wctx wctx,
					const char *path);

t_bool			_assets_load_sprite(t_list_iter *it, t_asset_col *col);

// Loads an image at the specified `path` and binds it to the specified `name`
// in the asset collection.
t_bool			assets_load_image(t_asset_col *col, t_wctx wctx,
					const char *chs[2], t_ivec2 og);

// Returns the image, bound to the specified `name` if it exists. `YNULL`
// otherwise.
t_image			*assets_get_image(t_asset_col *col, const char *name);

t_frame			*assets_get_frame(t_asset_col *col, const char *name);

t_list			*assets_get_sprite(t_asset_col *col, const char *name);

/// Keybinds
// Retrieves a cstring that represents the name of the key
const char		*get_key_name(t_game_key key);

// Retrieves a game key from its name (this function is slow, dont call it
// too much)
t_game_key		get_key_from_string(t_string *key_str);

// Setup the keymap in the game
void			configure_keymap(t_game *game);

/// Utilities
// Displays an error message and shuts down (crashes) the game
void			error_exit(char *msg);

t_game			*exit_data(t_game *addr, t_bool write);

// Convert a `text` t_string into space/newline-separated list of substrings.
t_list			*tokenize_str(t_string *text);

// Retrieves current time
double			get_time(void);

// Performs a check whenever 2 boxes described with their positions and sizes
// intersect or not.
t_bool			iaabb_test(t_ivec2 apos, t_ivec2 asize, t_ivec2 bpos,
					t_ivec2 bsize);

// Same as iaabb_test, except for t_dvec2 boxes.
t_bool			daabb_test(t_dvec2 apos, t_dvec2 asize, t_dvec2 bpos,
					t_dvec2 bsize);

// Clear the input state.
void			inputs_clear(t_inputs *inputs);

// Creates the quad with top-left corner being at (0, 0) and bottom-right
// being at the specified size.
t_ivec2_pair	full_quad(t_ivec2 size);

// Creates a new draw info struct with the specified type.
t_draw_info		draw_info_new(t_draw_type type);

t_draw_info		draw_info_from_tf2d(t_draw_type type, t_image *img,
					t_transform2d tf);

// Generates a random double between 0.0 and 1.0.
double			random_unit(void);

// Generates a random int between 0 and n.
int				randomn(int n);

t_npc_type		get_npc_type(t_string *type);

t_ivec2			mouse_delta(t_game *game);

#endif
