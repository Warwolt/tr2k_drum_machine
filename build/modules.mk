# Include module-specific makefiles
-include $(APP_ROOT)/common/makefile.mk

# Presentation
PRESENTATION_DIR = $(APP_ROOT)/presentation
-include $(PRESENTATION_DIR)/TempoControl/TempoControlView/makefile.mk
-include $(PRESENTATION_DIR)/TempoControl/TempoKnob/makefile.mk

# Application
APPLICATION_DIR = $(APP_ROOT)/application
-include $(APPLICATION_DIR)/RhythmPlaybackController/makefile.mk

# Domain
DOMAIN_DIR = $(APP_ROOT)/domain
-include $(DOMAIN_DIR)/RhythmPlayback/BeatsPerMinute/makefile.mk
-include $(DOMAIN_DIR)/RhythmPlayback/TempoTimer/makefile.mk
-include $(DOMAIN_DIR)/RhythmPlayback/TempoTimingManager/makefile.mk

# Infrastructure
INFRASRTUCTURE_DIR = $(APP_ROOT)/infrastructure
-include $(INFRASRTUCTURE_DIR)/CallbackScheduler/makefile.mk
-include $(INFRASRTUCTURE_DIR)/Interrupts/makefile.mk
-include $(INFRASRTUCTURE_DIR)/startup/makefile.mk

# Hardware Abstraction Layer
HAL_DIR = $(APP_ROOT)/hal
-include $(HAL_DIR)/makefile.mk

# Drivers
DRIVERS_DIR = $(APP_ROOT)/drivers
-include $(DRIVERS_DIR)/CharlieplexMatrix/makefile.mk
-include $(DRIVERS_DIR)/GpioMatrix/makefile.mk
-include $(DRIVERS_DIR)/GpioPin/makefile.mk
-include $(DRIVERS_DIR)/RotaryEncoder/makefile.mk
-include $(DRIVERS_DIR)/SegmentDisplay/makefile.mk
-include $(DRIVERS_DIR)/Spi/makefile.mk
-include $(DRIVERS_DIR)/Timer0/makefile.mk
-include $(DRIVElRS_DIR)/Timer1/makefile.mk
