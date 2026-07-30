Small beginner projects that utilizes `avr-gcc` in order to learn bare metal programming for an `ATmega328p` microcontroller.

This project uses `VS Code` but you may use any code editer or IDE. In order to build the project you have to ensure that `avr-gcc` is installed on your machine. If you are not connecting to a microcontroller through WSL or your using a pure windows environment you may have to modify the makefile.

Currently the following have been implemented to some degree:
- [x] Blink led
- [x] UART configuration
- [x] I2C (AVR uses TWI) setup/driver
- [x] LCD display communication driver
- [x] DHT11 temperature and humidity sensor
- [x] DC motor + blink utilizing FreeRTOS to schedule/manage the task
- [x] Including FreeRTOS to be utilized with future features