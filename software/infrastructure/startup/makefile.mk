#
# This file is included in the main makefile only if not unit testing.
# It makes no sense to include the on target application startup if running on host.
#

# It's important we don't link these files during unit testing!
ifneq ($(UNIT_TESTING),'TRUE')

MODULE_DIR := $(APP_ROOT)/infrastructure/Startup
-include common_makefiles/src_inc.mk

endif
