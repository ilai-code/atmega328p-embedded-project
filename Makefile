# In admin cmd on windows
# usbipd list
# usbipd bind --busid X-Y
# usbipd attach --wsl --busid X-Y
# sudo modprobe ch341
# For displaying kernel messages of serial devices: dmesg | grep tty
# lsusb for showing the connected devices
# ls /dev/tty*
# Using screen to listen to UART 
# Kill a screen task by sudo screen -X -S [PID] quit
# ctrl + a -> d to detach the screen
# sudo cat work the same way as screen so it might be better to just stick with the cat option
# sudo screen -S [name i.e uart_monitor] sh -c "cat /dev/ttyUSB0"
# set up the serial port
# sudo stty -F /dev/ttyUSB0 [baud rate] [extra flags if necessary]

MCU = atmega328p
F_CPU = 16000000UL
PROGRAMMER = arduino

CC = avr-gcc
OBJCOPY = avr-objcopy
CFLAGS = -Wall -Os -MMD -MP -mmcu=$(MCU) -DF_CPU=$(F_CPU) -Iinclude \
		 -Iinclude/freeRTOS_library/include -Iinclude/freeRTOS_library/portable/GCC/ATMega323 \
		 -Isrc/drivers 
LDFLAGS = -mmcu=$(MCU)

# the port that your microcontroller is connected to
PORT = /dev/ttyUSB0
# the baud rate needed to flash the microcontroller
BAUD = 115200

SRCS = src/main.c src/drivers/uart.c src/drivers/lcd.c src/drivers/twi_functions.c src/drivers/dht.c include/freeRTOS_library/portable/MemMang/heap_1.c
SRCS += include/freeRTOS_library/tasks.c include/freeRTOS_library/queue.c 
SRCS += include/freeRTOS_library/list.c include/freeRTOS_library/portable/GCC/ATMega323/port.c
OBJS = $(SRCS:.c=.o)
# using the object files makes it more useable in the event more than 1 programming language is used the .d files will still appear correctly
DEPS = $(OBJS:.o=.d)

#use PHONY to tell make that these are not files but commands/actions
.PHONY: clean flash uart uart_kill loadDevice

all: main.hex

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

main.out: $(OBJS)
	$(CC) $(LDFLAGS) -o main.out $(OBJS)

main.hex: main.out
	$(OBJCOPY) -O ihex -j .text -j .data main.out main.hex

-include $(DEPS)

flash:
	sudo avrdude -c $(PROGRAMMER) -p m328p -P $(PORT) -b $(BAUD) -D -U flash:w:main.hex:i
	make clean

uart:
	cmd.exe /C wt --title "UART_MONITOR" wsl -e bash -c "sudo stty -F /dev/ttyUSB0 9600 && sudo cat /dev/ttyUSB0"

uart_kill:
	sudo screen -S uart_monitor -X quit

loadDevice:
	./utility_scripts/wsl_connection_script.sh

clean:
	rm -rf *.out *.hex $(OBJS) $(DEPS)