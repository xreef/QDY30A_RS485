/**
 * Example sketch for the QDY30A_RS485 library.
 *
 * Demonstrates initializing the QDY30A level transmitter, reading device configurations,
 * and continuously reading and displaying fluid level values with applied precision.
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

// Initialize the QDY30A_RS485 object
// Assuming Serial1 is used for RS485 communication
QDY30A_RS485 qdy30a(RXD1, TXD1, &Serial1, DE_RE);

// Variables to hold precision and unit description
Precision precision;
String unitDescription;

void setup() {
  Serial.begin(115200); // Start the Serial port for debugging
  while (!Serial) { } // Wait for connection to serial port.

  Serial.println("START!!");
  Serial.println();

  qdy30a.begin(); // Initialize the Modbus device

  // Example of setting measurement unit and precision
  // Uncomment to use
  //qdy30a.setMeasurementUnit(UNIT_CM);
  //Serial.println("Set cm as unit!");
  //qdy30a.setPrecision(PRECISION_ONE_DECIMAL);

  // Reading device configurations
  BaudRate baudRate = qdy30a.getBaudRate();
  MeasurementUnit unit = qdy30a.getMeasurementUnit();
  precision = qdy30a.getPrecision();

  // Getting and displaying the current slave address
  uint8_t slaveAddress = qdy30a.getSlaveAddress();
  Serial.print("Current Slave Address: ");
  Serial.println(slaveAddress);

  // Getting the unit description for display purposes
  unitDescription = qdy30a.getUnitDescription(unit);

  // Output the current device configuration to the Serial Monitor
  Serial.print("Baud Rate: ");
  Serial.println(qdy30a.getBaudRateDescription(baudRate));
  Serial.print("Measurement Unit: ");
  Serial.println(qdy30a.getUnitDescription(unit));
  Serial.print("Precision: ");
  Serial.println(qdy30a.getPrecisionDescription(precision));

  // Reading and displaying initial values with applied precision
  float outputValue = qdy30a.getOutputValueWithPrecision();
  float zeroValue = qdy30a.getZeroScaleValueWithPrecision();
  float fullScaleValue = qdy30a.getFullScaleValueWithPrecision();

  Serial.print("Output Value: ");
  Serial.print(outputValue);
  Serial.println(unitDescription);
  Serial.print("Zero Scale Value: ");
  Serial.print(zeroValue);
  Serial.println(unitDescription);
  Serial.print("Full Scale Value: ");
  Serial.print(fullScaleValue);
  Serial.println(unitDescription);
}

void loop() {
  // Continuously reading and displaying values with applied precision
  float outputValue = qdy30a.getOutputValueWithPrecision();

  Serial.print("Output Value: ");
  Serial.print(outputValue, precision); // Display with number of decimal places based on precision
  Serial.println(unitDescription);

  delay(2000); // Read every 2 seconds
}
