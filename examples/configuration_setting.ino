/**
 * Configuration Example for QDY30A_RS485 Library
 *
 * This sketch demonstrates how to configure the QDY30A level transmitter
 * to use a specific measurement unit and precision. The settings applied
 * include setting the measurement unit to centimeters and the precision
 * to two decimal places.
 *
 * The QDY30A_RS485 library facilitates communication with the QDY30A
 * level transmitter over RS485, using the Modbus RTU protocol. This example
 * is part of a collection of sketches that illustrate various functionalities
 * of the QDY30A_RS485 library.
 *
 * Author: Renzo Mischianti <renzo.mischianti@gmail.com>
 * Website: https://www.mischianti.org
 */

#include "QDY30A_RS485.h"

// Define the pin used to control DE/RE on RS485 transceiver
#define DE_RE 16

// Initialize the QDY30A_RS485 library instance
// Serial1 is typically used for RS485 communication on many Arduino-compatible boards
QDY30A_RS485 qdy30a(&Serial1, DE_RE);

void setup() {
  Serial.begin(115200); // Start the Serial port for debugging purposes

  qdy30a.begin(); // Initialize the Modbus device

  // Configure the device to use centimeters for measurements
  // and to represent values with two decimal places of precision
  qdy30a.setMeasurementUnit(UNIT_CM);
  qdy30a.setPrecision(PRECISION_TWO_DECIMALS);

  Serial.println("Device configured to CM with two decimal precision.");
}

void loop() {
  // With the device configured, you can now proceed to read or write values
  // as needed. This example stops here, but typically you would include
  // measurement reading and handling logic in the loop.

  // Note: This example does not include a delay in the loop.
  // In a real application, ensure there's a delay or other non-blocking
  // logic to prevent continuous and rapid firing of commands to the device.
}
