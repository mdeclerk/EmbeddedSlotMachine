MODULES 		+= tetris
tetris_TARGET	= $(BIN_DIR)/tetris
tetris_SRC_DIR	= $(SRC_DIR)/tetris
tetris_OBJ_DIR	= $(OBJ_DIR)/tetris
tetris_DEPS		= libncursesui
tetris_CXXFLAGS = -I$(libncursesui_SRC_DIR)
tetris_LDFLAGS 	= -L$(dir $(libncursesui_TARGET)) -lncursesui