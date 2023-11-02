# Coding_Challenge
To ingest accelerometer data from a real BLE Tag and detect whether the tag is moving or stationary
Introduction
This repository contains code to parse BLE beacon frames, including accelerometer data and iBeacon frames. Follow the instructions below to compile and run the code.

Instructions
Prerequisites
This code is written in C and should be compiled using a C compiler (e.g., gcc).
Getting Started
Clone the repository to your local machine using the following command:

Change the current directory to the repository folder:

Compiling the Code
Compile the code using a C compiler. Use the following command to compile the code:
Running the Code
Run the compiled program with your sample data. You can provide the sample data as a command-line argument. For example:

The program will parse and display the data from the given BLE beacon frame. The output will include details about the frame components and their values.

Example Output
Field 0: Offset: 0, Length: 1, Data: 02 Field 1: Offset: 1, Length: 1, Data: 01 Field 2: Offset: 2, Length: 1, Data: 06
