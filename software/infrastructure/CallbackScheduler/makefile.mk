#
# This file is included in the main makefile only if not unit testing.
# It makes no sense to include the on target application startup if running on host.
#

MODULE_DIR := $(APP_ROOT)/infrastructure/CallbackScheduler
-include common_makefiles/src_inc.mk
-include common_makefiles/test.mk
