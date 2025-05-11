# Makefile for the SDL Access Control System
# Change these values according to your MCU and project settings

# MCU name
MCU = atmega32
F_CPU = 16000000UL

# Object files
OBJ = main.o password.o door.o people_counter.o user_interface.o \
      ../lib/LCD/LCD.o ../lib/KEYPAD/KEYPAD.o ../lib/EEPROM/EEPROM.o ../lib/SERVO/SERVO.o

# Compiler and flags
CC = avr-gcc
CFLAGS = -mmcu=$(MCU) -Wall -Os -DF_CPU=$(F_CPU)

# Linker
LDFLAGS = -mmcu=$(MCU)

# Target file
TARGET = sdl_access_control

# Programmer settings (for avrdude)
PROGRAMMER = usbasp
PORT = usb

# Default target
all: $(TARGET).hex

# Compile .c files to .o
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Link object files to create .elf file
$(TARGET).elf: $(OBJ)
	$(CC) $(LDFLAGS) -o $@ $^

# Generate hex file
$(TARGET).hex: $(TARGET).elf
	avr-objcopy -O ihex -R .eeprom $< $@

# Flash the microcontroller
flash: $(TARGET).hex
	avrdude -p $(MCU) -c $(PROGRAMMER) -P $(PORT) -U flash:w:$(TARGET).hex

# Clean up
clean:
	rm -f *.o *.elf *.hex