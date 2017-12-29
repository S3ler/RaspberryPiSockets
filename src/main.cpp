// nrf95_ping_pong.ino
// -*- mode: C++ -*-

#ifdef ESP8266
#include <ESP8266WiFi.h>
#endif

#include <Arduino.h>

#ifdef Arduino_h

#include <SPI.h>
#include <Ethernet.h>

#endif

#ifndef Arduino_h
#include <dirent.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <iostream>
#include <chrono>
#include <thread>
#include <string>
#include <LoggerInterface.h>
#endif

#include <RF95Socket.h>
#include <SimpleMqttSnClientTester.h>

#include <LinuxLogger.h>
#include <MqttSnMessageHandler.h>
#include <LinuxLogger.h>
#include <RF95Socket.h>
#include <RHReliableDatagram.h>
#if defined(DRIVER_RH_RF95)
//#include <wiringPi.h>
//#include <Arduino.h>

#else
#include <RH_NRF24.h>
#include <bcm2835.h>
#endif



RF95Socket socket;
LoggerInterface logger;

#ifdef SIMPLEMQTTSNCLIENTTESTER
SimpleMqttSnClientTester mqttSnMessageHandler;
#else
MqttSnMessageHandler mqttSnMessageHandler;
#endif


#ifdef DRIVER_RH_RF95
#ifdef ESP8266
RH_RF95 rh_driver(2, 15);
#else
RH_RF95 rh_driver;
#endif
#endif

#ifdef DRIVER_RH_NRF24
#ifdef ESP8266
// TODO
RH_NRF24 rh_driver(2, 15);
#else
#if defined(RASPBERRY_PI)
// Create an instance of a driver
// Chip enable is pin 22
// Slave Select is pin 24
RH_NRF24 rh_driver(RPI_V2_GPIO_P1_18, RPI_V2_GPIO_P1_24);
#else
//RH_NRF24 rh_driver;
#endif
#endif
#endif

RHReliableDatagram manager(rh_driver);

#if !defined(Arduino_h)
SerialLinux Serial;
#endif
#ifdef PING
#define OWN_ADDRESS 0x05
#define PONG_ADDRESS 0x03
device_address target_address(PONG_ADDRESS, 0, 0, 0, 0, 0);
uint8_t msg[] = {5, 'P', 'i', 'n', 'g'};
#elif PONG
#define OWN_ADDRESS 0x03
#endif


void setup() {
    Serial.begin(9600);
    Serial.println("Starting");
    Serial.print("OWN_ADDRESS: ");
#ifndef Arduino_h
    std::string number_str = std::to_string(OWN_ADDRESS);
    Serial.println(number_str.c_str());
#else
    Serial.println(OWN_ADDRESS);
#endif
    Serial.print("ROLE: ");
#ifdef PING
    Serial.println("PING");
#elif PONG
    Serial.println("PONG");
#else
    Serial.println("UNDEFINED");
#endif

#ifdef DRIVER_RH_RF95
    if(!wiringPiSetupGpio()){
        Serial.println("Done init wiringPiSetupGpio");
    }
    // Defaults after init are 434.0MHz, 13dBm, Bw = 125 kHz, Cr = 4/5, Sf = 128chips/symbol, CRC on
    if (!rh_driver.init()) {
        Serial.println("Failure init DRIVER_RH_RF95");
    }

#ifdef FREQUENCY
    if (!rh_driver.setFrequency(FREQUENCY)) {
        Serial.println("Failure set FREQUENCY");
    }
    Serial.println("FREQUENCY");
#endif

#ifdef TX_POWER_PIN
    if(!rh_driver.setTxPower(18, false)){
        Serial.println("Failure set TX_POWER_PIN");
    }
#endif

#ifdef MODEM_CONFIG_CHOICE
    if(!rh_driver.setModemConfig(RH_RF95::MODEM_CONFIG_CHOICE)){
        Serial.println("Failure set MODEM_CONFIG_CHOICE");
    }
    Serial.println("MODEM_CONFIG_CHOICE");
#endif
#endif
#ifdef DRIVER_RH_NRF24
    if(!bcm2835_init()){
        Serial.println("Failure init bcm2835");
    }
    if (!rh_driver.init()) {
        Serial.println("Failure init DRIVER_RH_NRF24");
    }

    if (!rh_driver.setRF(RH_NRF24::DataRate250kbps, RH_NRF24::TransmitPowerm18dBm)) {
        Serial.println("Failure set DataRate250kbps, TransmitPowerm18dBm");
    }
#endif
    manager.setThisAddress(OWN_ADDRESS);
    //socket.setRf95(&rh_driver);
    socket.setManager(&manager);
    socket.setLogger(&logger);
    socket.setMqttSnMessageHandler(&mqttSnMessageHandler);
    mqttSnMessageHandler.setLogger(&logger);
    mqttSnMessageHandler.setSocket(&socket);
    if (!mqttSnMessageHandler.begin()) {
        Serial.println("Failure init MqttSnMessageHandler");
    } else {
        Serial.println("Started");
    }
#ifdef PING

#ifndef SIMPLEMQTTSNCLIENTTESTER
    mqttSnMessageHandler.send(&target_address, msg, (uint16_t) msg[0]);
#ifdef RH_RF95_h
    mqttSnMessageHandler.send(&target_address, msg, (uint16_t) msg[0]);
#endif

#else
    mqttSnMessageHandler.send_pingreq(&target_address);
#ifdef RH_RF95_h
    mqttSnMessageHandler.send_pingreq(&target_address);
#endif

#endif

#endif
}

void loop() {
    mqttSnMessageHandler.loop();
}

#ifndef Arduino_h
bool run;

/* Signal the end of the software */
void sigint_handler(int signal) {
    run = false;
}


int main(int argc, char **argv) {
    run = true;

    signal(SIGINT, sigint_handler);

    setup();

    while (run) {
        loop();
        usleep(1);
    }

    return EXIT_SUCCESS;
}
#endif


