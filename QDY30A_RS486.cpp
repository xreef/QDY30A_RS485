#include "QDY30A_RS485.h"

void managedDelay(unsigned long timeout) {

	unsigned long t = millis();

	// make darn sure millis() is not about to reach max data type limit and start over
	if (((unsigned long) (t + timeout)) == 0){
		t = 0;
	}

	while ((millis() - t) < timeout) 	{ 	}

}

#ifdef ACTIVATE_SOFTWARE_SERIAL
	QDY30A_RS485::QDY30A_RS485(SoftwareSerial* serial, uint8_t deRePin, uint8_t address) {
		this->ss = serial;

		this->hs = NULL;

		this->dePin = deRePin;

		this->address = address;
	}
	QDY30A_RS485::QDY30A_RS485(uint8_t rxMCUPin, uint8_t txMCUPin, uint8_t deRePin, uint8_t address) {
		this->ss = NULL;

		this->hs = NULL;

		this->dePin = deRePin;

		this->txMCUPin = txMCUPin;
		this->rxMCUPin = rxMCUPin;

		this->address = address;
	}

#endif

QDY30A_RS485::QDY30A_RS485(HardwareSerial* serial, uint8_t deRePin, uint8_t address) {
	#ifdef ACTIVATE_SOFTWARE_SERIAL
		this->ss = NULL;
	#endif

	this->hs = serial;

	this->dePin = deRePin;

	this->address = address;
}
#ifdef HARDWARE_SERIAL_SELECTABLE_PIN
	QDY30A_RS485::QDY30A_RS485(uint8_t rxMCUPin, uint8_t txMCUPin, HardwareSerial* serial, uint8_t deRePin, uint8_t address) {
		#ifdef ACTIVATE_SOFTWARE_SERIAL
			this->ss = NULL;
		#endif

		this->hs = serial;

		this->dePin = deRePin;

		this->txMCUPin = txMCUPin;
		this->rxMCUPin = rxMCUPin;

		this->address = address;
	}
#endif

void QDY30A_RS485::begin(uint32_t baudRate) {
	pinMode(this->dePin, OUTPUT);

    DEBUG_PRINTLN("Begin ex");
    if (this->hs){
        DEBUG_PRINTLN("Begin Hardware Serial");

#ifdef HARDWARE_SERIAL_SELECTABLE_PIN
        if(this->rxMCUPin != -1 && this->txMCUPin != -1) {
			this->serialDef.begin(*this->hs, baudRate, SERIAL_8N1, this->rxMCUPin, this->txMCUPin);
		}else{
			this->serialDef.begin(*this->hs, baudRate, SERIAL_8N1);
		}
#endif
#ifndef HARDWARE_SERIAL_SELECTABLE_PIN
        this->serialDef.begin(*this->hs, baudRate);
#endif
        while (!this->hs) {
          ; // wait for serial port to connect. Needed for native USB
        }

#ifdef ACTIVATE_SOFTWARE_SERIAL
    }else if (this->ss){
        DEBUG_PRINTLN("Begin Software Serial");

		this->serialDef.begin(*this->ss, baudRate);
	}	else{
        DEBUG_PRINTLN("Begin Software Serial Pin");
        SoftwareSerial* mySerial = new SoftwareSerial((int)this->rxMCUPin, (int)this->txMCUPin); // "RX TX" // @suppress("Abstract class cannot be instantiated")
        this->ss = mySerial;

//		SoftwareSerial mySerial(this->rxMCUPin, this->txMCUPin);
        DEBUG_PRINT("RX Pin: ");
        DEBUG_PRINT((int)this->rxMCUPin);
        DEBUG_PRINT("TX Pin: ");
        DEBUG_PRINTLN((int)this->txMCUPin);

		this->serialDef.begin(*this->ss, baudRate);
#endif
	}

    this->serialDef.stream->setTimeout(1000);

    managedDelay(5);
    setLocalPrecisionFromSetting();
    managedDelay(5);
}

uint16_t QDY30A_RS485::crc16_modbus(uint8_t *data, uint16_t length) {
  uint16_t crc = 0xFFFF;
  for (uint16_t i = 0; i < length; i++) {
    crc ^= data[i];
    for (uint8_t j = 8; j != 0; j--) {
      if ((crc & 0x0001) != 0) {
        crc >>= 1;
        crc ^= 0xA001;
      } else {
        crc >>= 1;
      }
    }
  }
  return crc;
}
void writeHex(int n) {
	DEBUG_PRINT("0x");
	DEBUG_PRINT(n < 16 ? "0" : "");
	DEBUG_PRINT(n, HEX);
	DEBUG_PRINT(" ");
}

// Sends a Modbus RTU request frame
void QDY30A_RS485::sendRequest(uint8_t *request, uint8_t requestSize) {
	digitalWrite(dePin, DE_RE_WRITE);
    // Calculate CRC for the request
    uint16_t crc = crc16_modbus(request, requestSize - 2);
    // Append CRC to the request
    request[requestSize - 2] = crc & 0xFF;
    request[requestSize - 1] = (crc >> 8) & 0xFF;
    // Send request through serial
    for (int i = 0 ; i <= 7 ; i++) {
      DEBUG_PRINT("[");
      DEBUG_PRINT(i);
      DEBUG_PRINT("]");
      DEBUG_PRINT("=");
      writeHex(request[i]);
      DEBUG_PRINT(" ");
    }
    DEBUG_PRINTLN();

    for (uint8_t i = 0; i < requestSize; i++) {
        this->serialDef.stream->write(request[i]);
    }
    this->serialDef.stream->flush(); // Ensure data is transmitted

    managedDelay(5);
}

// Reads and validates a Modbus RTU response frame
bool QDY30A_RS485::readResponse(uint8_t *response, uint8_t responseSize) {
	digitalWrite(dePin, DE_RE_READ);

    unsigned long startTime = millis();
    uint8_t bytesRead = 0;
    // Wait for the response with a timeout
    while ((millis() - startTime) < 2000 && bytesRead < responseSize) {
        if (this->serialDef.stream->available()) {
            response[bytesRead++] = (uint8_t)this->serialDef.stream->read();
        }
    }

    for (int i = 0 ; i <= 6 ; i++) {
      DEBUG_PRINT("[");
      DEBUG_PRINT(i);
      DEBUG_PRINT("]");
      DEBUG_PRINT("=");
      writeHex(response[i]);
      DEBUG_PRINT(" ");
    }
    DEBUG_PRINTLN();
    managedDelay(5);


    // Verify response size and CRC
    if (bytesRead == responseSize) {
        uint16_t crc = crc16_modbus(response, responseSize - 2);
        uint16_t responseCrc = response[responseSize - 2] | (response[responseSize - 1] << 8);
        return crc == responseCrc;
    }
    return false; // Response invalid or timed out
}
//void writeHex(int n) {
//	DEBUG_PRINT("0x");
//	DEBUG_PRINT(n < 16 ? "0" : "");
//	DEBUG_PRINT(n, HEX);
//	DEBUG_PRINT(" ");
//}

// Read a register
uint16_t QDY30A_RS485::readRegister(uint16_t regAddr) {
    uint8_t request[] = {
        this->address, // Device address
        FUNCTION_CODE_STATUS_READ, // Function code
        (uint8_t)(regAddr >> 8), (uint8_t)(regAddr & 0xFF), // Register address
        0x00, 0x01, // Number of registers to read
        0x00, 0x00  // Placeholder for CRC
    };
    sendRequest(request, sizeof(request));
    uint8_t response[7]; // Expected response size
    if (readResponse(response, sizeof(response))) {
        // Response format: Addr, Func, ByteCount, Data(H), Data(L), CRC(L), CRC(H)
        return (response[3] << 8) | response[4]; // Combine the high and low bytes of the data
    }
    return 0; // Indicate error or no data
}

// Write to a register
void QDY30A_RS485::writeRegister(uint16_t regAddr, uint16_t value) {
    uint8_t request[] = {
        this->address, // Device address
        FUNCTION_CODE_STATUS_WRITE, // Function code
        (uint8_t)(regAddr >> 8), (uint8_t)(regAddr & 0xFF), // Register address
        (uint8_t)(value >> 8), (uint8_t)(value & 0xFF), // Value to write
        0x00, 0x00  // Placeholder for CRC
    };
    sendRequest(request, sizeof(request));
    uint8_t response[8]; // Expected response size for a write
    readResponse(response, sizeof(response));
    // Error handling or verification of response can be added here
}

// Translate Modbus exception codes to human-readable text
String QDY30A_RS485::getErrorDescription(uint8_t errorCode) {
    switch (errorCode) {
        case 0x01:
            return "Invalid function code";
        case 0x02:
            return "Invalid data address";
        case 0x03:
            return "Invalid data value";
        case 0x04:
            return "Device failure";
        // Add more cases as needed based on your device's specifications
        default:
            return "Unknown error";
    }
}

// Translate unit codes to descriptions
String QDY30A_RS485::getUnitDescription(uint16_t unitCode) {
    switch (unitCode) {
        case 0:
            return "Not displayed";
        case 1:
            return "cm";
        case 2:
            return "mm";
        case 3:
            return "MPa";
        case 4:
            return "Pa";
        case 5:
            return "kPa";
        case 6:
            return "mA";
        // Add more cases if there are more units
        default:
            return "Unknown unit";
    }
}

// Translate precision codes to descriptions
String QDY30A_RS485::getPrecisionDescription(uint16_t precisionCode) {
    switch (precisionCode) {
        case 0:
            return "No decimal";
        case 1:
            return "One decimal place";
        case 2:
            return "Two decimal places";
        case 3:
            return "Three decimal places";
        // Adjust based on the protocol specification if there are more precision formats
        default:
            return "Unknown precision";
    }
}
void QDY30A_RS485::setSlaveAddress(uint8_t newAddress) {
    // Ensure the new address is within a valid range if necessary
    // Example: if (newAddress >= 1 && newAddress <= 247)
    writeRegister(REG_STATUS_ADDRESS, newAddress);
}

void QDY30A_RS485::setBaudRate(BaudRate baudRate) {
    writeRegister(REG_STATUS_BAUDRATE, static_cast<uint16_t>(baudRate));
}

void QDY30A_RS485::setMeasurementUnit(MeasurementUnit unit) {
    writeRegister(REG_STATUS_UNITS, static_cast<uint16_t>(unit));
}
void QDY30A_RS485::setPrecision(Precision precision) {
    writeRegister(REG_STATUS_PRECISION, static_cast<uint16_t>(precision));
}
uint8_t QDY30A_RS485::getSlaveAddress() {
    return static_cast<uint8_t>(readRegister(REG_STATUS_ADDRESS));
}

BaudRate QDY30A_RS485::getBaudRate() {
    uint16_t baudRateValue = readRegister(REG_STATUS_BAUDRATE);
    return static_cast<BaudRate>(baudRateValue);
}
MeasurementUnit QDY30A_RS485::getMeasurementUnit() {
    uint16_t unitValue = readRegister(REG_STATUS_UNITS);
    return static_cast<MeasurementUnit>(unitValue);
}
Precision QDY30A_RS485::getPrecision() {
    uint16_t precisionValue = readRegister(REG_STATUS_PRECISION);
    return static_cast<Precision>(precisionValue);
}
String QDY30A_RS485::getUnitDescription(MeasurementUnit unit) {
    switch (unit) {
        case UNIT_NOT_DISPLAYED:
            return "Not displayed";
        case UNIT_CM:
            return "cm";
        case UNIT_MM:
            return "mm";
        case UNIT_MPA:
            return "MPa";
        case UNIT_PA:
            return "Pa";
        case UNIT_KPA:
            return "kPa";
        case UNIT_MA:
            return "mA";
        default:
            return "Unknown unit";
    }
}
String QDY30A_RS485::getPrecisionDescription(Precision precision) {
    switch (precision) {
        case PRECISION_NO_DECIMAL:
            return "No decimal";
        case PRECISION_ONE_DECIMAL:
            return "One decimal place";
        case PRECISION_TWO_DECIMALS:
            return "Two decimal places";
        case PRECISION_THREE_DECIMALS:
            return "Three decimal places";
        default:
            return "Unknown precision";
    }
}
String QDY30A_RS485::getBaudRateDescription(BaudRate baudRate) {
    switch (baudRate) {
        case BAUD_1200:
            return "1200 bps";
        case BAUD_2400:
            return "2400 bps";
        case BAUD_4800:
            return "4800 bps";
        case BAUD_9600:
            return "9600 bps";
        case BAUD_19200:
            return "19200 bps";
        case BAUD_38400:
            return "38400 bps";
        case BAUD_57600:
            return "57600 bps";
        case BAUD_115200:
            return "115200 bps";
        default:
            return "Unknown baud rate";
    }
}

// Get the current output value of the device
int16_t QDY30A_RS485::getOutputValue() {
    return static_cast<int16_t>(readRegister(REG_STATUS_OUTPUT));
}

// Get the zero scale value of the device
int16_t QDY30A_RS485::getZeroScaleValue() {
    return static_cast<int16_t>(readRegister(REG_STATUS_ZERO));
}

// Get the full scale value of the device
int16_t QDY30A_RS485::getFullScaleValue() {
    return static_cast<int16_t>(readRegister(REG_STATUS_FULL));
}
float QDY30A_RS485::applyPrecision(int16_t rawValue, Precision precision) {
    float scaleFactor = pow(10, static_cast<double>(precision)); // Precision as a scale factor
    return rawValue / scaleFactor;
}
// Get the current output value of the device with precision applied
float QDY30A_RS485::getOutputValueWithPrecision() {
    int16_t rawValue = getOutputValue();
//    Precision precision = getPrecision();
    return applyPrecision(rawValue, _precision);
}

// Get the zero scale value of the device with precision applied
float QDY30A_RS485::getZeroScaleValueWithPrecision() {
    int16_t rawValue = getZeroScaleValue();
//    Precision precision = getPrecision();
    return applyPrecision(rawValue, _precision);
}

// Get the full scale value of the device with precision applied
float QDY30A_RS485::getFullScaleValueWithPrecision() {
    int16_t rawValue = getFullScaleValue();
//    Precision precision = getPrecision();
    return applyPrecision(rawValue, _precision);
}

void QDY30A_RS485::setLocalPrecision(Precision precision) {
	this->_precision = precision;
}

void QDY30A_RS485::setLocalPrecisionFromSetting() {
	 Precision precision = getPrecision();
	 setPrecision(precision);
}
