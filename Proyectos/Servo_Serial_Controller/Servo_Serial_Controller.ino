#include <Servo.h>

Servo servo;

void setup() {
  Serial.begin(115200); //Importante -> Monitor Serial -> "Sin ajuste de línea / 115200 baudios"
  servo.attach(5); //D1
}

void loop() {

  setPosition(getPosition());
  delay(10);

}

int getPosition()
{
  int userInput;
  
  tryAgain:
  Serial.print("Introduzca posición [0, 180]: ");
  
  while(!Serial.available())
  {
    //Esperamos datos en el búffer serial  
  }

  if(Serial.available() > 0)
  {
      userInput = Serial.parseInt();
      
      if(userInput >= 0 && userInput <= 180)
      {
        return userInput;
      }
      else
      {
        Serial.println("Datos de entrada incorrectos.");
        goto tryAgain;
      }
  }
}

void setPosition(int pos)
{
    Serial.println("Pointing " + String(pos) + " degrees.");
    servo.write(pos);
    return;
}
