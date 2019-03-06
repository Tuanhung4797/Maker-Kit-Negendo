#include "Arduino.h"
#include "MakerKit.h"

void MakerKit::relay(int pin, int status) 
{
    //status = 1 --> close relay 
    //status = 0 --> open relay
    pinMode(pin, OUTPUT);
    if(status)
        digitalWrite(pin, HIGH);
    else
        digitalWrite(pin, LOW);
}
void MakerKit::setLED(int pin, int status)
{
    pinMode(pin, OUTPUT);
    if(status)
        digitalWrite(pin, HIGH);
    else
        digitalWrite(pin, LOW);
}
bool MakerKit::buttonPressed(int pin)
{
    pinMode(pin, INPUT);
    int statusButton = digitalRead(pin);
    if(statusButton)
        return 0;
    else
        return 1;
}
bool MakerKit::readMicroswitch(int pin)
{
    pinMode(pin, INPUT);
    int statusSwitch = digitalRead(pin);
    if(statusSwitch)
        return 0;
    else
        return 1;
}
int MakerKit::getGasSensor(int pin)
{
    int gas_value = analogRead(pin);
    gas_value = map(gas_value,0,150,0,100);
    return gas_value;
}
bool MakerKit::readTouch(int pin)
{
    pinMode(pin, INPUT);
    int touched = digitalRead(pin);
    if(touched)
        return 1;
    else 
        return 0;
}
int MakerKit::getSoilMoisture(int pin)
{
    int moisture_value = analogRead(pin);
    moisture_value = map(moisture_value,0,300,0,100);
    return moisture_value;
}
int MakerKit::getSound(int pin)
{
    int sound_value = analogRead(pin);
    return sound_value;
}
int MakerKit::getLight(int pin)
{
    int light_value = analogRead(pin);
    light_value = map(light_value,0,1024,0,100);
    return light_value;
}
bool MakerKit::readVibration(int pin)
{
    int vib_value = digitalRead(pin);
    return vib_value;
}
int MakerKit::getAcceleromenterValue(int axis)
{
    /*
        axis = 0 ---> axis X
        axis = 1 ---> axis Y
        axis = 2 ---> axis Z
    */
    int axis_value;
    if(axis == 0)
        axis_value = analogRead(A0);
    else if(axis == 1)
        axis_value = analogRead(A1);
    else if(axis ==2)
        axis_value = analogRead(A2);
    return axis_value;
}
int MakerKit::getPotentiomenterLocation(int pin)
{
    int location = analogRead(pin);
    location = map(location,0,1024,0,100);
    return location;
}
int MakerKit::getTemperature(int pin)
{

}
int MakerKit::getHumidity(int pin)
{

}
void MakerKit::stopM1()
{
    pinMode(M1A, OUTPUT);
    pinMode(M1B, OUTPUT);
    analogWrite(M1A, 0);
    analogWrite(M1B, 0);
}
void MakerKit::stopM2()
{
    pinMode(M2A, OUTPUT);
    pinMode(M2B, OUTPUT);
    analogWrite(M2A, 0);
    analogWrite(M2B, 0);
}
void MakerKit::runM1(int speed)
{
    pinMode(M1A, OUTPUT);
    pinMode(M1B, OUTPUT);
    speed = speed > 255 ? 255 : speed;   // if speed > 255 -> speed = 255
    speed = speed < -255 ? -255 : speed; // if speed < -255 -> speed = -255
     if(speed >= 0) {
        analogWrite(M1A,0);
        analogWrite(M1B,speed);
    } 
    else {
        analogWrite(M1B,0);
        analogWrite(M1A,-speed);
    }
}
void MakerKit::runM2(int speed)
{
    pinMode(M2A, OUTPUT);
    pinMode(M2B, OUTPUT);
    speed = speed > 255 ? 255 : speed;   // if speed > 255 -> speed = 255
    speed = speed < -255 ? -255 : speed; // if speed < -255 -> speed = -255
     if(speed >= 0) {
        analogWrite(M2A,0);
        analogWrite(M2B,speed);
    } 
    else {
        analogWrite(M2B,0);
        analogWrite(M2A,-speed);
    }
}
void MakerKit::setMotor(int M, int speed) //speed in units of %
{
    int speedMotor = speed*255/100;
    if(M)
        runM1(speedMotor);
    else 
        runM2(speedMotor);
}
void MakerKit::stopMotor(int M)
{
    if(M)
        stopM1();
    else
        stopM2();
}
void MakerKit::setServo(int pin, int angle)
{

}
void MakerKit::disableServo(int pin)
{

}