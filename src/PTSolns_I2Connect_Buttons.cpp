#include "PTSolns_I2Connect_Buttons.h"

PTSolns_I2Connect_Buttons::PTSolns_I2Connect_Buttons() {
    _wire = &Wire;
    _i2cAddress = DEFAULT_I2C_ADDRESS;
    _i2cClock = DEFAULT_I2C_CLOCK;
}

bool PTSolns_I2Connect_Buttons::begin(TwoWire &wirePort, uint8_t i2cAddress, uint32_t i2cClock) {
    _wire = &wirePort;
    setI2CAddress(i2cAddress); 
    _i2cClock = i2cClock;
    _wire->setClock(_i2cClock);

    delay(40); // Standard power-up wait time

    if (!isConnected()) {
        return false;
    }

    // Initialize the AW9523B IC for Button Input
    writeRegister(AW9523_REG_CTL, 0x10);       // GPOMD=1 (Port 0 Push-Pull mode)
    writeRegister(AW9523_REG_MODE_P0, 0xFF);   // Set Port 0 to GPIO Mode
    writeRegister(AW9523_REG_MODE_P1, 0xFF);   // Set Port 1 to GPIO Mode
    writeRegister(AW9523_REG_CONFIG_P0, 0xFF); // Set Port 0 as Inputs
    writeRegister(AW9523_REG_CONFIG_P1, 0xFF); // Set Port 1 as Inputs
    
    // Enable interrupts on all pins
    writeRegister(AW9523_REG_INT_P0, 0x00);
    writeRegister(AW9523_REG_INT_P1, 0x00);

    // Flush and clear any existing hardware interrupts on boot
    readRawHardware();
    
    return true;
}

void PTSolns_I2Connect_Buttons::setI2CAddress(uint8_t i2cAddress) {
    _i2cAddress = i2cAddress;
}

void PTSolns_I2Connect_Buttons::setI2CClock(uint32_t i2cClock) {
    _i2cClock = i2cClock;
    if (_wire) {
        _wire->setClock(_i2cClock);
    }
}

bool PTSolns_I2Connect_Buttons::isConnected() {
    // Reading the Chip ID register (should return 0x23 for AW9523B)
    _wire->beginTransmission(_i2cAddress);
    _wire->write(AW9523_REG_ID);
    if (_wire->endTransmission() != 0) return false;
    
    _wire->requestFrom(_i2cAddress, (uint8_t)1);
    if (_wire->available()) {
        return (_wire->read() == 0x23);
    }
    return false;
}

void PTSolns_I2Connect_Buttons::writeRegister(uint8_t reg, uint8_t value) {
    _wire->beginTransmission(_i2cAddress);
    _wire->write(reg);
    _wire->write(value);
    _wire->endTransmission();
}

uint8_t PTSolns_I2Connect_Buttons::readRegister(uint8_t reg) {
    _wire->beginTransmission(_i2cAddress);
    _wire->write(reg);
    _wire->endTransmission();
    
    _wire->requestFrom(_i2cAddress, (uint8_t)1);
    if (_wire->available()) {
        return _wire->read();
    }
    return 0xFF; // Default to unpressed state if read fails
}

uint16_t PTSolns_I2Connect_Buttons::readRawHardware() {
    uint8_t p0 = readRegister(AW9523_REG_INPUT_P0);
    uint8_t p1 = readRegister(AW9523_REG_INPUT_P1);
    return ((uint16_t)p1 << 8) | p0;
}

bool PTSolns_I2Connect_Buttons::isPressed(uint8_t logicalButtonId) {
    if (logicalButtonId > 15) return false;
    
    uint16_t rawState = readRawHardware();
    uint8_t hardwareBit = _pinMap[logicalButtonId];
    
    // 0 means pressed, 1 means released
    return !(rawState & (1 << hardwareBit)); 
}

bool PTSolns_I2Connect_Buttons::isReleased(uint8_t logicalButtonId) {
    return !isPressed(logicalButtonId);
}

uint16_t PTSolns_I2Connect_Buttons::readAll() {
    uint16_t rawState = readRawHardware();
    uint16_t cleanLogicalState = 0;
    
    // Map the scrambled hardware bits back to sequential 0-15 matching the silkscreen
    for (int i = 0; i < 16; i++) {
        if (rawState & (1 << _pinMap[i])) {
            cleanLogicalState |= (1 << i); // Keep bit HIGH (1) if unpressed
        }
    }
    
    return cleanLogicalState;
}