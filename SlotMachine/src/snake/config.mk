MODULES 		+= snake
snake_TARGET	= $(BIN_DIR)/snake
snake_SRC_DIR	= $(SRC_DIR)/snake
snake_OBJ_DIR	= $(OBJ_DIR)/snake
snake_DEPS		= libncursesui
snake_CXXFLAGS 	= -I$(libncursesui_SRC_DIR)
snake_LDFLAGS 	= -L$(dir $(libncursesui_TARGET)) -lncursesui