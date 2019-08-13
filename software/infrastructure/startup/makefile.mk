#
# This file is included in the main makefile only if not unit testing.
# It makes no sense to include the on target application startup if running on host.
#

ifneq ($(UNIT_TESTING),'TRUE')

MODULE_DIR := $(APP_ROOT)/infrastructure/startup
-include common_makefiles/src_inc.mk

endif
