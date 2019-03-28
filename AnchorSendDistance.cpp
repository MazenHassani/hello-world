//Anchor
#include <DW1000Ranging.h>

// connection pins
const uint8_t PIN_RST = 9; // reset pin
const uint8_t PIN_IRQ = 2; // irq pin
const uint8_t PIN_SS = SS; // spi select pin

double x[100] = {0};
double samplesSum = 0, MinMaxSum = 0;
int samplesAv = 0, MinMax = 0, theAv = 0;
double minSamplesAv =0, maxSamplesAv = 0;
int MinMaxCount = 0, theAvCount = 0 , RealDistance = 0;

void setup() {
	Serial.begin(115200);
	delay(1000);
	DW1000Ranging.initCommunication(PIN_RST, PIN_SS, PIN_IRQ); //Reset, CS, IRQ pin
	DW1000Ranging.attachNewRange(newRange);
	DW1000Ranging.startAsAnchor("1", DW1000.MODE_LONGDATA_RANGE_ACCURACY);
	randomSeed(analogRead(0));
}

void loop() {
	DW1000Ranging.loop();
}

void newRange() {
	minSamplesAv=maxSamplesAv=samplesAv ;
	//Filtering
	for (int i = 0; i < 100; i++)
	{
		samplesSum =samplesSum - x[i];
		x[i] = DW1000Ranging.getDistantDevice()->getRange();
		samplesSum += x[i];
	}
	samplesAv = samplesSum / 100 * 100;
	if (samplesAv < minSamplesAv)
		minSamplesAv = samplesAv;

	if (samplesAv > maxSamplesAv)
		maxSamplesAv = samplesAv;
	
	MinMax = (minSamplesAv + maxSamplesAv) / 4;
	MinMaxSum += MinMax;
	theAvCount++;
	if (theAvCount == 5)
	{
		theAvCount = 0;
		theAv = MinMaxSum / 5;
		RealDistance = (1.8004 * theAv)-(56.018);
		MinMaxSum = 0;
	}
	Serial.print('#');
	Serial.write(highByte(RealDistance));
    Serial.write(lowByte(RealDistance));
    Serial.println(RealDistance);
    delay(random(3, 10));
}