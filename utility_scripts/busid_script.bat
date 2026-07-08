@echo off
echo listing devices
usbipd list
echo Attempting to find the connected microcontroller
FOR /F "tokens=2" %%A IN ('usbipd list ^| findstr "1a86:7523"') do (SET VID_PID=%%A) 
IF DEFINED BUSID set "BUSID = %BUSID:"=%"

IF "%VID_PID%"=="" (
    echo [ERROR] is the microcontroller plugged in?
    pause
    exit /b
)
echo Found the VID:PID of the controller to be: %VID_PID%
echo try to unbind if possible first
usbipd unbind -i "%VID_PID%"
echo Binding the BUSID
usbipd bind -i "%VID_PID%"
echo Attaching
usbipd attach --wsl -a -i "%VID_PID%"
echo done
echo will now pause
pause