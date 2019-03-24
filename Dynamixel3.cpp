#include "DynamixelMotor.h"

// speed, between 0 and 1023
int16_t speed = 512;
// communication baudrate
const long unsigned int baudrate = 1000000; // 1 MHz

#define MAX_SAMPLES 300

// Use this for hardware serial with tristate buffer
#define DIR_PIN 2
HardwareDynamixelInterface interface(Serial2, DIR_PIN);

DynamixelMotor baseRotation(interface, 18);
DynamixelMotor baseTilt(interface, 9);
DynamixelMotor elbow(interface, 15);
DynamixelMotor wrist(interface, 10);
DynamixelMotor gripperRotation(interface, 2);
DynamixelMotor gripper(interface, 3);

#define LED_BUILTIN 8
#define RECORD_SPEED 100

String inputString = "";
bool stringComplete = false;
bool processed = false;
void setup()
{
	pinMode(LED_BUILTIN, OUTPUT);
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

	if (status != DYN_STATUS_OK)
	{
		pinMode(LED_BUILTIN, OUTPUT);
		digitalWrite(LED_BUILTIN, HIGH);
		while (1);
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

	baseRotation.enableTorque(0);
	baseTilt.enableTorque(0);
	elbow.enableTorque(0);
	wrist.enableTorque(0);
	gripperRotation.enableTorque(0);
	gripper.enableTorque(0);
}

uint16_t position [6 * MAX_SAMPLES] = {0};
uint16_t memory [1][6 * MAX_SAMPLES];
int movementSize = 0;
void loop()
{
	if (inputString == "Record#")
	{
		if (stringComplete)
		{
			Serial.println(inputString);
			// clear the string:
			inputString = "";
			stringComplete = false;
		}
		movementSize = 0 ;
		while (inputString != "Stop#")
		{
			position[movementSize + 0 * MAX_SAMPLES] = baseRotation.currentPosition();
			position[movementSize + 1 * MAX_SAMPLES] = baseTilt.currentPosition();
			position[movementSize + 2 * MAX_SAMPLES] = elbow.currentPosition();
			position[movementSize + 3 * MAX_SAMPLES] = wrist.currentPosition();
			position[movementSize + 4 * MAX_SAMPLES] = gripperRotation.currentPosition();
			position[movementSize + 5 * MAX_SAMPLES] = gripper.currentPosition();
			delay(RECORD_SPEED);
			movementSize++;
			if (movementSize == 600)
			{
				Serial.println("I stopped because I get no stop !");
				digitalWrite(LED_BUILTIN, HIGH);
				break;
			}
		}
		if (stringComplete && movementSize < 600) {
				Serial.println(inputString);
				// clear the string:
				inputString = "";
				stringComplete = false;
		}
	}
	else if (inputString == "Save#")
	{
		if (stringComplete) {
			Serial.println(inputString);
			// clear the string:
			inputString = "";
			stringComplete = false;
		}
		int storedMovementNumber = 0 ; /*Serial.read();*/
		processed=true;
		for (int i = 0; i < movementSize; ++i)
		{
			memory[storedMovementNumber][i] = position[i];
		}
	}
	else if (inputString == "Do#")
	{
		if (stringComplete) {
			Serial.println(inputString);
			// clear the string:
			inputString = "";
			stringComplete = false;
		}
		int storedMovementNumber = 0 ; /*Serial.read();*/
		//Enable Torque
		baseRotation.enableTorque(1);
		baseTilt.enableTorque(1);
		elbow.enableTorque(1);
		wrist.enableTorque(1);
		gripperRotation.enableTorque(1);
		gripper.enableTorque(1);
		//back to init position
		baseRotation.goalPosition(memory[storedMovementNumber][0 * MAX_SAMPLES]);
		baseTilt.goalPosition(memory[storedMovementNumber][1 * MAX_SAMPLES]);
		elbow.goalPosition(memory[storedMovementNumber][2 * MAX_SAMPLES]);
		wrist.goalPosition(memory[storedMovementNumber][3 * MAX_SAMPLES]);
		gripperRotation.goalPosition(memory[storedMovementNumber][4 * MAX_SAMPLES]);
		gripper.goalPosition(memory[storedMovementNumber][5 * MAX_SAMPLES]);
		//wait to reach init position
		delay(1000);
		for (int i = 1; i < movementSize; ++i)
		{
			baseRotation.goalPosition(memory[storedMovementNumber][i + 0 * MAX_SAMPLES]);
			baseTilt.goalPosition(memory[storedMovementNumber][i + 1 * MAX_SAMPLES]);
			elbow.goalPosition(memory[storedMovementNumber][i + 2 * MAX_SAMPLES]);
			wrist.goalPosition(memory[storedMovementNumber][i + 3 * MAX_SAMPLES]);
			gripperRotation.goalPosition(memory[storedMovementNumber][i + 4 * MAX_SAMPLES]);
			gripper.goalPosition(memory[storedMovementNumber][i + 5 * MAX_SAMPLES]);
			delay(RECORD_SPEED);
		}
	}
	//else Serial.println(inputString);
}

void serialEvent3() {
	while (Serial3.available()) {
		// get the new byte:
		char inChar = (char)Serial3.read();
		// add it to the inputString:
		inputString += inChar;
		// if the incoming character is a newline, set a flag so the main loop can
		// do something about it:
		if (inChar == '#') {
			stringComplete = true;
		}
	}
}

/*
String inputString = "";         // a String to hold incoming data
bool stringComplete = false;  // whether the string is complete

void setup() {
// initialize serial:
Serial.begin(9600);
// reserve 15 bytes for the inputString:
inputString.reserve(15);
}

void loop() {
// print the string when a newline arrives:
if (stringComplete) {
Serial.println(inputString);
// clear the string:
inputString = "";
stringComplete = false;
}
}

/*
SerialEvent occurs whenever a new data comes in the hardware serial RX. This
routine is run between each time loop() runs, so using delay inside loop can
delay response. Multiple bytes of data may be available.
//*
void serialEvent() {
while (Serial.available()) {
// get the new byte:
char inChar = (char)Serial.read();
// add it to the inputString:
inputString += inChar;
// if the incoming character is a newline, set a flag so the main loop can
// do something about it:
if (inChar == '\n') {
stringComplete = true;
}
}
}*/
