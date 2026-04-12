#
#             LUFA Library
#     Copyright (C) Dean Camera, 2021.
#
#  dean [at] fourwalledcubicle [dot] com
#           www.lufa-lib.org
#
# --------------------------------------
#         LUFA Project Makefile.
# --------------------------------------

# Run "make help" for target help.

MCU          = atmega32u4
ARCH         = AVR8
BOARD        = USBKEY
F_CPU        = 16000000
F_USB        = $(F_CPU)
OPTIMIZATION = s
OBJDIR       = build
TARGET       = build/firmware
SRC          = src/main.c drivers/gpio/gpio.c src/usb_descriptors.c \
					$(LUFA_SRC_USB) $(LUFA_SRC_USBCLASS)
LUFA_PATH    = libs/lufa/LUFA
CC_FLAGS     = -DUSE_LUFA_CONFIG_HEADER -Idrivers/gpio/ -Iinclude/ -Ilibs/
LD_FLAGS     =

# Default target
all:

# Include LUFA-specific DMBS extension modules
DMBS_LUFA_PATH ?= $(LUFA_PATH)/Build/LUFA
include $(DMBS_LUFA_PATH)/lufa-sources.mk
include $(DMBS_LUFA_PATH)/lufa-gcc.mk

# Include common DMBS build system modules
DMBS_PATH      ?= $(LUFA_PATH)/Build/DMBS/DMBS
include $(DMBS_PATH)/core.mk
#include $(DMBS_PATH)/cppcheck.mk
#include $(DMBS_PATH)/doxygen.mk
#include $(DMBS_PATH)/dfu.mk
include $(DMBS_PATH)/gcc.mk
#include $(DMBS_PATH)/hid.mk
#include $(DMBS_PATH)/avrdude.mk
#include $(DMBS_PATH)/atprogram.mk

cleanall:
	rm -rf $(OBJDIR)
	rm -f $(TARGET)* build/package/*