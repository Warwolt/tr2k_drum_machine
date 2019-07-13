#
# Special makefile for avr header mocks
#
# Included by maketest.mk

# Define folders
SRC_DIR := $(LIB_DIR)/mockheaders/avr

# Export local files to global INC and OBJ
SRC := $(wildcard $(SRC_DIR)/*.cpp)
INC += -I$(LIB_DIR)/mockheaders # used to mock out avi library headers
OBJ += $(patsubst %.cpp,%.o, $(patsubst $(SRC_DIR)/%, $(OBJ_DIR)/%, $(SRC)))

# Rule for compiling local source files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CC) $(INC) $(FLAGS) -c $< -MMD -o $@
