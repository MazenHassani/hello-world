#include "DynamixelMotor.h"
//Beginning of Auto generated function prototypes by Atmel Studio
//End of Auto generated function prototypes by Atmel Studio

#define SAMPLES 500
#define BUZZ_PIN 8
#define RECORD_SPEED 5

// Use this for hardware serial with tristate buffer
#define DIR_PIN 2
HardwareDynamixelInterface interface(Serial2, DIR_PIN);

// speed, between 0 and 1023
int16_t speed=512;
// communication baudrate
const long unsigned int baudrate = 1000000;

DynamixelMotor baseRotation(interface, 18);
DynamixelMotor baseTilt(interface, 9);
DynamixelMotor elbow(interface, 15);
DynamixelMotor wrist(interface, 10);
DynamixelMotor gripperRotation(interface, 2);
DynamixelMotor gripper(interface, 3);

void setup()
{
	Serial.println("Start");
	pinMode(BUZZ_PIN, OUTPUT);
	Serial.begin (9600);
	interface.begin(baudrate);
	delay(100);
// check if we can communicate with the motor
// if not, we turn the led on and stop here
	Serial.println("initilization");
	uint8_t status = 1;
	status &= baseRotation.init();
	delay(200);
	status &= baseTilt.init();
	delay(200);
	status &= elbow.init();
	delay(200);
	status &= wrist.init();
	delay(200);
	status &= gripperRotation.init();
	delay(200);
	status &= gripper.init();
	delay(200);
	if(status!=DYN_STATUS_OK)
	{
		pinMode(LED_BUILTIN, OUTPUT);
		digitalWrite(LED_BUILTIN, HIGH);
		Serial.println("ERROR");
		while(1);
	}
	Serial.println("set to joint mode");
	baseRotation.jointMode();
	delay(50);
	baseTilt.jointMode();
	delay(50);
	elbow.jointMode();
	delay(50);
	wrist.jointMode();
	delay(50);
	gripperRotation.jointMode();
	delay(50);
	gripper.jointMode();
	delay(50);

	Serial.println("set speed");
	baseRotation.speed(speed);
	delay(50);
	baseTilt.speed(speed);
	delay(50);
	elbow.speed(speed);
	delay(50);
	wrist.speed(speed);
	delay(50);
	gripperRotation.speed(speed);
	delay(50);
	gripper.speed(speed);
	delay(50);

	Serial.println("reset torque");
	baseRotation.enableTorque(0);
	delay(50);
	baseTilt.enableTorque(0);
	delay(50);
	elbow.enableTorque(0);
	delay(50);
	wrist.enableTorque(0);
	delay(50);
	gripperRotation.enableTorque(0);
	delay(50);
	gripper.enableTorque(0);
	delay(50);
}

uint16_t position [6*SAMPLES] = {0};
int Size = 0;

void loop()
{
	delay(1000);
	Serial.println("Recording ...");
	delay(500);
	for (Size = 0; Size < SAMPLES ; ++Size)
	{
		position[Size+0*SAMPLES]=baseRotation.currentPosition();
		delay(10);
		position[Size+1*SAMPLES]=baseTilt.currentPosition();
		delay(10);
		position[Size+2*SAMPLES]=elbow.currentPosition();
		delay(10);
		position[Size+3*SAMPLES]=wrist.currentPosition();
		delay(10);
		position[Size+4*SAMPLES]=gripperRotation.currentPosition();
		delay(10);
		position[Size+5*SAMPLES]=gripper.currentPosition();
		delay(10);
		delay(RECORD_SPEED);
		//Stop Recording
		if (Serial.available())
		{
			char r = (char) Serial.read();
			Serial.println(r);
			if (r=='r')
			{
				break;
			}
		}
		Serial.println();
	}

	Serial.print("Repeating");digitalWrite(BUZZ_PIN, HIGH);delay(500);digitalWrite(BUZZ_PIN, LOW);
	Serial.print("...");delay(500);digitalWrite(BUZZ_PIN, HIGH);delay(500);digitalWrite(BUZZ_PIN, LOW);
	delay(1000);

	//Enable Torque
	Serial.println("set torque");
	baseRotation.enableTorque(1);
	delay(10);
	baseTilt.enableTorque(1);
	delay(10);
	elbow.enableTorque(1);
	delay(10);
	wrist.enableTorque(1);
	delay(10);
	gripperRotation.enableTorque(1);
	delay(10);
	gripper.enableTorque(1);
	delay(10);

	//back to init position
	baseRotation.goalPosition(position[0*SAMPLES]);
	delay(10);
	baseTilt.goalPosition(position[1*SAMPLES]);
	delay(10);
	elbow.goalPosition(position[2*SAMPLES]);
	delay(10);
	wrist.goalPosition(position[3*SAMPLES]);
	delay(10);
	gripperRotation.goalPosition(position[4*SAMPLES]);
	delay(10);
	gripper.goalPosition(position[5*SAMPLES]);
	delay(10);
	//wait to reach init position
	delay(1000);
	//Repeating
	for (int i = 1; i < Size-1; ++i)
	{
		baseRotation.goalPosition(position[i+0*SAMPLES]);
		delay(10);
		baseTilt.goalPosition(position[i+1*SAMPLES]);
		delay(10);
		elbow.goalPosition(position[i+2*SAMPLES]);
		delay(10);
		wrist.goalPosition(position[i+3*SAMPLES]);
		delay(10);
		gripperRotation.goalPosition(position[i+4*SAMPLES]);
		delay(10);
		gripper.goalPosition(position[i+5*SAMPLES]);
		delay(10);
		delay(RECORD_SPEED);
	}
	Serial.println("Finish");
	while(true);
}
