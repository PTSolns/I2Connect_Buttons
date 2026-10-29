// Example: Hardware Interrupt for I2Connect: Buttons 
// Last Update: October 23, 2026
// Support: http://ptsolns.com/contact-us
//
// WHAT THIS EXAMPLE DOES
// Demonstrates how to use the INT pin to create a highly efficient, event-driven 
// button reader that frees up the microcontroller and I2C bus.
//
// SKILLS LEARNED
// attachInterrupt()
// readAll()
//
// DESCRIPTION
// Instead of constantly polling the I2C bus, the AW9523B chip on the Buttons  
// board will automatically pull the INT pin LOW the exact moment a button is pressed.
// The microcontroller detects this voltage drop instantly, waking up to query the 
// board. This method guarantees no button clicks are ever missed, even if your 
// main loop() is heavily delayed doing other tasks.

#include <Wire.h>
#include <PTSolns_I2Connect_Buttons.h>

// User Settings
const uint32_t SERIAL_BAUD  = 115200; 
const uint32_t I2C_CLOCK_HZ = 100000;
const uint8_t  I2C_ADDRESS  = 0x59;   

// Connect the 5th wire on the header (INT) to a hardware interrupt capable pin
const int INT_PIN = 2; // Digital Pin 2 supports external interrupts on Arduino Uno/Mega

PTSolns_I2Connect_Buttons buttons;

volatile bool interruptTriggered = false;

// Interrupt Service Routine (ISR)
// Keeps code execution inside the interrupt as short as possible
void handleButtonPress() {
    interruptTriggered = true;
}

void setup() {
    Serial.begin(SERIAL_BAUD);
    while (!Serial) { }

    Wire.begin();
    
    // The INT pin from the board is an open-drain output, so it needs a pull-up
    pinMode(INT_PIN, INPUT_PULLUP);
    
    if (!buttons.begin(Wire, I2C_ADDRESS, I2C_CLOCK_HZ)) {
        Serial.println("Buttons   not detected. Please check your wiring!");
        while (1) { } 
    }
    
    // Attach the interrupt to trigger when the line drops LOW (FALLING edge)
    attachInterrupt(digitalPinToInterrupt(INT_PIN), handleButtonPress, FALLING);
    
    Serial.println("Buttons Interrupt Mode initialized.");
    Serial.println("Waiting for physical press...");
    Serial.println("----------------------------------------");
}

void loop() {
    if (interruptTriggered) {
        delay(20); // Quick software debounce for the mechanical tactile switches
        
        // Read the state of all 16 buttons
        // Calling readAll() automatically resets the hardware INT line back to HIGH
        uint16_t currentState = buttons.readAll();
        
        Serial.print("Interrupt Detected! Action: ");
        bool anyPressed = false;
        
        // Loop through logical ID 0 to 15 to find which button triggered it
        for (int i = 0; i < 16; i++) {
            if (!(currentState & (1 << i))) { // 0 means pressed
                anyPressed = true;
                
                // Calculate and print the physical label (e.g., A1, B2)
                char row = 'A' + (i / 4);
                int col = (i % 4) + 1;
                Serial.print(row);
                Serial.print(col);
                Serial.print(" Pressed  ");
            }
        }
        
        // If the interrupt fired but no buttons are currently LOW, it was a release event
        if (!anyPressed) {
            Serial.print("Button Released");
        }
        Serial.println();
        
        interruptTriggered = false; // Reset the flag to wait for the next event
    }
    
    // The Arduino is free to run heavy, blocking code here without missing clicks!
    // Try uncommenting the delay below to prove it:
    // delay(3000); 
}
