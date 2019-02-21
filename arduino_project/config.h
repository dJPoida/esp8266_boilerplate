#ifndef CONFIG_H
#define CONFIG_H

#include <ArduinoJson.h>
#include "FS.h"
#include "reset.h"

// Application Settings
const String    applicationName = "ESP8266 Boilerplate";  // The name of the application
const String    ver = "1.0.0";

// WiFi Settings
String          apssid          = "ESP8266";              // The SSID when the device is running in Access Point mode
String          mdnsAddress     = "esp8266";              // The address that clients can use to connect to the device without the IP (i.e. http://esp8266.local)
String          networkHostname = "ESP8266";              // The Hostname to display when other devices on the network want to identify this device
String          ssid            = "";                     // The WiFi accesspoint to connect to (this is configured and loaded from SPIFFS)
String          password        = "";                     // The WiFi accesspoint password (this is condigured and loaded from SPIFFS)
int             serverPort      = 80;                     // The Port to run the server on (this is the default and after initial config is loaded from SPIFFS)

// Hardware Config
bool            ledOn           = 0;                      // Whether the onboard LED is ON or OFF (in normal operation mode)

// Globals
bool            writingConfig   = false;                  // Keeps track of whether the config is being written at the moment
const long      lazyWriteDelay  = 1000;                   // Wait this many MS to lazy write the config (useful to ensure we don't write too much consecutively)
bool            lazyWriteConfigFlag = false;              // Flag to indicate that the config needs to be written, just not right now.
long            lastLazyWriteConfigDebounce = 0;          // Last time something wanted to lazy write to the config
String          localIPAddress  = "";                     // The local IP address the device has been issued by the DCHP on the network (only updated on WiFi connect)



/**
 * Load the config.json from spiffs
 * @returns boolean true if the config could be loaded
 */
boolean loadConfig() {
  File configFile = SPIFFS.open("/config.json", "r");
  if (!configFile) {
    Serial.println("Failed to open config file");
    return false;
  }

  size_t size = configFile.size();
  if (size > 1024) {
    Serial.println("Config file size is too large");
    return false;
  }

  // Allocate a buffer to store contents of the file.
  std::unique_ptr<char[]> buf(new char[size]);

  // We don't use String here because ArduinoJson library requires the input
  // buffer to be mutable. If you don't use ArduinoJson, you may as well
  // use configFile.readString instead.
  configFile.readBytes(buf.get(), size);

  DynamicJsonBuffer jsonBuffer;
  JsonObject& json = jsonBuffer.parseObject(buf.get());

  // Close the file before we corrupt it.
  configFile.close();

  // Check that we were able to load and parse the file
  if (!json.success()) {
    Serial.println("Failed to parse config file");
    return false;
  }

  // Update the current config values from the loaded JSON
  ssid = json.get<String>("ssid");
  password = json.get<String>("password");
  ledOn = json.get<bool>("ledOn");
    
  Serial.println("Config Loaded from SPIFFS: ");
  Serial.print(" - WiFi SSID: ");
  Serial.println(ssid);
  Serial.print(" - WiFi Password: ");
  Serial.println(password);
  Serial.print(" - LED ON: ");
  Serial.println(ledOn);
  
  return true;
}


/**
 * Save the current configuration to SPIFFS
 */
bool saveConfig() {
  // Wait for any current write operations to finish
  while (writingConfig) {
    delay(1);
  };
  
  lazyWriteConfigFlag = false;
  writingConfig = true;
  Serial.println("Writing settings to configuration file...");

  DynamicJsonBuffer jsonBuffer;
  JsonObject& json = jsonBuffer.createObject();

  // Add everything we want in our JSON struct to the jsonObject
  json["ssid"] = ssid ? ssid : "";
  json["password"] = password ? password : "";
  json["ledOn"] = ledOn ? "true" : "false";

  // Open the file for writing
  File configFile = SPIFFS.open("/config.json", "w");
  if (!configFile) {
    Serial.println("Failed to open config file for writing");
    return false;
  }

  // Debug
  //String jsonData;
  //json.printTo(jsonData);
  //Serial.print("JSON: ");
  //Serial.println(jsonData);

  // Write the JSON to the file
  json.printTo(configFile);
  configFile.close();
  
  writingConfig = false;
  
  return true;
}



/**
 * Indicate that the config file needs to be written, but not right now.
 */
void lazyWriteConfig() {
  lazyWriteConfigFlag = true;
  lastLazyWriteConfigDebounce = millis();
}



/**
 * Checks to see if something is wanting to write the config to SPIFFS lazily 
 * (i.e. at the next opportunity)
 */
void checkLazyWriteConfig() {
  if (lazyWriteConfigFlag) {
    if (millis() - lastLazyWriteConfigDebounce > lazyWriteDelay) {
      Serial.println("Lazy write config triggered");
      saveConfig();
    }
  }
}



/**
 * Change the currrent Wifi Settings (triggered from the Configuration Webiste)
 * @var String newSSID the new WiFi SSID to save to the device
 * @var String newPassword the new WiFi Password to save to the device
 */
void setWiFiSettings(String newSSID, String newPassword) {
    ssid = newSSID;
    password = newPassword;

    Serial.print("Configuring and saving new WiFi Hotspot details, SSID: '");
    Serial.print(ssid);
    Serial.print("', Password: '");
    Serial.print(password);
    Serial.println("'...");

    // Save the updated config.
    saveConfig();

    Serial.println("restarting...");

    // Reset the device
    flagReset();
}



/**
 * Build up the current config for feedback to the control panel and other web
 * interfaces.
 * Originally I was simply returning the config.json directly from the SPIFFS
 * but, that also contained the WiFi password etc... so had to come up with a
 * mechanic to do it manually.
 */
String getConfigAsJson() {
  String result = "{";

  // ledFlashMode
  result += "\"ledOn\":";
  result += ledOn;
  //result += ",";

  result += "}";

  return result;
}

#endif
