#include "DynamixelMotor.h"
//Beginning of Auto generated function prototypes by Atmel Studio
//End of Auto generated function prototypes by Atmel Studio


// id of the motor
const uint8_t id=2;
// speed, between 0 and 1023
int16_t speed=512;
// communication baudrate
const long unsigned int baudrate = 1000000;

#define MAX_SAMPLES 600

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
#define RECORD_SPEED 100

String inputString = "";
bool stringComplete = false;

void setup()
{
  pinMode(BUZZ_PIN, OUTPUT);
  Serial.begin (9600);
  Serial3.begin (9600);
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

uint16_t position [6*MAX_SAMPLES] = {0};
uint16_t memory [20][3600];
int movementSize = 0;
void loop()
{
  if (inputString=="Record")
  {
    movementSize =0 ;
    while (inputString!="Stop")
    {
      position[movementSize+0*MAX_SAMPLES]=baseRotation.currentPosition();
      position[movementSize+1*MAX_SAMPLES]=baseTilt.currentPosition();
      position[movementSize+2*MAX_SAMPLES]=elbow.currentPosition();
      position[movementSize+3*MAX_SAMPLES]=wrist.currentPosition();
      position[movementSize+4*MAX_SAMPLES]=gripperRotation.currentPosition();
      position[movementSize+5*MAX_SAMPLES]=gripper.currentPosition();
      delay(RECORD_SPEED);
      movementSize++;
      if (movementSize == 600)
      {
        digitalWrite(BUZZ_PIN, HIGH);
        break;
      }
    }
    if (inputString=="Save")
    {
      int storedMovementNumber = Serial.read();
      for (int i = 0; i < movementSize; ++i)
      {
        memory[storedMovementNumber][i]=position[i];
      }
    }
    if (inputString=="Do")
    {
      int storedMovementNumber = Serial.read();
      //Enable Torque
      baseRotation.enableTorque(1);
      baseTilt.enableTorque(1);
      elbow.enableTorque(1);
      wrist.enableTorque(1);
      gripperRotation.enableTorque(1);
      gripper.enableTorque(1);
      //back to init position
      baseRotation.goalPosition(memory[storedMovementNumber][0*MAX_SAMPLES]);
      baseTilt.goalPosition(memory[storedMovementNumber][1*MAX_SAMPLES]);
      elbow.goalPosition(memory[storedMovementNumber][2*MAX_SAMPLES]);
      wrist.goalPosition(memory[storedMovementNumber][3*MAX_SAMPLES]);
      gripperRotation.goalPosition(memory[storedMovementNumber][4*MAX_SAMPLES]);
      gripper.goalPosition(memory[storedMovementNumber][5*MAX_SAMPLES]);
      //wait to reach init position
      delay(1000);
      for (int i = 1; i < movementSize; ++i)
      {
        baseRotation.goalPosition(memory[storedMovementNumber][i+0*MAX_SAMPLES]);
        baseTilt.goalPosition(memory[storedMovementNumber][i+1*MAX_SAMPLES]);
        elbow.goalPosition(memory[storedMovementNumber][i+2*MAX_SAMPLES]);
        wrist.goalPosition(memory[storedMovementNumber][i+3*MAX_SAMPLES]);
        gripperRotation.goalPosition(memory[storedMovementNumber][i+4*MAX_SAMPLES]);
        gripper.goalPosition(memory[storedMovementNumber][i+5*MAX_SAMPLES]);
        delay(RECORD_SPEED);
      }
    }
  }
}

void serialEvent3() {
  while (Serial3.available()) {
    // get the new byte:
    char inChar = (char)Serial3.read();
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag so the main loop can
    // do something about it:
    if (inChar == '\n') {
      stringComplete = true;
    }
  }
}