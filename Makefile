NAME=cub3D

UNAME=$(shell uname)

CFLAGS=-Wall -Wextra -Werror -O3
CFLAGS_DEBUG = -Wall -Wextra -Werror
AR = ar -rc
ifeq ($(UNAME),Linux)
	# SDL Backend vars
	# LFLAGS = -lSDL2 -lSDL2main -lSDL2_ttf -lm -L/usr/lib -lXext -lX11 -lm -lz -L/usr/lib
	# BACKEND_PATH = ""
	# EXT_DEFINES = -D LINUX -D BACKEND=SDL

	# MLX backend vars
	LFLAGS = -lm -L/usr/lib -lXext -lX11 -lm -lz -L/usr/lib
	BACKEND_PATH = minilibx-linux/
	BACKEND_LIB = $(BACKEND_PATH)libmlx.a
	COMPILE_BACKEND = 1
	EXT_DEFINES = -D LINUX -D BACKEND=MLX
	# EXT_DEFINES = -D LINUX -D BACKEND=MLX -D NO_MT_RENDERING

	EXT_INCLUDE = /usr/include
	CC = cc
else
	LFLAGS = -lm -Lmlx -lmlx -framework OpenGL -framework AppKit
	BACKEND_PATH = mlx/
	BACKEND_LIB = $(BACKEND_PATH)libmlx.dylib
	COMPILE_BACKEND = 1
	EXT_INCLUDE = ""
	EXT_DEFINES = -D OSX BACKEND=MLX
	CC = cc
endif

# source and object directories
SRC_ROOT = src
OBJ_ROOT = obj
DBG_OBJ_ROOT = dobj
SRC_DIR = $(sort $(SRC_ROOT)/ $(dir $(wildcard $(SRC_ROOT)/*/)))
OBJ_DIR = $(patsubst $(SRC_ROOT)/%,$(OBJ_ROOT)/%,$(SRC_DIR))
DBG_OBJ_DIR = $(patsubst $(SRC_ROOT)/%,$(DBG_OBJ_ROOT)/%,$(SRC_DIR))

$(info $$SRC_DIR is [${SRC_DIR}])
$(info $$OBJ_DIR is [${OBJ_DIR}])

# ylib directories
YLIB_PATH = ylib/
YLIB_OBJ = $(wildcard $(addsuffix *.o, $(YLIB_PATH)obj/))
YLIB_DBG_OBJ = $(wildcard $(addsuffix *.o, $(YLIB_PATH)dobj/))
YLIB_LIB = $(YLIB_PATH)ylib.a

# object files
SRC = $(wildcard $(addsuffix *.c, $(SRC_DIR)))
OBJ = $(patsubst $(SRC_ROOT)/%.c,$(OBJ_ROOT)/%.o,$(SRC))
DBG_OBJ = $(patsubst $(SRC_ROOT)/%.c,$(DBG_OBJ_ROOT)/%.o,$(SRC))

$(info $$SRC is [${SRC}])
$(info $$OBJ is [${OBJ}])

# defines and includes
DEFINES = -D GNL_BSIZE=24 -D LIST_C_STEP=20 -D MLX=1 -D SDL=2 $(EXT_DEFINES)
INC_DIR = -I ./ -I $(YLIB_PATH) -I $(BACKEND_PATH) -I $(EXT_INCLUDE)

all: $(NAME)

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

$(DBG_OBJ_DIR):
	@mkdir -p $(DBG_OBJ_DIR)

$(NAME): ylib backend $(OBJ)
	@echo $(NAME): creating binary $(NAME) for platform $(UNAME)
	$(CC) -o $(NAME) $(OBJ) $(YLIB_LIB) $(BACKEND_LIB) $(LFLAGS)

$(OBJ_ROOT)/%.o : $(SRC_ROOT)/%.c | $(OBJ_DIR)
	@echo $(NAME): building $@
	@$(CC) $(CFLAGS) $(INC_DIR) $(DEFINES) -c $< -o $@

debug: ylib-debug backend $(DBG_OBJ)
	@echo $(NAME): creating debuggable binary $(NAME) for platform $(UNAME)
	@$(CC) -o $(NAME) $(DBG_OBJ) $(YLIB_LIB) $(BACKEND_LIB) $(LFLAGS)

$(DBG_OBJ_ROOT)/%.o : $(SRC_ROOT)/%.c | $(DBG_OBJ_DIR)
	@echo $(NAME): debug building $@
	@$(CC) $(CFLAGS_DEBUG) $(INC_DIR) $(DEFINES) -c $< -o $@ -g

ylib:
	make -C $(YLIB_PATH) ylib

ylib-debug:
	make -C $(YLIB_PATH) debug

ylib-clean:
	make -C $(YLIB_PATH) clean

ylib-fclean:
	make -C $(YLIB_PATH) fclean

backend:
ifdef COMPILE_BACKEND
	@echo "Building multimedia backend"
	make -C $(BACKEND_PATH) || true
endif

backend-clean:
ifdef COMPILE_BACKEND
	@echo "Cleaning multimedia backend"
	make -C $(BACKEND_PATH) clean || true
endif

clean: ylib-clean backend-clean
	@echo $(NAME): cleaning objects
	@rm -f $(OBJ) $(DBG_OBJ)

fclean: clean ylib-fclean backend-clean
	@echo $(NAME): cleaning build artifacts
	@rm -f $(NAME)

re: fclean all

redb: fclean debug

.PHONY: all clean fclean re debug redb ylib ylib-debug ylib-clean ylib-fclean
