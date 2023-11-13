# Coding_Challenge
To compile and run the program, you can use the following commands:
gcc BLE_Tag.c -o BLE_Tag -lbluetooth
sudo ./BLE_Tag.c
Make sure you have the BlueZ library installed on your system. Note that the use of 'sudo' might be required to access the Bluetooth device.
Header Files:

The code includes several header files, such as <stdio.h>, <stdlib.h>, <unistd.h>, and Bluetooth-related headers like <bluetooth/bluetooth.h> and <bluetooth/hci.h>. These headers provide functions and definitions used in the program.
Function Declarations:

The code declares three functions: parseAccelerometerData, checkMovementStatus, and main. Function declarations provide information about the functions before their actual implementation.
parseAccelerometerData Function:

This function takes a pointer to BLE packet data as input and extracts accelerometer data. It assumes a certain format for the accelerometer data in the packet and prints the X, Y, and Z axis values in terms of gravity.
checkMovementStatus Function:

This function checks the movement status based on accelerometer data. It assumes that if any of the X, Y, or Z axis values are non-zero, the device is considered to be in motion. It prints whether the device is moving or stationary.
main Function:

The main function is the entry point of the program. It initializes the Bluetooth device, sets up a filter to capture BLE events, and enters a loop to continuously read Bluetooth events.

Inside the loop, the program reads Bluetooth events, extracts information from advertising reports, such as Mac address and RSSI, and checks if the received data corresponds to an accelerometer beacon. If so, it calls the parseAccelerometerData and checkMovementStatus functions
To summarize, this code is designed to read BLE advertising events, extract information from them (Mac address, RSSI), and determine the movement status if the data corresponds to an accelerometer beacon. It serves as a basic framework and may need adjustments based on the actual data format and specifications of your BLE beacons.
