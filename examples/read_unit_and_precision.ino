/**
 * Simplified example sketch for the QDY30A_RS485 library.
 *
 * Demonstrates how to read the fluid level from the QDY30A level transmitter
 * and display it with the measurement unit and precision applied. This version
 * has been streamlined to focus on the core functionality of reading and displaying
 * the fluid level.
 *
 * Author: Renzo Mischianti <renzo.mischianti@gmail.com>
 * Website: https://www.mischianti.org
 */

#include "Arduino.h"
#include "QDY30A_RS485.h"

// Define pins for RS485 module
#define RXD1 18
#define TXD1 17
#define DE_RE 16

// Initialize the QDY30A_RS485 object for RS485 communication
QDY30A_RS485 qdy30a(RXD1, TXD1, &Serial1, DE_RE);

// Variables to store the measurement unit and precision
String unitDescription;
Precision_QDY30A precision;

void displayReadings() {
  // Read the fluid level with precision applied
  float outputValue = qdy30a.getOutputValueWithPrecision();

  // Display the fluid level with the appropriate unit and precision
  Serial.print("Output Value: ");
  Serial.print(outputValue, static_cast<int>(precision)); // Display with specified precision
  Serial.println(unitDescription);
}

void setup() {
  Serial.begin(115200); // Start the Serial port for debugging
  while (!Serial) { } // Wait for the serial port to connect. Necessary for native USB

  Serial.println("START - QDY30A Fluid Level Reading");

  qdy30a.begin(); // Initialize the Modbus device

  // Read current measurement unit and precision from the device
  MeasurementUnit_QDY30A unit = qdy30a.getMeasurementUnit();
  precision = qdy30a.getPrecision();

  // Obtain and store the unit description for display purposes
  unitDescription = qdy30a.getUnitDescription(unit);

  // Display the device configuration for reference
  Serial.print("Measurement Unit: ");
  Serial.println(unitDescription);
  Serial.print("Precision: ");
  Serial.println(qdy30a.getPrecisionDescription(precision));

  // Perform an initial reading and display
  displayReadings();
}

void loop() {
  // Continuously read and display the fluid level with the unit and precision applied
  displayReadings();
  delay(2000); // Refresh every 2 seconds
}

