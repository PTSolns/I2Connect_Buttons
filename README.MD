# PTSolns *I2Connect: Buttons* Library

![Arduino Library](https://img.shields.io/badge/Arduino-Library-blue)
![License](https://img.shields.io/badge/License-MIT-green)
![Input](https://img.shields.io/badge/Input-16__Button__Matrix-orange)

Library for the PTSolns *I2Connect: Buttons* module. Available through both the Arduino Library Manager and the PTSolns IDE Library Manager, this library provides seamless interaction with the 16-button matrix, translating complex hardware pin routing into a clean, sequential logical interface.

---

## Overview

The *I2Connect: Buttons* is a compact user-input module featuring 16 tactile pushbuttons arranged in a 4x4 matrix labeled A1 through D4. Driven by an onboard AW9523B I2C GPIO expander, this module provides an immediate, reliable hardware interface for embedded systems without consuming numerous microcontroller pins.

The module communicates over the industry standard I2C interface and features a dedicated INT (Interrupt) pin. This allows for an event-driven architecture where the microcontroller is only notified the exact moment a button is pressed, eliminating the need for resource-heavy constant I2C polling. The board is fully 3.3/5V rated for compatibility across standard microcontroller logic levels. 

Like all I2Connect modules, it connects using dual Qwiic® compatible connectors to easily integrate into existing I2C systems without soldering, while the preinstalled right-angle header provides direct access to the I2C bus and the interrupt pin for breadboard-based prototyping.

---

## *I2Connect* Series

The PTSolns *I2Connect* Series is a family of plug and play expansion modules designed to simplify the integration of sensors, displays, user interfaces, and other peripherals into embedded systems. Built around the industry standard I2C communication protocol and featuring convenient Qwiic® compatible connectors (4 pin, 1 mm JST SH), *I2Connect* modules can be quickly connected without soldering or complex wiring.

Each module is fully assembled and includes a preinstalled male header that is compatible with standard solderless breadboards, enabling immediate use and rapid prototyping. Designed to work seamlessly with a wide range of microcontroller platforms, I2Connect modules combine compact hardware, comprehensive documentation, and easy to use software libraries. The _I2Connect_ Series enables makers, students, educators, and professionals to rapidly prototype, evaluate, and develop reliable electronic systems.

The following modules are part of the *I2Connect* Series:

- [**AHT20**](https://ptsolns.com/products/i2connect-aht20) — Temperature & Humidity
- [**VEML7700**](https://ptsolns.com/products/i2connect-veml7700) — Light & Lux
- [**EEPROM**](https://ptsolns.com/products/i2connect-eeprom) — 2 Mbit Memory
- **Buttons** — User Input via 4×4 Matrix of Buttons
- **Digit Display** — Dual 7 Segment Display
- **ICP-20100** — Pressure & Temperature
- **KXTJ3-1057** — 3 Axis Accelerometer
- **MUX** — 4 Channel I2C Multiplexer
- **OLED** — 0.96" 128×64 OLED Display
- **I2C Doctor** — I2C Bus Recovery and Diagnostics
- **...and more!**

---

## Key Features

- 16 tactile pushbuttons configured in a 4x4 matrix (A1 to D4).
- Driven by the AW9523B IC.
- Hardware INT (Interrupt) output pin for event-driven logic.
- Default I2C address of 0x59.
- 3.3V and 5V logic and power compatibility.
- Dual Qwiic® compatible 4-pin JST SH connectors for daisy-chaining.
- Breadboard-friendly 5-pin 2.54mm angled male header (VCC, GND, SDA, SCL, INT).

---

## I2Connect Ecosystem

The *I2Connect: Buttons* is part of the PTSolns I2Connect ecosystem.

All *I2Connect* modules utilize a standardized 4 pin JST SH 1.0 mm connector carrying:

| Pin | Function |
| --- | -------- |
| 1   | GND      |
| 2   | VCC      |
| 3   | SDA      |
| 4   | SCL      |

This connector follows the widely adopted Qwiic® standard, allowing modules and compatible boards to be connected without soldering.

For maximum compatibility, the module also includes a preinstalled angled male header, allowing use with virtually any 3.3 V or 5 V microcontroller platform even when a Qwiic compatible connector is unavailable. Note: The hardware interrupt feature requires connecting the `INT` pin available exclusively on the 5-pin header.

---

## Buttons Specifications

| Parameter               | Value                               |
| ----------------------- | ----------------------------------- |
| Button Layout           | 4x4 Matrix (16 Buttons)             |
| Interface               | I2C + External Hardware Interrupt   |
| I2C Address             | 0x59 (Default)                      |
| Operating Voltage       | 3.3 V / 5 V Rated                   |
| Connectors              | 2x 4-pin 1mm JST SH, 1x 5-pin Header|

---

## I2C Addressing & Multiplexing

The *I2Connect: Buttons* module uses a default I2C address of:

```text
0x59
```
This address can be reconfigured to 0x58, 0x5A, or 0x5B by modifying the AD0 and AD1 solder jumpers on the back of the PCB.

If more than four Button modules are required in the same system, an I2C multiplexer can be used. The PTSolns I2Connect: MUX module provides four independent I2C channels, allowing extended expansion without address collisions.

---

## *PTSolns IDE*

The [*PTSolns IDE*](https://ptsolns.com/ide) is a free desktop application for programming PTSolns microcontroller development boards and compatible Arduino platforms.

This library is available directly through both the PTSolns IDE Library Manager and the Arduino Library Manager.

To install:

1. Open the PTSolns IDE (or Arduino IDE).
2. Open the Library Manager.
3. Search for:

```text
I2Connect_Buttons
```
4. Click Install.

Features of the [*PTSolns IDE*](https://ptsolns.com/ide) include:

* Integrated board management
* Built in serial monitor
* Library management
* Example project browser
* Support for over 8,500 third party libraries
* Built in CH340 USB driver support
* Open source and community driven

The [*PTSolns IDE*](https://ptsolns.com/ide) can be used alongside the Arduino IDE and supports many PTSolns development boards out of the box.

---

## Quick Start

```cpp
#include <Wire.h>
#include <PTSolns_I2Connect_Buttons.h>

PTSolns_I2Connect_Buttons buttons;

void setup()
{
    Serial.begin(115200);
    Wire.begin();

    if (buttons.begin()) {
        Serial.println("Buttons initialized!");
    } else {
        Serial.println("Failed to find module.");
        while(1);
    }
}

void loop()
{
    // Check if a specific button is physically pressed
    if (buttons.isPressed(BUTTON_A1)) {
        Serial.println("Button A1 is pressed.");
    }
    
    if (buttons.isPressed(BUTTON_D4)) {
        Serial.println("Button D4 is pressed.");
    }
    
    delay(100);
}
```

---
## Basic Functions
Initialization

```cpp
// Initializes the button board at the default 0x59 address
buttons.begin();
```
Reading Data
```cpp
// Check a single button state using the silkscreen macro
if (buttons.isPressed(BUTTON_B2)) {
    // Execute action
}

// Read the state of all 16 buttons simultaneously
uint16_t allButtons = buttons.readAll();
```
---

## Library Functions

| Function          | Description                              |
| ----------------- | -----------------------------------------|
| begin()           | Initialize the I2C bus and configure AW9523B IC registers          |
| isPressed()         | Check if a specific logical button ID is currently pressed|
| isReleased()       |Check if a specific logical button ID is currently released|
| readAll()       |Retrieve a 16-bit integer reflecting the entire matrix state|

---

## Typical Applications




---
