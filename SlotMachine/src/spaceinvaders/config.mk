MODULES 				+= spaceinvaders
spaceinvaders_TARGET	= $(BIN_DIR)/spaceinvaders
spaceinvaders_SRC_DIR	= $(SRC_DIR)/spaceinvaders
spaceinvaders_OBJ_DIR	= $(OBJ_DIR)/spaceinvaders
spaceinvaders_DEPS		= libncursesui
spaceinvaders_CXXFLAGS 	= -I$(libncursesui_SRC_DIR)
spaceinvaders_LDFLAGS 	= -L$(dir $(libncursesui_TARGET)) -lncursesui