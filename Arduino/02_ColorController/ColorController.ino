int redLed = 5; //D1
int greenLed = 0; //D3
int blueLed = 2; //D4

void setup()
{
  Serial.begin(115200);
	delay(25);

 	pinMode(redLed, OUTPUT);
 	pinMode(greenLed, OUTPUT);
 	pinMode(blueLed, OUTPUT);

 	//Comenzar la ejecución con el LED apagado
 	digitalWrite(redLed, LOW);
 	digitalWrite(greenLed, LOW);
 	digitalWrite(blueLed, LOW);
}

void loop()
{
	setLed(255, 255, 255);
  colorWheel(35);
}

bool setLed(int r, int g, int b)
{
	analogWrite(redLed, r);
	analogWrite(greenLed, g);
	analogWrite(blueLed, b);
}

void colorWheel(int wait)
{
  int increase, decrease;
  for(int i = 1; i < 768; i++)
  {
    increase = i % 255;
    decrease = 255 - increase;

    switch(i / 255)
    {
      case 0:
        setLed(decrease, increase, 0);
        Serial.print(decrease);
        Serial.print(",");
        Serial.print(increase);
        Serial.print(",");
        Serial.println("0");
        delay(wait);
        break;
      case 1:
        setLed(0, decrease, increase);
        Serial.print("0");
        Serial.print(",");
        Serial.print(decrease);
        Serial.print(",");
        Serial.println(increase);
        delay(wait);
        break;
      case 2:
        setLed(increase, 0, decrease);
        Serial.print(increase);
        Serial.print(",");
        Serial.print("0");
        Serial.print(",");
        Serial.println(decrease);
        delay(wait);
        break;
    }
  }  
}

