MODULES 		+= init
init_TARGET		= $(BIN_DIR)/init
init_SRC_DIR	= $(SRC_DIR)/init
init_OBJ_DIR	= $(OBJ_DIR)/init
init_DEPS		= libncursesui
init_CXXFLAGS 	= -I$(libncursesui_SRC_DIR)
init_LDFLAGS 	= -L$(dir $(libncursesui_TARGET)) -lncursesui