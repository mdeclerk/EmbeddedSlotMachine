SRC_DIR 	:= src
OBJ_DIR 	:= obj
BIN_DIR 	:= bin

CXX 		:= $(CROSS_COMPILE)g++
CXXFLAGS 	:= -MMD -MP -std=c++23 -Wall -Wextra -O2
LDFLAGS 	:= -Wl,-rpath,'$$ORIGIN'