# Include module-specific makefiles
-include $(APP_ROOT)/common/makefile.mk

# Presentation
-include $(APP_ROOT)/presentation/tempo_control/tempo_knob/makefile.mk
-include $(APP_ROOT)/presentation/tempo_control/tempo_control_view/makefile.mk

# Domain
-include $(APP_ROOT)/domain/playback/beats_per_minute/makefile.mk
-include $(APP_ROOT)/domain/playback/tempo_timer/makefile.mk
-include $(APP_ROOT)/domain/playback/tempo_timing_manager/makefile.mk

# Infrastructure
-include $(APP_ROOT)/infrastructure/interrupts/makefile.mk

# Hardware Abstraction Layer
-include $(APP_ROOT)/hal/makefile.mk

# Drivers
-include $(APP_ROOT)/drivers/gpio/makefile.mk
-include $(APP_ROOT)/drivers/rotary_encoder/makefile.mk
-include $(APP_ROOT)/drivers/segment_display/makefile.mk
-include $(APP_ROOT)/drivers/spi/makefile.mk
-include $(APP_ROOT)/drivers/timer0/makefile.mk
-include $(APP_ROOT)/drivers/timer1/makefile.mk

