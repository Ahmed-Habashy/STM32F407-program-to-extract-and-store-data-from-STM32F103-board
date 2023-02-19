# STM32F407-program-to-extract-and-store-data-from-STM32F103-board
This project extracts and stores data from an STM32F103 board connected to an STM32F407 board via USB. The program uses the STM32F407 board as the USB host to read data from the STM32F103 board and store it in a file on a USB flash drive. 

# Table of Contents
Overview
Hardware Required
Software Required
Setup Instructions
Usage
References

# Overview
This project extracts and stores data from an STM32F103 board connected to an STM32F407 board via USB. The program uses the STM32F407 board as the USB host to read data from the STM32F103 board and store it in a file on a USB flash drive. The program can also write data to the STM32F103 board.

# Hardware Required
STM32F407 board
STM32F103 board
USB cable
USB flash drive
Software Required
STM32CubeIDE
FatFs library
USB Host library
Setup Instructions
Connect the STM32F407 board to the STM32F103 board via USB.
Connect a USB flash drive to the STM32F407 board.
Open the project in STM32CubeIDE.
Build and flash the code onto the STM32F407 board.

# Usage
Press the button on the STM32F407 board or enter a correct password to read data from the STM32F103 board and store it in a file on the USB flash drive.
The program can also write data to the STM32F103 board.
The LEDs on the STM32F407 board indicate the status of the USB communication.

# Functions
UsbTest_Write()
This function writes data to the STM32F103 board using UART communication. It returns a boolean value that indicates whether the write operation was successful or not.

UsbTest_Read()
This function reads data from the STM32F103 board using a USB connection and writes it to a file on an external USB storage device. It returns a boolean value that indicates whether the read operation was successful or not.

Variables
myUsbFatFS
This variable is of the FATFS type and represents the file system on the external USB storage device.

USBHPath
This variable is a character array that represents the logical path of the USB storage device.

myFile
This variable is of the FIL type and represents the file that data is written to on the USB storage device.

res
This variable is of the FRESULT type and represents the result of a file system operation.

byteswritten
This variable is of the UINT type and represents the number of bytes that were written to a file.

bytesread
This variable is of the UINT type and represents the number of bytes that were read from a file.

rxIMU
This variable is a byte array that represents the data that is read from the STM32F103 board.

usb_buffer
This variable is a character array that represents the data that is written to the USB storage device.

wifi_code
This variable is a character array that represents the code that is written to the STM32F103 board using UART communication.

write_flag
This variable is a boolean that is set to true when data is written to the STM32F103 board using UART communication.

byte_check
This variable is a boolean that is set to true when data is read from the STM32F103 board using a USB connection.

read_flag
This variable is a boolean that is set to true when data is successfully read from the STM32F103 board and written to the USB storage device.

pass_correct
This variable is a boolean that is set to true when the correct password is entered.

file_name
This variable is a character array that represents the name of the file that data is written

# References
STM32F407 Datasheet
STM32F103 Datasheet
FatFs Documentation
USB Host Library
