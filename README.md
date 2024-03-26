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
#include <QDY30A_RS485.h>

QDY30A_RS485 qdy30a(Serial1); // Initialize the library with your RS485 serial port

void setup() {
  Serial.begin(9600);
  Serial1.begin(9600); // Adjust baud rate as needed
  qdy30a.begin(9600); // Initialize QDY30A communication with the same baud rate
}

void loop() {
  float level = qdy30a.getOutputValueWithPrecision(); // Read the fluid level
  Serial.print("Fluid Level: ");
  Serial.println(level);
  delay(1000); // Read every second
}
```