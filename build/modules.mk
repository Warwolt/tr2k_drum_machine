# Include module-specific makefiles
-include $(APP_ROOT)/common/makefile.mk

# Presentation
PRESENTATION_DIR = $(APP_ROOT)/presentation
-include $(PRESENTATION_DIR)/tempo_control/tempo_control_view/makefile.mk
-include $(PRESENTATION_DIR)/tempo_control/tempo_knob/makefile.mk

# Application
APPLICATION_DIR = $(APP_ROOT)/application
-include $(APPLICATION_DIR)/rhythm_playback_controller/makefile.mk

# Domain
DOMAIN_DIR = $(APP_ROOT)/domain
-include $(DOMAIN_DIR)/rhythm_playback/beats_per_minute/makefile.mk
-include $(DOMAIN_DIR)/rhythm_playback/rhythm_playback_controller/makefile.mk
-include $(DOMAIN_DIR)/rhythm_playback/tempo_timer/makefile.mk
-include $(DOMAIN_DIR)/rhythm_playback/tempo_timing_manager/makefile.mk

# Infrastructure
INFRASRTUCTURE_DIR = $(APP_ROOT)/infrastructure
-include $(INFRASRTUCTURE_DIR)/interrupts/makefile.mk
-include $(INFRASRTUCTURE_DIR)/startup/makefile.mk

# Hardware Abstraction Layer
HAL_DIR = $(APP_ROOT)/hal
-include $(HAL_DIR)/makefile.mk

# Drivers
DRIVERS_DIR = $(APP_ROOT)/drivers
-include $(DRIVERS_DIR)/charlieplex_matrix/makefile.mk
-include $(DRIVERS_DIR)/gpio_matrix/makefile.mk
-include $(DRIVERS_DIR)/gpio/makefile.mk
-include $(DRIVERS_DIR)/rotary_encoder/makefile.mk
-include $(DRIVERS_DIR)/segment_display/makefile.mk
-include $(DRIVERS_DIR)/spi/makefile.mk
-include $(DRIVERS_DIR)/timer0/makefile.mk
-include $(DRIVERS_DIR)/timer1/makefile.mk
