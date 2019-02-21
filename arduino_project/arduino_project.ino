/**
 * ESP8266 Boilerplate
 * 
 * Basic WiFi control of an ESP8266 with API endpoints and Socket Control
 * Author: Peter Eldred
 * Date: 2019-02
 * License: MIT
 * 
 * Dependencies:
 *  - ESP8266 Arduino Core (https://github.com/esp8266/Arduino)
 *  - ESP8266 Sketch Data Upload Tool (http://esp8266.github.io/Arduino/versions/2.0.0/doc/filesystem.html)
 *  - ESP8266AsyncTCP (https://github.com/me-no-dev/ESPAsyncTCP)
 *  - ESPAsyncWebServer (https://github.com/me-no-dev/ESPAsyncWebServer)
 */

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include "config.h"
#include "pins.h"
#include "wifi.h"
#include "webServer.h"



/**
 * Setup
 */
void setup() {
  delay(1000);
  
  // Serial Initialisation
  Serial.begin(115200);
  Serial.println("");
  Serial.print(applicationName);
  Serial.print(" v");
  Serial.println(ver);
  Serial.println("");

  // Start the SPI Flash Files System
  if (!SPIFFS.begin()) {
    Serial.println("Failed to mount file system");
    return;
  }
  Serial.println("SPIFFS initialised.");

  // Load the config from the onboard SPI File System
  if (!loadConfig()) {
    Serial.println("Failed to load configuration from SPIFFS");
    Serial.print("Creating new Config File... ");
    if (!saveConfig()) {
      Serial.println("Failed to create new config file on SPIFFS.");
      return;
    }
    Serial.println("done.");
  }

  // Give SPIFFS a tick to catch up
  delay(250);
  
  // Initialise the Reset Capability
  if (!initReset()) {
    Serial.println("Failed to initialise the Reset function");
    return;
  }

  // Initialise the WiFi
  if (!initWifi()) {
    Serial.println("Failed to initialise the WiFi.");

    // Failed to initialise the WiFi - Begin broadcasting a hotspot
    initAccessPointMode();
  }

  // Initialise the Web Server
  if (!initWebServer()) {
    Serial.println("Failed to initialise the Web Server");
    return;
  }

  // Initialise the LED
  if (!initLED()) {
    Serial.println("Failed to initialise the LED");
    return;
  }

  // Add other normal operation initialisation code here
  if (!accessPointMode) {
  }
}



/**
 * Application Loop
 */
void loop() {
  // Has something requested a reset?
  if (resetFlag) {
    resetDevice();
  }

  if (accessPointMode) {
    apLoop();
  } else {
    normalLoop();
  }

  // Check so see if something wants to write to the config
  checkLazyWriteConfig();

  delay(1);
}



/**
 * Code to run when the device is in normal opration mode
 */
void normalLoop() {
  // Put any normal operation code here
}



/**
 * Code to run when the device is in Access Point mode
 */
void apLoop() {
    // Flash the LEDs to indicate the device is in configuration mode
    handleFlashAPModeLED();
}

