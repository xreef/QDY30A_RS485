# QDY30A_RS485

The QDY30A_RS485 library provides a seamless interface for communicating with the QDY30A level transmitter over an RS485 connection using the Modbus RTU protocol. Designed for ease of use and flexibility, this library is compatible with a wide range of microcontrollers including Arduino, STM32, ESP8266, ESP32, and Raspberry Pi Pico, making it a versatile choice for projects requiring precise fluid level measurement.

## Features

- **Device Configuration**: Easily configure device parameters such as slave address, baud rate, measurement units, and precision.
- **Read Functionality**: Accurately read fluid levels with applied device precision, ensuring reliable data for your applications.
- **Write Functionality**: Modify device settings in real-time to adapt to changing requirements.
- **Broad Compatibility**: Works with Arduino, STM32, ESP8266, ESP32, and Raspberry Pi Pico platforms.
- **Error Handling**: Built-in error handling for robust and reliable operation.

## Installation

### For Arduino

1. Download the latest release of the QDY30A_RS485 library.
2. In the Arduino IDE, navigate to `Sketch` > `Include Library` > `Add .ZIP Library` and select the downloaded file.
3. Include the library in your sketch with `#include <QDY30A_RS485.h>`.

### For Other Platforms

Ensure your development environment is set up for library inclusion, and include the QDY30A_RS485 library in your project according to your platform's standard procedure.

## Usage

Below is a simple example demonstrating how to read the fluid level from a QDY30A device:

```cpp
/**
 * Basic Reading Example for QDY30A_RS485 Library
 *
 * This sketch demonstrates the fundamental operation of reading the fluid level
 * from the QDY30A level transmitter using the QDY30A_RS485 library. It continuously
 * reads the fluid level and prints the value to the Serial Monitor with the precision
 * applied as configured in the device. The sketch is ideal for verifying the functionality
 * of the QDY30A level transmitter and the library's ability to communicate and interpret
 * the readings accurately.
 *
 * The DE/RE pin is controlled to manage the RS485 transceiver's transmit and receive
 * modes, ensuring proper communication over RS485. The sketch assumes the use of
 * Serial1 for RS485 communication, which is common in many Arduino and compatible boards.
 *
 * Author: Renzo Mischianti <renzo.mischianti@gmail.com>
 * Website: https://www.mischianti.org
 */

#include "QDY30A_RS485.h"

// Define the pin used for DE/RE control on the RS485 transceiver
#define DE_RE 16

// Initialize the QDY30A_RS485 object for communication
QDY30A_RS485 qdy30a(&Serial1, DE_RE);

void setup() {
   Serial.begin(115200); // Start the Serial port for debugging

   qdy30a.begin(); // Initialize the QDY30A level transmitter
}

void loop() {
   // Read the current fluid level with the device's configured precision
   float outputValue = qdy30a.getOutputValueWithPrecision();

   // Print the fluid level to the Serial Monitor
   Serial.print("Level: ");
   Serial.println(outputValue);

   delay(1000); // Wait for 1 second before reading the level again
}

```