// Example: Basic Polling for I2Connect: Buttons 
// Last Update: October 23, 2026
// Support: http://ptsolns.com/contact-us
//
// WHAT THIS EXAMPLE DOES
// Initializes the Buttons board and continuously polls the module over I2C
// to detect if any of the 16 tactile switches have been pressed.
//
// SKILLS LEARNED
// begin()
// readAll()
//
// DESCRIPTION
// The I2Connect: Buttons features 16 tactile pushbuttons in a 4x4 matrix.
// This example uses the standard Qwiic/STEMMA QT connector to poll the board 
// without needing the INT (Interrupt) pin connected. It translates the scrambled 
// physical routing of the IC into a clean 16-bit integer that perfectly matches 
// the A1 through D4 (0x00 to 0x0F) silkscreen labels on the PCB.

#include <Wire.h>
#include <PTSolns_I2Connect_Buttons.h>

// User Settings
const uint32_t SERIAL_BAUD  = 115200; 
const uint32_t I2C_CLOCK_HZ = 100000; // Keep at 100 kHz to prevent Qwiic cable capacitance issues
const uint8_t  I2C_ADDRESS  = 0x59;   // Default address (AD0 High, AD1 Low)

PTSolns_I2Connect_Buttons buttons;

uint16_t lastState = 0xFFFF; // Tracks previous state to prevent Serial Monitor spam

void setup() {
    Serial.begin(SERIAL_BAUD);
    while (!Serial) { }

    Wire.begin();
    
    if (!buttons.begin(Wire, I2C_ADDRESS, I2C_CLOCK_HZ)) {
        Serial.println("Buttons not detected. Please check your wiring!");
        while (1) { } // Halt execution
    }
    
    Serial.println("Buttons initialized.");
    Serial.println("Press any button on the matrix (A1-D4)...");
    Serial.println("----------------------------------------");
}

void loop() {
    // Read the current state of all 16 buttons. 
    // Unpressed buttons return 1. Pressed buttons return 0.
    uint16_t currentState = buttons.readAll();
    
    // Only print if the state has changed
    if (currentState != lastState) {
        
        Serial.print("Pressed: ");
        bool anyPressed = false;
        
        // Loop through logical ID 0 to 15 (0x00 to 0x0F)
        for (int i = 0; i < 16; i++) {
            if (!(currentState & (1 << i))) {
                anyPressed = true;
                
                // Calculate and print the physical label (e.g., A1, B2)
                char row = 'A' + (i / 4);
                int col = (i % 4) + 1;
                Serial.print(row);
                Serial.print(col);
                Serial.print(" ");
            }
        }
        
        if (!anyPressed) {
            Serial.print("None");
        }
        Serial.println();
        
        lastState = currentState;
    }
    
    delay(50); // Small polling delay
}
