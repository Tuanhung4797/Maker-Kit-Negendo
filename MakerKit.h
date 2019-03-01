/*
https://negendo.com

Project: Maker Kit for education
Hardware: Arduino Uno R3 + Maker shield Uno made by NEGENDO
    Sensor:
        1 - Temperature Humidity sensor - DHT11
        2 - Gas sensor - MQ-2
        3 - Touch sensor
        4 - Soil moisture sensor
        5 - Vibration sensor
        6 - Light sensor
        7 - Sound sensor
        8 - Triple axis acceleromenter sensor
        9 - Rotation potentiomenter sensor
    Module:
        1 - Microswitch
        2 - Push button
        3 - LED Light
        4 - Relay module
        5 - Module 2 motor
Author: Tuan Hung
NEGENDO Education
*/

#ifndef Makerkit_H_
#define Makerkit_H_

#include "Arduino.h"
#include "stdint.h"

#include "DHT.h"

#define DHTTYPE DHT11
#define DHTPIN 2
#define M1A 3
#define M1B 5
#define M2A 6
#define M2B 11

class MakerKit
{
public:
    MakerKit(){}
    void runM1(int speed);
    void runM2(int speed);
    void stopM1();
    void stopM2();
    void setMotor(int M, int speed);
    void relay(int pin, int status);
    void setLED(int pin, int status);
    bool readButton(int pin);
    bool readMicroswitch(int pin);
    int getGasSensor(int pin);
    int getTemperature(int pin);
    int getHumidity(int pin);
    bool readTouch(int pin);
    int getSoilMoisture(int pin);
    int getSound(int pin);
    int getLight(int pin);
    int getAcceleromenterValue(int axis);
    int getPotentiomenterLocation(int pin);
private:
    /* data */
    DHT dht = DHT(DHTPIN, DHTTYPE);
};
#endif