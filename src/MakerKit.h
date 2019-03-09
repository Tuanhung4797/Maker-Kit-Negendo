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
        6 - LCD I2C
Author: Tuan Hung
NEGENDO Education
*/

#ifndef Makerkit_H_
#define Makerkit_H_

#include "Arduino.h"
#include "stdint.h"

#include "Scratch.h"
#include "DHT.h"
#include "Servo.h"

#define DEBUG 1
//#define DEBUG_SERIAL 1

//#define DHTTYPE DHT11
//#define DHTPIN 2
#define M1A 3
#define M1B 5
#define M2A 6
#define M2B 11

//////// define State
#define READ_SERIAL 0 
#define PARSING 1
#define WRITE_SERIAL 2

class MakerKit
{
public:
    MakerKit(){}
    void runM1(int speed);
    void runM2(int speed);
    void stopM1();
    void stopM2();
    void setMotor(int M, int speed);
    void stopMotor(int M);
    void relay(int pin, int status);
    void setLED(int pin, int status);
    void setServo(int pin, int angle);
    void disableServo(int pin);
    bool buttonPressed(int pin);
    bool readMicroswitch(int pin);
    int getGasSensor(int pin);
    float getTemperature(int pin);
    float getHumidity(int pin);
    bool readTouch(int pin);
    bool readVibration(int pin);
    int getSoilMoisture(int pin);
    int getSound(int pin);
    int getLight(int pin);
    int getAcceleromenterValue(int axis);
    int getPotentiomenterLocation(int pin);
    //////////////////////////////////////////

    void run();
    void readSerial();
    void parseData();
    void writeSerial();

private:

    int State = 0;
    bool first_run = true;
    bool actionDone = false;
    double timeStart;
    bool isAvailable = false;
    bool isStart=false;
    unsigned char prevc=0;
    int index = 0;
    int dataLen;
    int ind = 0;
    unsigned char buffer[32]; // reading
    unsigned char serial_buf[12]; // writing
    unsigned char serialRead;
    uint8_t command_index = 0;

    union
    {
        byte byteVal[4];
        float floatVal;
        long longVal;
    }val;

    union
    {
        byte byteVal[8];
        double doubleVal;
    }valDouble;

    union
    {
        byte byteVal[2];
        short shortVal;
    }valShort;
    /////////////////////////////////////////
    void writeHead();
    void writeEnd();
    /////////////////////////////////////////
    unsigned char readBuffer(int index);  //read Serial Comming buffer
    void writeBuffer(int index,unsigned char c); //write to Serial Sending Buffer
    void clearBuffer(unsigned char *buf, int leng);
    /////////////////////////////////
    void callOK();
    void sendByte(char c);
    void sendString(String s);
    void sendFloat(float value);
    void sendShort(double value);
    void sendDouble(double value);
    short readShort(int idx);
    float readFloat(int idx);
    long readLong(int idx);

    void runFunction(int device);
    int searchServoPin(int pin);
    void readSensors(int device);
};
#endif