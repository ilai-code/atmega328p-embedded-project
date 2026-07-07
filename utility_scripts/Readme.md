⚠️**Note: This is an experimental hobby project and I am definitely still tweaking things. Make sure to review the scripts and your wiring before plugging anything in, just to be safe.**

The utility scripts here are to help you connect a microcontroller to your WSL environment so that you can build and code the project in WSL.

The busid_script.bat is a windows script that utilizes usbipd to find and connect to the microcontroller. Use `usbipd list` to find the microcontroller and identify the VID:PID.

Always run the bat script first before running the linux script to attach the microcontroller. You may have to run the bat script as admin or give the machine some time to detect the device before running. 
