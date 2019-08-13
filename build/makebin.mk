PROJ_ROOT = ..
APP_ROOT = $(PROJ_ROOT)/app
OBJ_DIR = obj/bin
LIB_DIR = $(PROJ_ROOT)/lib

INC :=
OBJ := $(OBJ_DIR)/main.o
LIB :=

CC = avr-g++

FLAGS := -Wall -std=c++11
FLAGS += -fmax-errors=5

# AVR specific
MCU = atmega328p
F_CPU = 16000000
FLAGS += -g -Os -mmcu=$(MCU) -DF_CPU=$(F_CPU)
TARGET := main
BIN  := $(OBJ_DIR)/$(TARGET).bin
HEXFILE := $(OBJ_DIR)/$(TARGET).hex
OBJCOPY  := avr-objcopy
COM_PORT := com5 # NOTE: This must correspond to the usb port you use!

# Definition of operations new and delete
LIB += $(LIB_DIR)/newdelete/newdelete.a
INC += -I$(LIB_DIR)/newdelete/inc/

# Add borrowed Standard Template Library files
INC += -I$(LIB_DIR)
INC += -I$(LIB_DIR)/stlstub

.PHONY: clean directories
all: $(OBJ_DIR) $(BIN)

# Include all module-specific makefiles
-include modules.mk

# Include all dependency .d files
DEP = $(OBJ:%.o=%.d)
-include $(DEP)

# Make avr binary
$(BIN): $(OBJ)
	$(CC) $(FLAGS) $(OBJ) $(LIB) -MMD -o $(BIN)
	$(OBJCOPY) -j .text -j .data -O ihex $(BIN) $(HEXFILE)

$(OBJ_DIR)/main.o: $(APP_ROOT)/main.cpp
	$(CC) $(FLAGS) $(INC) $< -MMD -c -o $@

# Flash program to microcontroller
flash:
	avrdude -p $(MCU) -c arduino -U flash:w:$(HEXFILE):i -F -P $(COM_PORT)

# Make folder if missing
$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

clean:
	@echo Cleaning folder
	rm -rf $(BIN) $(OBJ_DIR)

debug:
	$(CC) -E $(FLAGS) $(INC) $(PROJ_ROOT)/app/main.cpp $(LIB) $(LINK_FLAGS)
