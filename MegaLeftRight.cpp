void setup()
{
	Serial.begin(9600);
	Serial2.begin(115200);
	Serial3.begin(115200);
	delay(1000);
}
int state = 0;
int Anc1, Anc2;
void loop()
{
	if (Serial2.available())
	{
		if (Serial2.read()=='#')
		{
			delay(10);
			byte h = Serial2.read();
			byte l = Serial2.read();
			Anc1 = word(h, l);
			//Serial.print("Serial 2 is : ");Serial.println(Anc1);
		}
	}
	if (Serial3.available())
	{
		if (Serial3.read()=='#')
		{
			delay(10);
			byte h2 = Serial3.read();
			byte l2 = Serial3.read();
			Anc2 = word(h2, l2);
			//Serial.print("Serial 3 is : "); Serial.println(Anc2);
		}
	}
	
	if (Anc1>Anc2)
	{
		if (state!=2)
		{
			state=2;
			//Serial.print(Anc1);Serial.print(" ");Serial.print(Anc2);Serial.print(" ");
			Serial.println("2");
		}
	}
	else
	{
		if (state!=1)
		{
			state =1;
			//Serial.print(Anc1);Serial.print(" ");Serial.print(Anc2);Serial.print(" ");
			Serial.println("1");
		}
	}
		
}