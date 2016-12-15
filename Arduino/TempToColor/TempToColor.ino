int wheelPosition;

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
  int tmax = 310;
  int tmin = 250;
  
  //map devuelve una salida proporcional (0, 255) a los valores de entrada tmin, tmax. (Regla de tres)
  //constrain devuelve el valor original del número cuando este no excede los valores tmax o tmin.
  //en caso de excederlos, devuelve dichos valores en lugar del valor original. 
  
  wheelPosition = map(constrain(analogRead(0), tmin, tmax), tmin, tmax, 0, 255); 
  Serial.println(wheelPosition);
  blueRedWheel(wheelPosition);
  delay(70);
}

void setLed(int r, int g, int b)
{
  analogWrite(redLed, r);
  analogWrite(greenLed, g);
  analogWrite(blueLed, b);
  return;
}

void blueRedWheel(int pos)
{
  int increase, decrease;
  increase = pos % 255;
  decrease = 255 - increase;

  setLed(increase, 0, decrease);
  return;
}

