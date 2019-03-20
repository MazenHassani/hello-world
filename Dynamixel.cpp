
#include "DynamixelMotor.h"
//Beginning of Auto generated function prototypes by Atmel Studio
//End of Auto generated function prototypes by Atmel Studio


// id of the motor
const uint8_t id=2;
// speed, between 0 and 1023
int16_t speed=512;
// communication baudrate
const long unsigned int baudrate = 1000000;

#define SAMPLES 150

// Use this for hardware serial with tristate buffer
#define DIR_PIN 2
HardwareDynamixelInterface interface(Serial2, DIR_PIN);

DynamixelMotor baseRotation(interface, 18);
DynamixelMotor baseTilt(interface, 9);
DynamixelMotor elbow(interface, 15);
DynamixelMotor wrist(interface, 10);
DynamixelMotor gripperRotation(interface, 2);
DynamixelMotor gripper(interface, 3);

#define BUZZ_PIN 8
#define RECORD_SPEED 50
void setup()
{
  pinMode(BUZZ_PIN, OUTPUT);
  Serial.begin (9600);
  interface.begin(baudrate);
  delay(100);
  // check if we can communicate with the motor
  // if not, we turn the led on and stop here
  uint8_t status = 1;
  status &= baseRotation.init();
  status &= baseTilt.init();
  status &= elbow.init();
  status &= wrist.init();
  status &= gripperRotation.init();
  status &= gripper.init();
  
  if(status!=DYN_STATUS_OK)
  {
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);
    while(1);
  }
  // set to joint mode, with a 180° angle range
  // see robotis doc to compute angle values
  baseRotation.jointMode(204, 820);
  baseTilt.jointMode(204, 820);
  elbow.jointMode(204, 820);
  wrist.jointMode(204, 820);
  gripperRotation.jointMode(204, 820);
  gripper.jointMode(204, 820);
  
  baseRotation.speed(speed);
  baseTilt.speed(speed);
  elbow.speed(speed);
  wrist.speed(speed);
  gripperRotation.speed(speed);
  gripper.speed(speed);
}

uint16_t position [6*SAMPLES] = {0};
  
void loop()
{
/*
  baseRotation.goalPosition(512);
  baseTilt.goalPosition(512);
  elbow.goalPosition(512);
  wrist.goalPosition(512);
  gripperRotation.goalPosition(512);
  gripper.goalPosition(512);
*/
  //wait till send any byte to serial
  while (!Serial.available());
  delay(500);
  
  baseRotation.enableTorque(0);
  baseTilt.enableTorque(0);
  elbow.enableTorque(0);
  wrist.enableTorque(0);
  gripperRotation.enableTorque(0);
  gripper.enableTorque(0);
  
  Serial.println("Recording ...");
  delay(500);
  for (int i = 0; i < 100; ++i)
  {
    position[i+0*SAMPLES]=baseRotation.currentPosition();
    position[i+1*SAMPLES]=baseTilt.currentPosition();
    position[i+2*SAMPLES]=elbow.currentPosition();
    position[i+3*SAMPLES]=wrist.currentPosition();
    position[i+4*SAMPLES]=gripperRotation.currentPosition();
    position[i+5*SAMPLES]=gripper.currentPosition();
    delay(RECORD_SPEED);
  }
  
  //Repeating
  Serial.print("Repeating");digitalWrite(BUZZ_PIN, HIGH);delay(500);digitalWrite(BUZZ_PIN, LOW);
  Serial.print("...");delay(500);digitalWrite(BUZZ_PIN, HIGH);delay(500);digitalWrite(BUZZ_PIN, LOW);
  delay(1000);
  //Enable Torque
  baseRotation.enableTorque(1);
  baseTilt.enableTorque(1);
  elbow.enableTorque(1);
  wrist.enableTorque(1);
  gripperRotation.enableTorque(1);
  gripper.enableTorque(1);
  
  //back to init position
  baseRotation.goalPosition(position[0*SAMPLES]);
  baseTilt.goalPosition(position[1*SAMPLES]);
  elbow.goalPosition(position[2*SAMPLES]);
  wrist.goalPosition(position[3*SAMPLES]);
  gripperRotation.goalPosition(position[4*SAMPLES]);
  gripper.goalPosition(position[5*SAMPLES]);
  
  //wait to reach init position
  delay(1000);
  for (int i = 1; i < 100; ++i)
  {
    baseRotation.goalPosition(position[i+0*SAMPLES]);
    baseTilt.goalPosition(position[i+1*SAMPLES]);
    elbow.goalPosition(position[i+2*SAMPLES]);
    wrist.goalPosition(position[i+3*SAMPLES]);
    gripperRotation.goalPosition(position[i+4*SAMPLES]);
    gripper.goalPosition(position[i+5*SAMPLES]);
    delay(RECORD_SPEED);
  }
  delay(10000);
}
