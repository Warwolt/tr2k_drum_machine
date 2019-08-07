# Include module-specific makefiles
-include $(APP_ROOT)/common/makefile.mk

# Domain
-include $(APP_ROOT)/domain/playback/beatsperminute/makefile.mk
-include $(APP_ROOT)/domain/playback/tempotimer/makefile.mk
-include $(APP_ROOT)/domain/playback/timingmanager/makefile.mk

# Infrastructure
-include $(APP_ROOT)/infrastructure/interrupts/makefile.mk

# Hardware Abstraction Layer
-include $(APP_ROOT)/hal/makefile.mk

# Drivers
-include $(APP_ROOT)/drivers/gpio/makefile.mk
-include $(APP_ROOT)/drivers/timer0/makefile.mk
-include $(APP_ROOT)/drivers/timer1/makefile.mk
-include $(APP_ROOT)/drivers/spi/makefile.mk
