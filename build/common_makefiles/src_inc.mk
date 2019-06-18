#
# Common makefile for source and include definition
#
# Included by module-specific makefiles, that define MODULE_DIR

# Define folders
SRC_DIR := $(MODULE_DIR)/src
INC_DIR := $(MODULE_DIR)/inc

# Export local files to global INC and OBJ
SRC := $(wildcard $(SRC_DIR)/*.cpp)
INC += -I$(INC_DIR)
OBJ += $(patsubst %.cpp,%.o, $(patsubst $(SRC_DIR)/%, $(OBJ_DIR)/%, $(SRC)))

# Rule for compiling local source files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CC) $(INC) $(FLAGS) -c $< -MMD -o $@
