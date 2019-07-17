#
# Common makefile for interface definition
#
# Included by module-specific makefiles, that define MODULE_DIR

# Define folders
IF_DIR := $(MODULE_DIR)/if
MOCK_DIR := $(IF_DIR)/mock

# Export local files to global INC
INC += -I$(IF_DIR)
INC += -I$(MOCK_DIR)
