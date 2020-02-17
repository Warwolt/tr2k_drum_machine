DOC_DIR := $(basename $(abspath $(lastword $(MAKEFILE_LIST))))
SRC_DIR := src
IMG_DIR := img

SEQ_DIR := sequence_diagrams
CLASS_DIR := class_diagrams

SRC := $(wildcard $(SRC_DIR)/*/*.puml) $(wildcard $(SRC_DIR)/*/*/*.puml)
IMG := $(patsubst %.puml,%.png, $(patsubst $(SRC_DIR)/%, $(IMG_DIR)/%, $(SRC)))

.PHONY: clean doc
all: $(IMG_DIR) doc

# Phony target for all images
doc: $(IMG)

# NOTE: The way that the plantuml.jar works is that it runs in the same
# directory as whatever file we feed it, so it will output the file relative to
# the directory of the source file.
#
# There is an option -o to assign a path to output the file to, but I couldn't
# get it to work for source files residing at different depths, so as a work
# around the output file is generated in the same directory as the source file
# and then moved using mv, since that works better with relative paths.
#
# Create png from puml source in sequence_diagrams subdirectory
$(IMG_DIR)/%.png: $(SRC_DIR)/%.puml
	@echo generating $(notdir $@)
	@mkdir -p $(dir $@)
	@$(eval IMG_SRC := $(patsubst %.puml, %.png, $<))
	@$(eval DST_SRC := $(patsubst $(SRC_DIR)/%, $(IMG_DIR)/%, $(IMG_SRC)))
	@java -jar jar/plantuml.jar $^
	@mv $(IMG_SRC) $(DST_SRC)

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
	@echo PROJ_ROOT = $(PROJ_ROOT)
