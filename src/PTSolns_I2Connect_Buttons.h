#ifndef PTSOLNS_I2CONNECT_BUTTONS_H
#define PTSOLNS_I2CONNECT_BUTTONS_H

#include <Arduino.h>
#include <Wire.h>

// Logical Button Definitions 
#define BUTTON_A1 0x00
#define BUTTON_A2 0x01
#define BUTTON_A3 0x02
#define BUTTON_A4 0x03
#define BUTTON_B1 0x04
#define BUTTON_B2 0x05
#define BUTTON_B3 0x06
#define BUTTON_B4 0x07
#define BUTTON_C1 0x08
#define BUTTON_C2 0x09
#define BUTTON_C3 0x0A
#define BUTTON_C4 0x0B
#define BUTTON_D1 0x0C
#define BUTTON_D2 0x0D
#define BUTTON_D3 0x0E
#define BUTTON_D4 0x0F

class PTSolns_I2Connect_Buttons {
public:
    // The default address based on the standard solder jumpers (AD1=Low, AD0=High)
    static const uint8_t DEFAULT_I2C_ADDRESS = 0x59; 
    static const uint32_t DEFAULT_I2C_CLOCK = 100000UL;
    
    // AW9523B IC Registers
    static const uint8_t AW9523_REG_INPUT_P0  = 0x00;
    static const uint8_t AW9523_REG_INPUT_P1  = 0x01;
    static const uint8_t AW9523_REG_CONFIG_P0 = 0x04;
    static const uint8_t AW9523_REG_CONFIG_P1 = 0x05;
    static const uint8_t AW9523_REG_INT_P0    = 0x06;
    static const uint8_t AW9523_REG_INT_P1    = 0x07;
    static const uint8_t AW9523_REG_ID        = 0x10;
    static const uint8_t AW9523_REG_CTL       = 0x11;
    static const uint8_t AW9523_REG_MODE_P0   = 0x12;
    static const uint8_t AW9523_REG_MODE_P1   = 0x13;

    PTSolns_I2Connect_Buttons();

    bool begin(TwoWire &wirePort = Wire, uint8_t i2cAddress = DEFAULT_I2C_ADDRESS, uint32_t i2cClock = DEFAULT_I2C_CLOCK);

    void setI2CAddress(uint8_t i2cAddress);
    void setI2CClock(uint32_t i2cClock);

    bool isConnected();
    
    // Core functional methods for interacting with the buttons
    bool isPressed(uint8_t logicalButtonId);
    bool isReleased(uint8_t logicalButtonId);
    uint16_t readAll();

private:
    TwoWire *_wire;
    uint8_t _i2cAddress;
    uint32_t _i2cClock;

    // Hardware to Silkscreen Lookup Table
    // Index = Logical ID (0-15). Value = Hardware IC Bit (0-15)
    const uint8_t _pinMap[16] = {
        13, 12, 7, 5, 14, 15, 6, 4, 8, 9, 1, 3, 10, 11, 0, 2
    };

    void writeRegister(uint8_t reg, uint8_t value);
    uint8_t readRegister(uint8_t reg);
    uint16_t readRawHardware();
};

#endif