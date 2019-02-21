#ifndef IFTTT_H
#define IFTTT_H

#include "config.h"

/**
 * Make an HTTP request to the IFTTT web service
 * This is called if IFTTT is enabled
 */
void sendTriggerToIFTTT() {
  Serial.println("Sending Trigger to IFTTT");
  // Make sure the WiFi is connected
  if (initWifi()) {
    Serial.print("Connecting to "); 
    Serial.print(iftttServer);
    
    WiFiClient client;
    int retries = 5;
    while(!!!client.connect(iftttServer.c_str(), 80) && (retries-- > 0)) {
      Serial.print(".");
    }
    Serial.println();
    if(!!!client.connected()) {
       Serial.println("Failed to connect, going back to sleep");
    }
    
    Serial.print("Request resource: "); 
    Serial.println(iftttEndpoint.c_str());
    client.print(String("GET ") + iftttEndpoint + 
                    " HTTP/1.1\r\n" +
                    "Host: " + iftttServer + "\r\n" + 
                    "Connection: close\r\n\r\n");
                    
    int timeout = 5 * 10; // 5 seconds             
    while(!!!client.available() && (timeout-- > 0)){
      delay(100);
    }
    if(!!!client.available()) {
       Serial.println("No response, going back to sleep");
    }
    while(client.available()){
      Serial.write(client.read());
    }
    
    Serial.println("\nclosing connection");
    client.stop();
  }
}

#endif
