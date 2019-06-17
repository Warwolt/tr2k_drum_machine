#
# Common makefile for interface definition
#
# Included by module-specific makefiles, that define MODULE_DIR

# Define folders
IF_DIR := $(MODULE_DIR)/if

# Export local files to global INC
INC += -I$(IF_DIR)
