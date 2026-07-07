#!/bin/bash

echo Initiating connection to microcontroller through wsl2
sudo modprobe ch341
dmesg | grep tty