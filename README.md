# QDY30A_RS485 Library v1.0.2

![QDY30A_RS485 Logo](https://raw.githubusercontent.com/xreef/QDY30A_RS485/master/resources/QDY30A_RS485Logo.png)

[![arduino-library-badge](https://www.ardu-badge.com/badge/QDY30A_RS485.svg?)](https://www.ardu-badge.com/QDY30A_RS485)
[![](https://img.shields.io/badge/Platform-Arduino%20%7C%20ESP32%20%7C%20ESP8266%20%7C%20RP2040%20%7C%20STM32%20%7C%20SAMD-green.svg)]()
[![](https://img.shields.io/badge/Protocol-Modbus%20RTU%20over%20RS485-blue.svg)]()
[![](https://img.shields.io/badge/License-MIT-lightgrey.svg)](LICENSE.md)

An Arduino library to read and configure the **QDY30A** submersible level transmitter over **RS485** with the
**Modbus RTU** protocol: level with the precision set on the device, zero and full scale, unit, decimals, baud rate and
slave address. The QDY30A-B variant (24 units) is supported too.

**Author:** Renzo Mischianti
**Website:** [www.mischianti.org](https://www.mischianti.org)
**GitHub:** [xreef/QDY30A_RS485](https://github.com/xreef/QDY30A_RS485)

---

## 📚 Documentation & Tutorials

Complete tutorials on [mischianti.org](https://www.mischianti.org):

- 📖 **[QDY30A RS485 water level sensor with ESP32-S3, Arduino UNO and Raspberry Pi Pico](https://mischianti.org/qdy30a-rs485-water-level-sensor-esp32-arduino-pico/)** - 24V supply, MAX3485/MAX485 wiring, library usage and complete sketch
- 🐍 **[QDY30A RS485 liquid level sensor with MicroPython on ESP32-S3 and Raspberry Pi Pico](https://mischianti.org/qdy30a-rs485-liquid-level-micropython-esp32-pico/)** - the same sensor with the [MicroPython library](https://github.com/xreef/qdy30a-rs485-micropython)
- 💬 🇺🇸 **[Mischianti's libraries forum](https://mischianti.org/forums/forum/mischiantis-libraries/)** - questions, bugs and suggestions
- 💬 🇮🇹 **[Forum delle librerie di Mischianti](https://mischianti.org/it/forums/forum/le-librerie-di-mischianti/)** - domande, bug e suggerimenti

---

## 📋 Table of Contents
- [Features](#-features)
- [Supported Platforms](#-supported-platforms)
- [Wiring](#-wiring)
- [Installation](#-installation)
- [Basic Usage](#-basic-usage)
- [API](#-api)
- [Configuration](#-configuration)
- [Included Examples](#-included-examples)
- [Changelog](#-changelog)
- [License](#-license)
- [Support & Contact](#-support--contact)

## ✨ Features

- Read the level already scaled with the precision stored in the transmitter (`getOutputValueWithPrecision()`)
- Read zero scale and full scale, unit, decimals, baud rate and slave address
- Write unit, decimals, baud rate and slave address
- Direction control of the RS485 transceiver with one `DE/RE` pin
- Hardware serial on every platform, selectable RX/TX pins on ESP32, SoftwareSerial where it exists
- Error descriptions for Modbus exceptions, optional debug output

## 🎯 Supported Platforms

| Platform | Serial | Notes |
|---|---|---|
| ESP32 / ESP32-S3 / ESP32-C3 | HardwareSerial, RX/TX pins selectable in the constructor | Use a 3.3V transceiver (MAX3485) |
| ESP8266 | SoftwareSerial or HardwareSerial | 3.3V transceiver |
| Raspberry Pi Pico (RP2040) | `Serial1` / `Serial2` | 3.3V transceiver: the RP2040 is not 5V tolerant |
| Arduino UNO, Nano, Mega (AVR) | SoftwareSerial (or `Serial1` on the Mega) | 5V transceiver (MAX485) |
| Arduino SAMD, STM32, mbed | HardwareSerial | 3.3V transceiver |

The QDY30A needs **24V**: a step-up converter (for example an MT3608) set to 24V before you connect the probe.

## 🔌 Wiring

| Signal | ESP32-S3 | Arduino UNO | Raspberry Pi Pico | Transceiver |
|---|---|---|---|---|
| RX | GPIO18 | 10 | GPIO9 | RO |
| TX | GPIO17 | 11 | GPIO8 | DI |
| Direction | GPIO16 | 5 | GPIO10 | DE + RE |
| Supply | 3.3V | 5V | 3.3V | VCC |

| QDY30A wire | Connect to |
|---|---|
| Red | +24V (step-up VOUT+) |
| Green | GND (step-up VOUT-, common ground) |
| Yellow | B |
| Blue | A |

Add a 120Ω resistor across A and B at the end of the line. Diagrams and photos are in the
[tutorial](https://mischianti.org/qdy30a-rs485-water-level-sensor-esp32-arduino-pico/).

## 📦 Installation

### Arduino IDE (Library Manager)
1. Open **Sketch > Include Library > Manage Libraries**.
2. Search for **QDY30A_RS485** and install it.

### Manual (ZIP)
1. Download the latest release from [GitHub](https://github.com/xreef/QDY30A_RS485/releases).
2. **Sketch > Include Library > Add .ZIP Library** and select the file.

### PlatformIO
```ini
lib_deps = https://github.com/xreef/QDY30A_RS485.git#1.0.2
```

## 🚀 Basic Usage

```cpp
#include "QDY30A_RS485.h"

#define RXD1 18   // RO of the transceiver
#define TXD1 17   // DI of the transceiver
#define DE_RE 16  // DE and RE of the transceiver

// ESP32: Serial1 on custom pins
QDY30A_RS485 qdy30a(RXD1, TXD1, &Serial1, DE_RE);

void setup() {
  Serial.begin(115200);
  qdy30a.begin();  // 9600 baud, the default of the QDY30A
}

void loop() {
  float level = qdy30a.getOutputValueWithPrecision();
  MeasurementUnit_QDY30A unit = qdy30a.getMeasurementUnit();
  Serial.print("Level: ");
  Serial.print(level);
  Serial.println(qdy30a.getUnitDescription(unit));
  delay(1000);
}
```

Other constructors:

```cpp
QDY30A_RS485 qdy30a(&Serial2, DE_RE);          // HardwareSerial on its default pins (e.g. Pico Serial2: GPIO8/9)
QDY30A_RS485 qdy30a(10, 11, DE_RE);            // SoftwareSerial (AVR, ESP8266): RX, TX, DE/RE
QDY30A_RS485 qdy30a(&mySoftwareSerial, DE_RE); // an existing SoftwareSerial
// every constructor accepts the slave address as last parameter (default 0x01)
```

## 📖 API

| Method | Description |
|---|---|
| `begin(baud = 9600)` | Start the serial port and the DE/RE pin, read the precision stored in the device |
| `getOutputValueWithPrecision()` | Level with the device precision applied |
| `getZeroScaleValueWithPrecision()`, `getFullScaleValueWithPrecision()` | Range of the transmitter |
| `getMeasurementUnit()`, `setMeasurementUnit(unit)` | Unit (`UNIT_CM`, `UNIT_MM`, `UNIT_MPA`, `UNIT_PA`, `UNIT_KPA`, `UNIT_MA`) |
| `getPrecision()`, `setPrecision(p)` | Decimals (`PRECISION_NO_DECIMAL` … `PRECISION_THREE_DECIMALS`) |
| `getBaudRate()`, `setBaudRate(b)` | Baud rate of the device (`BAUD_1200` … `BAUD_115200`) |
| `getSlaveAddress()`, `setSlaveAddress(a)` | Modbus slave address |
| `getUnitDescription()`, `getPrecisionDescription()`, `getBaudRateDescription()` | Human readable text |
| `readRegister(reg)`, `writeRegister(reg, value)` | Raw Modbus register access |
| `getErrorDescription(code)` | Text of a Modbus exception code |
| `getDeviceType()` | `"QDY30A"` or `"QDY30A-B"` |

## ⚙️ Configuration

In `QDY30A_RS485.h`:

- `#define QDY30A` (default) or `#define QDY30AB` for the QDY30A-B variant, which has a different unit table
  (kPa, bar, mH₂O, °C, …) and does not need the pause after each request.
- `#define QDY30A_RS485_DEBUG` to print the Modbus frames on `Serial`.

## 🧪 Included Examples

| Example | What it does |
|---|---|
| `basic_redings` | Reads the level every second |
| `configuration_setting` | Writes unit and decimals |
| `read_unit_and_precision` | Reads unit and precision and prints them |
| `qdy30a_read_all_and_apply_precision` | Reads the whole configuration, then the level in a loop |

## 📝 Changelog

- **2026-11-16 v1.0.2**: README with documentation links, logo and badges; `keywords.txt` completed; source file renamed
  `QDY30A_RS485.cpp` (was `QDY30A_RS486.cpp`); `library.json` homepage fixed.
- **2026-11-16 v1.0.1**: QDY30A-B variant (`#define QDY30AB`), `getDeviceType()`, `sendRequest()` debug output fixed
  (it always printed 8 bytes), duplicate functions removed, examples updated to the `_QDY30A` type names.
- **2024-08-19 v1.0.0**: first production release.

## 📄 License

MIT License, see [LICENSE.md](LICENSE.md).

## 🤝 Support & Contact

- 🌐 Website: [www.mischianti.org](https://www.mischianti.org)
- 💬 Forum: [English](https://mischianti.org/forums/forum/mischiantis-libraries/) · [Italiano](https://mischianti.org/it/forums/forum/le-librerie-di-mischianti/)
- 🐛 Issues: [GitHub](https://github.com/xreef/QDY30A_RS485/issues)
