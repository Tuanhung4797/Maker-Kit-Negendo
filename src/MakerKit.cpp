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
/////////////////////////////////////

void MakerKit::run()
{
    if(first_run)
    {
        timeStart = millis();
        #ifdef DEBUG
            Serial.println(String("State No: ") + State + String(" Running..."));
        #endif
        first_run = false;
    }
    switch (State)
    {
        case READ_SERIAL:
        {
            readSerial();
        }
        break;
        case PARSING:
        {
            parseData();
        }
        break;
        case WRITE_SERIAL:
        {
            writeSerial();
        }
        break;
    }
}
void MakerKit::readSerial()
{
    if(Serial.available() > 0){
        isAvailable = true;
        serialRead = Serial.read();

        unsigned char c = serialRead & 0xff;
        if(c==0x55 && isStart == false){
            if(prevc == 0xff){
                index = 1;
                isStart = true;
            }
        }
        else{
            prevc = c;
            if(isStart){
                if(index == 2){
                    dataLen = c;
                    #ifdef DEBUG_SERIAL
                        Serial.print(c,HEX);
                        Serial.print(" ");
                    #endif
                }
                else if(index > 2){
                    dataLen--;
                    #ifdef DEBUG_SERIAL
                        Serial.print(dataLen);
                        Serial.print(" ");
                    #endif
                }
            }
        }
        buffer[index] = c;
        index++;
        if(index > 51){
            index = 0;
            isStart = false;
        }
        if(isStart && dataLen == 0 && index > 3){
            isStart = false;
            State = PARSING; //Serial Data available now, State change to parsing 
            first_run = true; //set first run for next State
            #ifdef DEBUG
                Serial.print("Valid Data coming ");
                for(int i=0; i < index+1; i++){
                    Serial.print(buffer[i],HEX);
                    Serial.print("-");
                }
                Serial.println();
                Serial.println("Goto Parsing");
            #endif
            index = 0;
        } 
    }
}
void MakerKit::parseData()
{
    ind = 0;
    isStart = false;
    int idx = buffer[3];
    command_index = (uint8_t)idx;
    int action = buffer[4];
    int device = buffer[5];
    switch(action){
        case GET:{
            if(device != ULTRASONIC_SENSOR){
                writeHead();
                writeBuffer(ind++,idx);
            }
            readSensors(device);
            writeEnd();
            State = WRITE_SERIAL;
            first_run = true;
        }
        break;
        case RUN:{
            runFunction(device);
            callOK();
            #ifdef DEBUG
                Serial.println("Scratch command done, go to send response");
            #endif
            State = WRITE_SERIAL;
            first_run = true;
        }
        break;
        case RESET:{
            callOK();
            State = WRITE_SERIAL;
            first_run = true;
        }
        break;
        default:
            callOK();
            State = WRITE_SERIAL;
            first_run = true;
        break;
    }
    clearBuffer(buffer,sizeof(buffer));  //clear receiving buffers
}
void MakerKit::writeSerial()
{
    for(int i=0; i<ind+1; i++){
        Serial.write(serial_buf[i]);
        delayMicroseconds(100);
    }
    clearBuffer(buffer,sizeof(buffer));
    clearBuffer(serial_buf,sizeof(serial_buf));
    State = READ_SERIAL;
}
void MakerKit::runFunction(int device)
{
    //0xff 0x55 0x6 0x0 0x1 0xa 0x9 0x0 0x0 0xa
    //int port = buffer[6];
    //int pin = port;
    switch(device){
        case SET_MOTOR:{
            int motor = readShort(6);
            int speed = readShort(8);
            setMotor(motor,speed);
            break;
        }
        case STOP_MOTOR:{
            int motor = readShort(6);
            stopMotor(motor);
            break;
        }
        case SET_RELAY:{
            int pin = readShort(6);
            int status = readShort(8);
            relay(pin,status);
            break;
        }
        case SET_LED:{
            int pin = readShort(6);
            int status = readShort(8);
            setLED(pin,status);
            break;
        }
        case SET_SERVO:{
            int pin = readShort(6);
            int angle = readShort(8);
            setServo(pin,angle);
            break;
        }
        case DIS_SERVO:{
            int pin = readShort(6);
            disableServo(pin);
            break;
        }
        case LCD_PRINT:{
            int line = readShort(6);
            int pos = readShort(8);
            
            break;
        }
        case LCD_CLEAR:{

            break;
        }
    }
}
void MakerKit::readSensors(int device)
{
    /*****************Recevice*************************
      ff 55 len idx action device port slot data a
      0  1  2   3   4      5      6    7    8
    *********************Response**********************
      ff 55 idx type data \r \n
    ***************************************************/
    float value = 0.0;
    int port,slot,pin;
    port = readBuffer(6);
    pin = port;
    switch (device){
        case BUTTON:{
           uint8_t pin = readBuffer(6);
           sendByte((byte)buttonPressed(pin));
        }
        break;
        case SWITCH:{
            uint8_t pin = readBuffer(6);
            sendByte((byte)readMicroswitch(pin));
        }
        break;
        case TOUCH:{
            uint8_t pin = readBuffer(6);
            sendByte((byte)readTouch(pin));
        }
        break;
        case VIBRATION:{
            uint8_t pin = readBuffer(6);
            sendByte((byte)readVibration(pin));
        }
        break;
        case GAS:{
            uint8_t pin = readBuffer(6);
            sendShort((int)getGasSensor(pin));
        }
        break;
        case SOUND:{
            uint8_t pin = readBuffer(6);
            sendShort((int)getSound(pin));
        }
        break;
        case LIGHT:{
            uint8_t pin = readBuffer(6);
            sendShort((int)getLight(pin));
        }
        break;
        case SOIL_MOISTURE:{
            uint8_t pin = readBuffer(6);
            sendShort((int)getSoilMoisture(pin));
        }
        break;
        case POT_LOCATION:{
            uint8_t pin = readBuffer(6);
            sendShort((int)getPotentiomenterLocation(pin));
        }
        break;
        case ACCELEROMENTER:{
            uint8_t axis = readBuffer(6);
            sendShort((int)getAcceleromenterValue(axis));
        }
        break;
        case TEMPERATURE:{
            uint8_t pin = readBuffer(6);
            sendShort((int)getTemperature(pin));
        }
        case HUMIDITY:{
            uint8_t pin = readBuffer(6);
            sendShort((int)getHumidity(pin));
        }
        break;
        case DONE:{
            sendFloat(actionDone);
        }
        break;
    }
}

///////////Private method for data package
void MakerKit::clearBuffer(unsigned char *buf, int leng)
{
    for(int i=0; i<leng; i++){
        *(buf+i) = 0;
    }
}
void MakerKit::writeHead()
{
    ind = 0;
    serial_buf[ind++] = 0xff;
    serial_buf[ind++] = 0x55;
}
void MakerKit::writeEnd()
{
    serial_buf[ind++] = 0xd;
    serial_buf[ind++] = 0xa;
}
unsigned char MakerKit::readBuffer(int index){    
    return buffer[index]; 
}
void MakerKit::writeBuffer(int index,unsigned char c)
{
  serial_buf[index]=c;
}

void MakerKit::callOK()
{ 
    ind = 0;
    writeBuffer(ind++,0xff);
    writeBuffer(ind++,0x55);
    writeEnd();
}
void MakerKit::sendByte(char c)
{
    writeBuffer(ind++,1);
    writeBuffer(ind++,c);
}
void MakerKit::sendString(String s)
{
    int l = s.length();
    writeBuffer(ind++,4);
    writeBuffer(ind++,l);
    for(int i=0;i<l;i++)
    {
        writeBuffer(ind++,s.charAt(i));
    }
}
void MakerKit::sendFloat(float value)
{
    writeBuffer(ind++,0x2);
    val.floatVal = value;
    writeBuffer(ind++,val.byteVal[0]);
    writeBuffer(ind++,val.byteVal[1]);
    writeBuffer(ind++,val.byteVal[2]);
    writeBuffer(ind++,val.byteVal[3]);
}
void MakerKit::sendShort(double value)
{
    writeBuffer(ind++,3);
    valShort.shortVal = value;
    writeBuffer(ind++,valShort.byteVal[0]);
    writeBuffer(ind++,valShort.byteVal[1]);
}
void MakerKit::sendDouble(double value)
{
    writeBuffer(ind++,2);
    valDouble.doubleVal = value;
    writeBuffer(ind++,valDouble.byteVal[0]);
    writeBuffer(ind++,valDouble.byteVal[1]);
    writeBuffer(ind++,valDouble.byteVal[2]);
    writeBuffer(ind++,valDouble.byteVal[3]);
}
short MakerKit::readShort(int idx)
{
    valShort.byteVal[0] = readBuffer(idx);
    valShort.byteVal[1] = readBuffer(idx+1);
    return valShort.shortVal;
}

float MakerKit::readFloat(int idx)
{
    val.byteVal[0] = readBuffer(idx);
    val.byteVal[1] = readBuffer(idx+1);
    val.byteVal[2] = readBuffer(idx+2);
    val.byteVal[3] = readBuffer(idx+3);
    return val.floatVal;
}
long MakerKit::readLong(int idx)
{
    val.byteVal[0] = readBuffer(idx);
    val.byteVal[1] = readBuffer(idx+1);
    val.byteVal[2] = readBuffer(idx+2);
    val.byteVal[3] = readBuffer(idx+3);
    return val.longVal;
}