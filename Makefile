# Makefile for ATmega32U4 keyboard project

# Target microcontroller
MCU = atmega32u4
DEVICE = m32u4

# Toolchain
CC = avr-gcc
LD = avr-ld
OBJCOPY = avr-objcopy
OBJDUMP = avr-objdump
SIZE = avr-size
AVRDUDE = avrdude
INCLUDES = drivers/gpio/
# Compiler flags
CFLAGS = -mmcu=$(MCU) -DF_CPU=16000000UL -Os -Wall -Wextra -ffunction-sections -fdata-sections -I$(INCLUDES)
LDFLAGS = -mmcu=$(MCU) -Wl,--gc-sections -Wl,-Map=$(MAP) -T atmega32u4.ld

# Source files
SRCS = src/main.c startup/startup.c drivers/gpio/gpio.c
OBJS = $(addprefix build/,$(SRCS:.c=.o))

# Output files
BUILD_DIR = build
TARGET = $(BUILD_DIR)/firmware
ELF = $(TARGET).elf
HEX = $(TARGET).hex
MAP = $(BUILD_DIR)/firmware.map

# Default target
all: $(BUILD_DIR) $(HEX) size

# Create build directory
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)/src $(BUILD_DIR)/startup  $(BUILD_DIR)/drivers/gpio

# Compile object files
$(BUILD_DIR)/%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Link ELF
$(ELF): $(OBJS)
#  If you wanted to use the default linker skip -T. You can see the default linker with --verbose
#	$(LD) --verbose $(OBJS) -o $@
	$(LD) -T atmega32u4.ld $(OBJS) -o $@

# Generate hex file
$(HEX): $(ELF)
	$(OBJCOPY) -O ihex -R .eeprom $< $@

# Display size information
size: $(ELF)
	$(SIZE) $<
	@echo ""
	@echo "Sections:"
	@$(OBJDUMP) -h $<

# Flash to board via serial port
flash: $(HEX)
	$(AVRDUDE) -p $(DEVICE) -c avr109 -P /dev/ttyACM0 -b 57600 -U flash:w:$<:i

# Clean build artifacts
clean:
	rm -rf $(BUILD_DIR)

.PHONY: all clean size flash
