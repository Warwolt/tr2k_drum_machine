# Include module-specific makefiles
-include $(APP_ROOT)/common/makefile.mk

# Presentation
-include $(APP_ROOT)/presentation/tempo_control/tempo_control_view/makefile.mk
-include $(APP_ROOT)/presentation/tempo_control/tempo_knob/makefile.mk

# Application
-include $(APP_ROOT)/application/rhythm_playback_controller/makefile.mk

# Domain
-include $(APP_ROOT)/domain/rhythm_playback/beats_per_minute/makefile.mk
-include $(APP_ROOT)/domain/rhythm_playback/rhythm_playback_controller/makefile.mk
-include $(APP_ROOT)/domain/rhythm_playback/tempo_timer/makefile.mk
-include $(APP_ROOT)/domain/rhythm_playback/tempo_timing_manager/makefile.mk

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

