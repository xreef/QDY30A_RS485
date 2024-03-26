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
