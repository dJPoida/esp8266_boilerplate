#ifndef LED_CONTROL_H
#define LED_CONTROL_H

#include "pins.h"
#include "config.h"

// Globals
bool apFlashStateOn = false;     
unsigned long previousAPFlashMillis = 0;
const long apFlashInterval = 1000;


/**
/ Write the current LED state to the LED Pin
*/
void updateLED() {
  digitalWrite(PIN_LED, accessPointMode ? apFlashStateOn : ledOn);  
}



/**
 * Set the LED On or Off
 * 
 * @param bool newLedOn whether the LED should now be on or off
 */
void setLedOn(bool newLedOn) {
  Serial.print("Setting the LED to: '");
  if (newLedOn) {
    Serial.println("ON'");
  } else {
    Serial.println("OFF'");
  }
  ledOn = newLedOn;

  updateLED();

  // Save the updated config.
  lazyWriteConfig();
}



/**
 * Flash the LEDs when in Access Point Mode as the device needs configuring to the WiFi
 */
void handleFlashAPModeLED() {
  unsigned long currentMillis = millis();
  if(currentMillis - previousAPFlashMillis >= apFlashInterval) {
    previousAPFlashMillis = currentMillis;   
    if (apFlashStateOn == false) {
      apFlashStateOn = true;
    } else {
      apFlashStateOn = false;
    }
    updateLED();
  }
}



/**
 * Initialise the LED
 * @returns boolean true if the LED was initialised
 */
boolean initLED() {
  pinMode(PIN_LED, OUTPUT);

  updateLED();
  
  return true;
}

#endif
