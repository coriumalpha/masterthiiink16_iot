#include <ESP8266WiFi.h>
 
//const char* ssid = "Nombre_De_Red[BSSID]";
//const char* password = "Password";
const char* ssid = "AVG";
const char* password = "DDDDDAAAAA00000DDDDDAAAAA00000";

int redLed = 5; //D1
int greenLed = 0; //D3
int blueLed = 2; //D4

WiFiServer server(80);

void setup() {

  Serial.begin(115200);
  delay(10);

  pinMode(redLed, OUTPUT);
  pinMode(greenLed, OUTPUT);
  pinMode(blueLed, OUTPUT);

  digitalWrite(redLed, LOW);
  digitalWrite(greenLed, LOW);
  digitalWrite(blueLed, LOW);

  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
 
  // Start the server
  server.begin();
  Serial.println("Server started");
 
  // Print the IP address
  Serial.print("Use this URL to connect: ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");
  
}

void loop() {
  
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
 
  // Wait until the client sends some data
  Serial.println("new client");
  long now = millis();
  while(!client.available()){
    delay(1);
    if((millis() - now) > 7500) //timeout
    {
      client.flush();
      break;
    }
  }
 
  // Read the first line of the request
  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();

  if(request.indexOf("/?") != -1) //GET has data
  {
    if(request.indexOf("=") != -1 && request.indexOf("&") != -1)
    {    
      String firstParam = request.substring(request.indexOf("=") + 1, request.indexOf("&"));
      int secondParamBegin = request.indexOf("&") + 1;
      String secondParam = request.substring(request.indexOf("=", secondParamBegin) + 1, request.indexOf("&", secondParamBegin));
      int thirdParamBegin = request.indexOf("&", secondParamBegin) + 1;
      String thirdParam = request.substring(request.indexOf("=", thirdParamBegin) + 1, request.indexOf("&", thirdParamBegin));

      setLed(firstParam.toInt(), secondParam.toInt(), thirdParam.toInt());
    
            // Return the response
      client.println("HTTP/1.1 200 OK");
      client.println("Content-Type: application/json");
      client.println("Access-Control-Allow-Origin: *");
      client.println(""); //  do not forget this one
    
      client.print("{\"operation\": \"success\"}");
      client.println("");
    
      delay(1);
      Serial.println("Client disonnected");
      Serial.println("");
    }
  }
  else if(request.indexOf("/") != -1)
  {
    String apiCall = request.substring(request.indexOf("/") + 1, request.indexOf(" HTTP/1.1"));
    if(String("getTemp").equals(apiCall))
    {
      sendTemperature(client);
    }
    else if(String("getColorInfo").equals(apiCall))
    {
      Serial.println("Color info request");
    }
    else
    {
      Serial.println("Undefined api call");
    }
  }
}

void setLed(int r, int g, int b)
{
  analogWrite(redLed, r);
  analogWrite(greenLed, g);
  analogWrite(blueLed, b);
}

void sendTemperature(WiFiClient client)
{
 // Return the response
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: application/json");
  client.println("Access-Control-Allow-Origin: *");
  client.println(""); //  do not forget this one

  client.print("{\"analogReading\": \""); //", "analogReading": "
  client.print(String(getMeasurement()));
  client.print("\"}");
  client.println("");

  delay(1);
  Serial.println("Client disonnected");
  Serial.println("");  
}

float getMeasurement()
{
  float temperature = (float(analogRead(0)) / 10);
  return temperature;
}
