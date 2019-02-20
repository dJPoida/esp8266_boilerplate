# esp8266_boilerplate
My attempt at a boilerplate environment for the 8266 using Arduino, Socket.IO and React

## Current Features
- React
- Webpack
- Babel

## ToDo
This repo will be considered complete when it contains the following
- [ ] An Arduino project setup to Flash the onboard LED
    - [ ] WiFi Hotspot and Configuration
    - [ ] Serves a webpage and Static Assets from the onboard SPIFFS
    - [ ] Can be configured using the Access Point mode
    - [ ] Can be remotely reset
    - [ ] Accepts incoming Socket connections and broadcasts the state of the LED
    - [ ] Contains an API for turning the LED flash on / off
    - [ ] Stores its settings locally on the SPIFFS
- [ ] A React application for the APP which will be accessible either via an Access Point or Local WiFi
    - [ ] Serves an Access Point Configuration mode application
    - [ ] Serves a regular operation mode control application