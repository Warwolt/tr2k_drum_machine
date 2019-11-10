# work in progress "non-recursive" makefile

# make won't resolve ifeq true if there are any spaces in the below statement!
UNIT_TESTING :='TRUE'# include test files in this build

PROJ_ROOT = ..
APP_ROOT = $(PROJ_ROOT)/software
OBJ_DIR = obj/test
LIB_DIR = $(PROJ_ROOT)/lib

INC :=
OBJ :=
LIB :=
BIN := test.exe

CC := g++

FLAGS := -Wall
FLAGS += -fmax-errors=5
FLAGS += -std=gnu++11
FLAGS += -DUNIT_TESTING

# google test
INC += -I$(LIB_DIR)/googletest/include
LIB += $(LIB_DIR)/googletest/gtest_main.a

# Add google test and google mock libraries
INC += -I$(LIB_DIR)/googlemock/include
LIB += $(LIB_DIR)/googlemock/gmock.a

# Add borrowed Standard Template Library files
INC += -I$(LIB_DIR)

# Add avr header mocks
-include $(LIB_DIR)/mockheaders/avr/makefile.mk

.PHONY: clean directories
all: $(OBJ_DIR) $(BIN)

# Include all module-specific makefiles
-include modules.mk

# Avr header mocks
$(OBJ_DIR)/%.o: $(AVR_MOCK_DIR)/%.cpp
	@echo "Compiling AVR header mock $(notdir $@)"
	@$(CC) $(INC) $(FLAGS) -c $< -MMD -o $@

# Include all dependency .d files
DEP = $(OBJ:%.o=%.d)
-include $(DEP)

$(BIN): $(OBJ)
	@echo "Linking $(notdir $@)"
	@$(CC) $(FLAGS) $(INC) $^ $(LIB) $(LINK_FLAGS) -o $@

# Makes folder if missing
$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

clean:
	@echo "Cleaning test build folders"
	@rm -rf $(OBJ_DIR) $(BIN)

