#ifndef RESET_H
#define RESET_H

#include "pins.h"
#include "FS.h"

bool resetFlag = false; // Flag to tell the kernel it's time for a reset



/**
 * Resets the device by pulling the reset pin high
 */
void resetDevice() {
  Serial.println("Restarting device...");

  // Shut down SPIFFS
  SPIFFS.end();
  delay(200);

  // From https://github.com/esp8266/Arduino/issues/1722#issuecomment-321818357
  //WiFi.forceSleepBegin();
  //wdt_reset();
  ESP.restart();
  //while(1) {
  //  wdt_reset();
  //}
  //ESP.reset();
}


/**
 * Flags the kernal down for a reset
 */
void flagReset() {
  resetFlag = true;
}



/**
 * Initialise any pins not initialised by their own dedicated controllers
 * @returns boolean whether the init was successful
 */
bool initReset() {
  resetFlag = false;
  return true;
}

#endif

