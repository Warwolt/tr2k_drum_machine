# Include module-specific makefiles
-include $(APP_ROOT)/common/makefile.mk
-include $(APP_ROOT)/domain/playback/tempotimer/makefile.mk
-include $(APP_ROOT)/infrastructure/interrupts/makefile.mk
-include $(APP_ROOT)/hal/makefile.mk
-include $(APP_ROOT)/drivers/gpio/makefile.mk
-include $(APP_ROOT)/drivers/timer1/makefile.mk
