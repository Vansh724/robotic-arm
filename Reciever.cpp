#include <SPI.h>   //Library for Serial Peripheral Interface Protocol
#include <nRF24L01.h> //Library for Transceiver 
#include <RF24.h>  //Library for 2.4GHz Digital Communication
#include <Servo.h> //Library for servo motor

//Defining pwm pins for servo signal
#define pwm1 9  
#define pwm2 10
#define pwm3 5
#define pwm4 6

RF24 radio(7,8); // CE, CSN

const byte address[6] = "00001"; //Channel Address

Servo s1,s2,s3,s4; //Defining Object of class Servo

struct joystick //Structure containing all parameters of joystick and gyro sensor
{
    int joy1x; 
    int joy1y;
    int joy2x;
    int joy2y;
    float xAxis;
    float yAxis;
};

joystick joy;  //Object Defining of structure "joystick"

//Defining pins for motor
int MotorA1 = 3; 
int MotorA2 = 2;
int MotorB1 = 1;
int MotorB2 = 0;

void setup() {
  // Serial.begin(9600); 
  radio.begin(); //To begin radio communication
  radio.openReadingPipe(0, address); //To read incoming data from channel
  radio.setPALevel(RF24_PA_MIN); //To set power amplifier level
  radio.startListening(); //To make transceiver act as a receiver

  //Defining servo is attached to which pin
  s1.attach(pwm1);
  s2.attach(pwm2);
  s3.attach(pwm3);
  s4.attach(pwm4);

  //Defining mode for motor pins
  pinMode(MotorA1,OUTPUT);
  pinMode(MotorA2,OUTPUT);
  pinMode(MotorB1,OUTPUT);
  pinMode(MotorB2,OUTPUT); 
}


void loop() {
  Serial.println(radio.available()); //Checking whether data is available or not
  if (radio.available()) {
    radio.read(&joy, sizeof(joy));  //Reading incoming data from transmitter
//mapping of joystick data to servo angle
    int o1 = map(joy.joy1y,0,1023,0,180);  //base 9
    int o2 = map(joy.joy1x,0,1023,30,90);  //elbow 10
    int o3 = map(joy.joy2y,0,1023,30,110);  //shoulder 5
    int o4 = map(joy.joy2x,0,1023,30,70);  //claw 6

    //Giving angles to servo 
    s1.write(o1);
    s2.write(o2);
    s3.write(o3);
    s4.write(o4);

    //Checking incoming data and mapping of joystick with servo is correct or not
    Serial.print("Joy1x ");
    Serial.print(joy.joy1x);
    Serial.print("\t");
    Serial.print("Joy1y ");
    Serial.print(joy.joy1y);
    Serial.print("\t");
    Serial.print("Joy2x ");
    Serial.print(joy.joy2x);
    Serial.print("\t");
    Serial.print("Joy2y ");
    Serial.print(joy.joy2y);
    Serial.print("\t");
    Serial.print("X-Axis ");
    Serial.print(joy.xAxis);
    Serial.print("\t");
    Serial.print("Y-Axis ");
    Serial.println(joy.yAxis);

    //Using Gyro Sensor data for motor rotation 
    if(joy.yAxis>0.3) 
     {
         digitalWrite(MotorA1,1);
         digitalWrite(MotorA2,0);
         digitalWrite(MotorB1,1);
         digitalWrite(MotorB2,0); 
     }
     else if(joy.yAxis<-0.3)
     {
         digitalWrite(MotorA1,0);
         digitalWrite(MotorA2,1);
         digitalWrite(MotorB1,0);
         digitalWrite(MotorB2,1); 
     }
     else if(joy.xAxis>0.5)
     {
         digitalWrite(MotorA1,1);
         digitalWrite(MotorA2,0);
         digitalWrite(MotorB1,0);
         digitalWrite(MotorB2,1); 
     }
     else if(joy.xAxis<-0.5)
     {
         digitalWrite(MotorA1,0);
         digitalWrite(MotorA2,1);
         digitalWrite(MotorB1,1);
         digitalWrite(MotorB2,0); 
     }
     else
     {
        digitalWrite(MotorA1,0);
        digitalWrite(MotorA2,0);
        digitalWrite(MotorB1,0);
        digitalWrite(MotorB2,0);
     }
    // delay(1000);
  }

//Receiver Side Code