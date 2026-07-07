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
# sudo screen -S [name i.e uart_monitor] sh -c "cat /dev/ttyUSB0"
# set up the serial port
# sudo stty -F /dev/ttyUSB0 [baud rate] [extra flags if necessary]

MCU = atmega328p
F_CPU = 16000000UL
PROGRAMMER = arduino

CC = avr-gcc
OBJCOPY = avr-objcopy
CFLAGS = -Wall -Os -mmcu=$(MCU) -DF_CPU=$(F_CPU) -Iinclude

PORT = /dev/ttyUSB0
BAUD = 115200

OBJ = src/main.o src/drivers/delay.o

all: main.hex

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

main.out: $(OBJ)
	$(CC) $(CFLAGS) -o main.out $(OBJ)

main.hex: main.out
	$(OBJCOPY) -O ihex -j .text -j .data main.out main.hex

flash:
	sudo avrdude -c $(PROGRAMMER) -p m328p -P $(PORT) -b $(BAUD) -D -U flash:w:main.hex:i
	make clean

configure_uart:
	sudo stty -F /dev/ttyUSB0 9600

uart:
	cmd.exe /C wt --title "UART_MONITOR" wsl -e bash -c "sudo screen -S uart_monitor sh -c 'sudo cat /dev/ttyUSB0'"
uart_kill:
	sudo screen -S uart_monitor -X quit

clean:
	rm -rf *.out *.hex *.o src/*.o src/drivers/*.o
