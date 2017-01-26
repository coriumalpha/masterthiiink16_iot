/**
 * Basic PN532 NFC reading&forwarding via WiFi Http Client
 *
 *  Created on: 2017-01-25
 *
 */
 int greenLed = 16;
 int redLed = 16;

void setup() {


  pinMode(greenLed, OUTPUT);
  pinMode(redLed, OUTPUT);

}

void loop() {

statusIndicator(true);
delay(3000);
digitalWrite(redLed, HIGH); 
delay(150);
digitalWrite(redLed, LOW);
delay(150);
digitalWrite(redLed, HIGH); 
delay(150);
digitalWrite(redLed, LOW);

  
}


void statusIndicator(bool type)
{
  long now;
  bool ledStatus = true;
  
  if(type)
  {
    for(int i = 0; i < 4; i++)
    {
      now = millis();
      while(millis() - now < 500)
      {
        digitalWrite(greenLed, (ledStatus) ? HIGH : LOW);
      }
      ledStatus = !ledStatus;
    }
    digitalWrite(greenLed, LOW);    
  }
  else
  {
    for(int i = 0; i < 4; i++)
    {
      now = millis();
      while(millis() - now < 500)
      {
        digitalWrite(redLed, (ledStatus) ? HIGH : LOW);
      }
      ledStatus = !ledStatus;
    }
    digitalWrite(redLed, LOW); 
  }
  
  return;  
}




