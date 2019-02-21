#ifndef WIFI_H
#define WIFI_H

#include <ESP8266WiFi.h>

boolean wifiConnected = false;
boolean accessPointMode = false;

/**
 * Connect to the WiFi network (if not already connected)
 * @returns boolean true if wifi could be connected
 */
boolean initWifi() {
  if (WiFi.status() != WL_CONNECTED) {
    WiFi.hostname(networkHostname);
    
    // Don't bother if we don't have a configuration yet
    if (!ssid.equals("")) {
      Serial.print("Connecting to: "); 
      Serial.print(ssid);
      WiFi.begin(ssid.c_str(), password.c_str());  
    
      int timeout = 10 * 4; // 10 seconds
      while(WiFi.status() != WL_CONNECTED  && (timeout-- > 0)) {
        delay(250);
        Serial.print(".");
      }
      Serial.println("");
    
      if(WiFi.status() != WL_CONNECTED) {
         Serial.println("Failed to connect!");
      }

      localIPAddress = WiFi.localIP().toString();
    
      Serial.print("WiFi connected."); 
      Serial.print("IP address: "); 
      Serial.println(localIPAddress);
  
      wifiConnected = (WiFi.status() == WL_CONNECTED);
    }
  }

  return wifiConnected;
}



/**
 * Broadcast a WiFi network for the purposes of initialising the Wifi
 * @returns boolean true if a the access point was initialised
 */
boolean initAccessPointMode() {
  accessPointMode = true;

  Serial.print("Configuring Open WiFi Access Point: ");
  Serial.println(apssid);
  
  WiFi.mode(WIFI_AP_STA);
  //  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));   // subnet FF FF FF 00  
  WiFi.softAP(apssid.c_str()); // No password makes it an open network

  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);

  wifiConnected = true;
  return wifiConnected;
}

#endif
