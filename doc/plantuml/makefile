SRC_DIR := src
IMG_DIR := img

SEQ_DIR := sequence_diagrams
CLASS_DIR := class_diagrams

SRC := $(wildcard $(SRC_DIR)/$(SEQ_DIR)/*.txt)
SRC += $(wildcard $(SRC_DIR)/$(CLASS_DIR)/*.txt)
IMG := $(patsubst %.txt,%.png, $(patsubst $(SRC_DIR)/%, $(IMG_DIR)/%, $(SRC)))

.PHONY: clean doc
all: $(IMG_DIR) doc

# Phony target for all images
doc: $(IMG)

# Create png from txt source in sequence_diagrams subdirectory
$(IMG_DIR)/$(SEQ_DIR)%.png: $(SRC_DIR)/$(SEQ_DIR)%.txt
	java -jar jar/plantuml.jar $^ -o ../../$(IMG_DIR)/$(SEQ_DIR)

# Create png from txt source in class_diagrams subdirectory
$(IMG_DIR)/$(CLASS_DIR)%.png: $(SRC_DIR)/$(CLASS_DIR)%.txt
	java -jar jar/plantuml.jar $^ -o ../../$(IMG_DIR)/$(CLASS_DIR)

# Test that graphvizdot is installed and configured
testdot:
	java -jar jar/plantuml.jar -testdot

# Make folder if missing
$(IMG_DIR):
	@mkdir -p $(IMG_DIR)

clean:
	@echo cleaning..
	@rm -rf $(IMG_DIR)

debug:
	@echo SRC = $(SRC)
	@echo IMG = $(IMG)
