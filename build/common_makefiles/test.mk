#
# Common makefile for unit test definition
#
# Included by module-specific makefiles, that define MODULE_DIR

ifeq ($(UNIT_TESTING),'TRUE')

# Define folders
TEST_DIR := $(MODULE_DIR)/test

# Export local files to global OBJ
TEST_SRC := $(wildcard $(TEST_DIR)/*.cpp)
OBJ += $(patsubst %.cpp,%.o, $(patsubst $(TEST_DIR)/%, $(OBJ_DIR)/%, $(TEST_SRC)))

# Rule for compiling local test files
$(OBJ_DIR)/%.o: $(TEST_DIR)/%.cpp
	@echo "Compiling unit test"
	g++ $(INC) $(FLAGS) -c $< -MMD -o $@

endif
