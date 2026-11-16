/**
 * QDY30A_RS485.h - Library Header File
 *
 * The QDY30A_RS485 library provides an easy-to-use interface for communicating with
 * the QDY30A RS485 level transmitter. It simplifies the process of reading fluid levels
 * and configuring the device over an RS485 connection using the Modbus RTU protocol.
 * Designed to be compatible with a wide array of microcontrollers that support Serial
 * communication, including Arduino, ESP8266, ESP32, STM32, and Raspberry Pi Pico.
 *
 * This library allows users to quickly integrate the QDY30A level transmitter into their
 * projects, providing functions to read fluid levels with precision, set and get device
 * configuration such as measurement units and precision, and handle communication
 * errors gracefully.
 *
 * Key Features:
 * - Easy configuration and reading of the QDY30A level transmitter
 * - Support for Modbus RTU protocol over RS485
 * - Compatible with Arduino and other microcontrollers with Serial support
 * - Functions to set measurement units and data precision
 * - Utility functions for error handling and converting enumerations to descriptive text
 *
 * Version: 1.0.0
 * Author: Renzo Mischianti <renzo.mischianti@gmail.com>
 * Website: https://www.mischianti.org
 *
 * The development of this library was inspired by the need for a straightforward way to
 * interface with the QDY30A transmitter in various IoT and automation projects, focusing
 * on ease of use and reliability.
 *
 * License: MIT
 *
 * For more information, examples, and API documentation, visit the GitHub repository:
 * https://github.com/xreef/QDY30A_RS485
 */

#ifndef QDY30A_RS485_h
#define QDY30A_RS485_h

// #define QDY30AB
#define QDY30A


#include "Arduino.h"
#if !defined(ARDUINO_ARCH_STM32) && !defined(ESP32) && !defined(ARDUINO_ARCH_SAMD) && !defined(ARDUINO_ARCH_MBED) && !defined(__STM32F1__) && !defined(__STM32F4__)
	#define ACTIVATE_SOFTWARE_SERIAL
#endif
#if defined(ESP32)
	#define HARDWARE_SERIAL_SELECTABLE_PIN
#endif

#ifdef ACTIVATE_SOFTWARE_SERIAL
	#include <SoftwareSerial.h>
#endif
// Uncomment to enable printing out nice debug messages.
//#define QDY30A_RS485_DEBUG

// Define where debug output will be printed.
#define DEBUG_PRINTER Serial

// Setup debug printing macros.
#ifdef QDY30A_RS485_DEBUG
	#define DEBUG_PRINT(...) { DEBUG_PRINTER.print(__VA_ARGS__); }
	#define DEBUG_PRINTLN(...) { DEBUG_PRINTER.println(__VA_ARGS__); }
#else
	#define DEBUG_PRINT(...) {}
	#define DEBUG_PRINTLN(...) {}
#endif

#ifndef QDY30AB
	#define ADD_DELAY_AFTER_SEND_REQUEST
#endif

// Define register addresses
enum RegisterAddress_QDY30A {
    REG_STATUS_ADDRESS = 0x0000,
    REG_STATUS_BAUDRATE,
    REG_STATUS_UNITS,
    REG_STATUS_PRECISION,
    REG_STATUS_OUTPUT,
    REG_STATUS_ZERO,
    REG_STATUS_FULL
};

// Define baud rate settings
enum BaudRate_QDY30A {
    BAUD_1200 = 0,
    BAUD_2400,
    BAUD_4800,
    BAUD_9600,
    BAUD_19200,
    BAUD_38400,
    BAUD_57600,
    BAUD_115200
};

#ifndef QDY30AB

// Define units of measurement
enum MeasurementUnit_QDY30A {
    UNIT_NOT_DISPLAYED = 0,
    UNIT_CM,
    UNIT_MM,
    UNIT_MPA,
    UNIT_PA,
    UNIT_KPA,
    UNIT_MA
};
#endif

#ifdef QDY30AB

// Define units of measurement
enum MeasurementUnit_QDY30A {
//    UNIT_MPA      = 0,   // Megapascal
	UNIT_NOT_DISPLAYED      = 0,   // Megapascal
    UNIT_KPA      = 1,   // Kilopascal
    UNIT_PA       = 2,   // Pascal
    UNIT_BAR      = 3,   // Bar
    UNIT_MBAR     = 4,   // Millibar
    UNIT_KGCM2    = 5,   // Kilogram-force per cm²
    UNIT_PSI      = 6,   // Pounds per square inch
    UNIT_MH2O     = 7,   // Meter of water column
    UNIT_MMH2O    = 8,   // Millimeter of water column
    UNIT_INH2O    = 9,   // Inch of water column
    UNIT_H2O      = 10,  // Water column (generic)
    UNIT_MHG      = 11,  // Meter of mercury column
    UNIT_MMHG     = 12,  // Millimeter of mercury column
    UNIT_INHG     = 13,  // Inch of mercury column
    UNIT_ATM      = 14,  // Standard atmosphere
    UNIT_TORR     = 15,  // Torr
    UNIT_M        = 16,  // Meter
    UNIT_CM       = 17,  // Centimeter
    UNIT_MM       = 18,  // Millimeter
    UNIT_KG       = 19,  // Kilogram
    UNIT_DEG_C    = 20,  // Degree Celsius
    UNIT_PH       = 21,  // pH
    UNIT_DEG_F    = 22,  // Degree Fahrenheit
    UNIT_EMPTY    = 23   // Not displayed / empty
};
#endif

// Define precision settings
enum Precision_QDY30A {
    PRECISION_NO_DECIMAL = 0,
    PRECISION_ONE_DECIMAL,
    PRECISION_TWO_DECIMALS,
    PRECISION_THREE_DECIMALS
};
#define FUNCTION_CODE_STATUS_READ    0x03
#define FUNCTION_CODE_STATUS_WRITE   0x06

#define DE_RE_READ    LOW
#define DE_RE_WRITE   HIGH


class QDY30A_RS485 {
  public:
    QDY30A_RS485(HardwareSerial* serial, uint8_t deRePin, uint8_t address = 0x01);
#ifdef ACTIVATE_SOFTWARE_SERIAL
    QDY30A_RS485(SoftwareSerial* serial, uint8_t deRePin, uint8_t address = 0x01);
    QDY30A_RS485(uint8_t rxMCUPin, uint8_t txMCUPin, uint8_t deRePin, uint8_t address = 0x01);
#endif

#ifdef HARDWARE_SERIAL_SELECTABLE_PIN
    QDY30A_RS485(uint8_t rxMCUPin, uint8_t txMCUPin, HardwareSerial* serial, uint8_t deRePin, uint8_t address = 0x01);
#endif
    void begin(uint32_t baudRate = 9600);
    uint16_t readRegister(uint16_t regAddr);
    void writeRegister(uint16_t regAddr, uint16_t value);
    String getErrorDescription(uint8_t errorCode);
    String getUnitDescription(uint16_t unitCode);
    String getPrecisionDescription(uint16_t precisionCode);

    String getBaudRateDescription(BaudRate_QDY30A baudRate);
    String getUnitDescription(MeasurementUnit_QDY30A unit);
    String getPrecisionDescription(Precision_QDY30A precision);

    void setBaudRate(BaudRate_QDY30A baudRate);
    void setMeasurementUnit(MeasurementUnit_QDY30A unit);
    void setPrecision(Precision_QDY30A precision);

    BaudRate_QDY30A getBaudRate();
    MeasurementUnit_QDY30A getMeasurementUnit();
    Precision_QDY30A getPrecision();

    int16_t getOutputValue();
    int16_t getZeroScaleValue();
    int16_t getFullScaleValue();

    float getOutputValueWithPrecision();
    float getZeroScaleValueWithPrecision();
    float getFullScaleValueWithPrecision();
    void setSlaveAddress(uint8_t newAddress);
    uint8_t getSlaveAddress();

    void setLocalPrecisionFromSetting();
    void setLocalPrecision(Precision_QDY30A precision);

    String getDeviceType() {
#ifdef QDY30AB
        return "QDY30A-B";
#else
        return "QDY30A";
#endif
    }
  private:
	HardwareSerial* hs;

#ifdef ACTIVATE_SOFTWARE_SERIAL
	SoftwareSerial* ss;
#endif

	struct NeedsStream {
		template<typename T>
		void begin(T &t, uint32_t baud) {
			//DEBUG_PRINTLN("Begin ");
			t.setTimeout(500);
			t.begin(baud);
			stream = &t;
		}

#ifdef HARDWARE_SERIAL_SELECTABLE_PIN
//		  template< typename T >
//		  void begin( T &t, int baud, SerialConfig config ){
//			  DEBUG_PRINTLN("Begin ");
//			  t.setTimeout(500);
//			  t.begin(baud, config);
//			  stream = &t;
//		  }
//
		template< typename T >
		void begin( T &t, uint32_t baud, SerialConfig config ) {
			DEBUG_PRINTLN("Begin ");
			t.setTimeout(500);
			t.begin(baud, config);
			stream = &t;
		}

		template< typename T >
		void begin( T &t, uint32_t baud, SerialConfig config, int8_t rxMCUPin, int8_t txMCUPin ) {
			DEBUG_PRINTLN("Begin ");
			t.setTimeout(500);
			t.begin(baud, config, rxMCUPin, txMCUPin);
			stream = &t;
		}
#endif

		void listen() {}

		Stream *stream;
	};
	NeedsStream serialDef;

    uint16_t crc16_modbus(uint8_t *data, uint16_t length);
    void sendRequest(uint8_t *request, uint8_t requestSize);
    bool readResponse(uint8_t *response, uint8_t responseSize);

    float applyPrecision(int16_t rawValue, Precision_QDY30A precision);
    int8_t rxMCUPin = -1;
    int8_t txMCUPin = -1;
    int8_t dePin = -1;

    uint8_t address = 0x01;

    Precision_QDY30A _precision = PRECISION_ONE_DECIMAL;

    void managedDelay(unsigned long timeout);
    void writeHex(int n);
};

#endif
