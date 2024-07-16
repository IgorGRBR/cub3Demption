# cub3Demption

The cub3D project from 42 School.

This is a simple game (engine) written in C, using primarily [miniLibX](https://github.com/42Paris/minilibx-linux), optionally [SDL2](https://www.libsdl.org/) (as an alternative for miniLibX) and [yLib](https://github.com/IgorGRBR/ylib). Header files within the root directory, as well as source files within `src` directory are written in accordance to 42 School Norm v4*.

This game was meant to be a continuation of [so_sneaky](https://github.com/IgorGRBR/so_sneaky) project. Similarily to so_sneaky, you are still playing as a secret agent attempting an escape from the base of the enemy. Levels** in this game are series of rooms and corridors that you have to traverse through. And just like in the previous game, they are filled with enemies, doors and various items that you will encounter on your way out. Unlike the previous game, the player is now wielding a weapon of its own and is able to engage themselves in fights against opponents.

As a player, your goal is to reach the exit of the level. Exiting a level does not require one to collect all the items, like in so_sneaky, but locked doors are still present and they still require you to collect keycards to unlock. Along the keycards, there are ammo packs scattered around levels that partially refill your limited ammo supply, as well as health packs that heal you upon picking them up. Bandits are also present. Unlike in so_sneaky, they don't use simplistic algorithms to follow a predefined path. Instead, each bandit patrols a certain, configurable area in the level, and, when it notices the player or gets hit by player's gunfire, starts chasing the player and gunning them down. To chase the player, guards move directly towards them when there is a line of sight, and otherwise apply the A* algorithm to find a path to the player. Player wins the game if they reach the exit, and loses when their health reaches 0.

\* My definition of compatible is that the norminette doesn't complain about the code :). Also, there have been a few changes to the code in order to clean it up for the github release. These changes were'nt verified with the norminette tool, so it might not pass the norm check.

\*\* This game intended to have much more levels and content, but I couldn't finish any of it due to personal time restrictions, so currently what you see in the repo is more of an MVP than the actual finished project.

![screenshot of level.cub map](/images_src/screenshot.png)

---

## Building instructions

*I only tested building and running this game on MacOS, Fedora 40 and Ubuntu 20.04.*

### Building on Linux

Make sure you have installed all the necessary dependencies:

* On Fedora:
`sudo dnf install make clang libXext libXext-devel zlib zlib-devel libbsd libbsd-devel SDL2 SDL2-devel SDL2_ttf SDL2_ttf-devel`

* On Debian/Ubuntu:
`TODO`

After installing dependencies, `cd` into the repository directory. To compile the project with miniLibX backend, just run `make`, however it is recomended to compile the project with SDL2, by running `SDL=1 make`. MiniLibX library build is not guaranteed to work indefinitely and I'm not keeping it up to date myself.

### Building on Windows 11

Install either Fedora, Debian or Ubuntu under WSL2 and follow the Linux build instructions.

### Building on Windows 10

*TODO*

## Playing the game

You can launch the game from the command line, using:

`./cub3D <path to the level.cub file>`

For example:

`./cub3D maps/level.cub`

will launch the demonstration level of the game, that currently contains all of the features the engine has to offer.

Player can move around using WSAD keys on the keyboard, interact with doors using 'E' key, rotate the view with arrow keys or by moving a mouse and shoot using either spacebar or left mouse button. Player can exit at any moment by pressing ESC.

## Game configuration

Game can be configured with either `config_mlx_lx.txt` file (on miniLibX builds), or `config_sdl.txt` (on SDL2 builds). Config files contain various configuration properties. Each of those properties are configured by specifying the configuration identifier, followed by a set of parameters for that identifier. Here is a list of all identifiers and their properties:

* window_size <width> <height> - sets the size of a window to the specified width and height in pixels.
* resolution_height <height> - sets the internal rendering resolution to the specified height. Width is determined automatically from the window aspect ratio.
* keybind <identifier> <value> - binds the key specified by identifier to a numeric value representing a key. Numeric key values can be obtained [here] for miniLibX, and [here] for SDL2. Following is a list of valid identifiers:
	* forward
	* backward
	* strafe_left
	* strafe_right
	* shoot
	* look_left
	* look_right
	* use
	* exit
	* debug
* rendering_threads <count> - sets the amount of threads to be used for performing rendering tasks.
* sensitivity <percent> - sets the viewing sensitivity of the game with an integer, representing a percentage.
* field_of_view <degrees> - specifies horizontal FoV in degrees. Vertical FoV is determined automatically from the aspect ratio of a window.
* mouselook <boolean> - specifies whether to use a mouse to rotate the view with an integer representing a boolean value (1 for true, 0 for false).

---

## Technical features

Primary goal of the project was to create a program that displays a view of a maze-like environment from the first person perspective by rendering it with the raycasting approach. The resulting application meets all of the mandatory and bonus requirements, specified in the subject description document from 42 School. On top of that, there is a number of features that extend the functionality of this application beyond the specified requirements. The decision to implement those features was made by myself as an attempt to test and push my knowledge of the domain as much as possible.

### Raycasting

Raycasting is the primary mechanism used in this engine for rendering the view of the scene and performing certain physics checks. Ray traversal is done using Digital Differential Analysis algorithm. During the rendering step, raycasting is used to determine the distance to a point on a wall from the camera, as well as collect and aggregate lists of visible entities. In physics, raycasting is primarily used to determine what a shot bullet has hit, spawn particles and determine if a straight path exists between 2 actors.

### Rendering

Due to the nature of the project, the entire game is being rendered in software. There are usual image rendering procedures that allow one to render an image into another image with applied scaling and clipping. These procedures aren't made to be performant, as I don't consider myself to be capable of writing them optimally with given restrictions to the programming style. Instead, they are made to be quite flexible, allowing the user to use depth information when rendering images, or apply programmable effects to the rendered image (similarily to how pixel shaders work in OpenGL, for example). To regain some of the lost performance, the entire game is rendered at a lower-than native resolution and the resulting frame is being scaled up to match the dimensions of a window. Another method used to improve rendering performance is multithreading - the vast majority of the scene rendering pipeline is being performed in parallel, allowing the end user to utilize most of their systems to achieve optimal smoothness. Internal rendering resolution and amount of rendering threads can both be configured within a configuration file.

### Physics

The engine employs a simple AABB physics simulation. Since the level is represented as a grid of nodes, the underlying grid structure is already being used as a spatial optimization structure for collision detection and resolution. Each physical body can be configured with certain event callbacks, allowing the programmer to define the behavior of collisions of bodies of various types. Physical bodies are usually treated as axis-aligned bounding boxes, however, when performing ray-intersection tests, they are treated as circles.

### NPC Logic

The AI of every NPC in the game is implemented as a finite state machine (FSM), where each state is represented with a state function pointer and some internal NPC context data. A state function always returns another state function pointer, hence changing state is performed by iteratively running the state function of every NPC in a level. At a high level, enemies in the game only have 2 states - neutral and aggressive. In a neutral state, they either stand still or walk randomly in a circular area with a configurable radius. In an aggressive state, they chase down the target (the player), until either they die, or the target is neutralized. When they are able to see the target (a line of sight exists), they simply attempt to move directly towards it. Otherwise, they employ the A* pathfinding algorithm to find the shortest path to the target, and then follow that path.

### Asset management

`assets.txt` contains a list of all assets the game uses. There are 3 types of assets. Each one of them can be defined using following expressions:

* image <id> <file_path> [offset_x] [offset_y] - defines an image asset with specified `id`, with data being loaded from specified file. Optionally, user can provide x and y offsets of the image if necessary (0 and 0 will be used otherwise).
* frame <id> <east_image_id> <south_image_id> <west_image_id> <north_image_id> - defines a frame asset with specified `id`, by specifying the set of images to associate with given id. East/west images are optional (user can specify non-existent `id` at their place) if image from the opposite direction is specified. In that case, it will simply be mirrored when a frame is drawn.
* sprite <id> <frame_ids...> - defines a sprite asset with specified `id`, followed by a variable-length list of frame ids.

Assets are not dynamically managed (meaning that they are loaded and unloaded only once).

### Level format

Level data is stored in `.cub` files inside the `maps` folder. Original file format of a cub file is defined within the project document, and is fully supported by the engine. However, file format of the level has been extended to be much more flexible in order to support additional engine features. More specifically - the cub file is split in 2 halfs - level configuration and the grid. Level configuration half allows level designer to specify textures of the walls, and which colors to use for floors and ceilings (unused, since floors and ceilings are always textured, but kept in engine for the sake of compatibility with generic `.cub` files). Inside the grid half of the file, level designer creates a level with a grid of characters, each one of which corresponds to a certain grid cell configuration (for example - `0` is a free-walking space, whereas `1` is a solid wall, etc...). The set of characters is, usually, defined in the subject document and is very limited, however the engine allows one to define any custom definitions for any character using `cub` keyword within the `cub` file.

*TODO: describe `cub` expressions in more detail*

User can also overload game assets with the `assets` keyword, followed with a path to a custom `assets.txt` file.
