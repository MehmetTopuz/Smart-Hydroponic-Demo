# Smart Hydroponic Demo Project

In this demo hydroponic project I have used ESP8266 with the STM32 microcontroller. The main goal of this project is to develop control software for a non-existing hydroponic system. Therefore this project is not tested on a real system. It was only tested to verify that this system sends data over the MQTT protocol. The drivers that will control the inputs and outputs have not been written yet.

I decided to use an ESP32 series microcontroller after this stage because communicating with ESP8266 using AT commands was difficult and unreliable. I will create a new version of this software using only the ESP32 series microcontroller without STM32. I am planning to use the VS Code IDE to program the ESP32.

## Bonus

I am considering creating UI software with Qt/QML to remotely control and monitor the system.
