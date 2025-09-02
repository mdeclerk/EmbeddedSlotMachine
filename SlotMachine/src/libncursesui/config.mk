MODULES 				+= libncursesui
libncursesui_TARGET		= $(BIN_DIR)/libncursesui.so
libncursesui_SRC_DIR	= $(SRC_DIR)/libncursesui
libncursesui_OBJ_DIR	= $(OBJ_DIR)/libncursesui
libncursesui_DEPS		= 
libncursesui_CXXFLAGS 	= -fPIC
libncursesui_LDFLAGS 	= -shared -lncurses