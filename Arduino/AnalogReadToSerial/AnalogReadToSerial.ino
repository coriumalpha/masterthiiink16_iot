float temperature;

void setup()
{
  Serial.begin(115200);
}

void loop()
{
  showTemperature();
}

void showTemperature()
{
  temperature = (float(analogRead(0)) / 10);
  Serial.print("LM35: " + String(temperature) + "C at ");
  Serial.println(String((float(millis()) / 1000)));
  delay(750);
}