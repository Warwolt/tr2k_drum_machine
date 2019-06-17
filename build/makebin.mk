PROJ_ROOT = ..
APP_ROOT = $(PROJ_ROOT)/app
OBJ_DIR = obj/bin
LIB_DIR = $(PROJ_ROOT)/lib

INC :=
OBJ := $(OBJ_DIR)/main.o
LIB :=
BIN := pong.exe

FLAGS := -Wall -std=c++11
FLAGS += -fmax-errors=5

# SDL2
LIB += -L$(LIB_DIR)/SDL2/lib
INC += -I$(LIB_DIR)/SDL2/include
LINK_FLAGS := -lSDL2main -lSDL2

.PHONY: clean directories
all: $(OBJ_DIR) $(BIN)

# Include all module-specific makefiles
-include modules.mk

# Include all dependency .d files
DEP = $(OBJ:%.o=%.d)
-include $(DEP)

debug:
	@echo DEP = $(DEP)

# Compile main
$(OBJ_DIR)/main.o: $(APP_ROOT)/main.cpp
	g++ $(INC) -c $< -MMD -o $@

# Link program executable
$(BIN): $(OBJ)
	@echo Building executable $@
	g++ $(FLAGS) $(INC) $^ $(LIB) $(LINK_FLAGS) -o $@

# Make folder if missing
$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

clean:
	@echo Cleaning folder
	rm -rf $(BIN) $(OBJ_DIR)
