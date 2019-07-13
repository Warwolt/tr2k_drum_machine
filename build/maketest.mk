# work in progress "non-recursive" makefile

# make won't resolve ifeq true if there are any spaces in the below statement!
UNIT_TESTING :='TRUE'# include test files in this build

PROJ_ROOT = ..
APP_ROOT = $(PROJ_ROOT)/app
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

# google test
INC += -I$(LIB_DIR)/googletest/include
LIB += $(LIB_DIR)/googletest/gtest_main.a

# Add google test and google mock libraries
INC += -I$(LIB_DIR)/googlemock/include
LIB += $(LIB_DIR)/googlemock/gmock.a
INC += -I$(LIB_DIR)/mockheaders # used to mock out avi library headers

# Embedded template library
INC += -I$(LIB_DIR)/embeddedtemplatelibrary/Include
INC += -I$(LIB_DIR)/embeddedtemplatelibrary/Include/etl/profiles

.PHONY: clean directories
all: $(OBJ_DIR) $(BIN)

# Include all module-specific makefiles
-include modules.mk

# Include all dependency .d files
DEP = $(OBJ:%.o=%.d)
-include $(DEP)

$(BIN): $(OBJ)
	@echo Building executable $@
	$(CC) $(FLAGS) $(INC) $^ $(LIB) $(LINK_FLAGS) -o $@

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR) # Makes folder if missing

clean:
	@echo Cleaning folder
	rm -rf $(OBJ_DIR) $(BIN)

debug:
	@echo OBJ = $(OBJ)
