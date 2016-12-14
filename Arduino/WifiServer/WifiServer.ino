#include <ESP8266WiFi.h>
 
const char* ssid = "AVG";
const char* password = "DDDDDAAAAA00000DDDDDAAAAA00000";
bool timedOut = false;

WiFiServer server(80);

void setup() {

  Serial.begin(115200);
  delay(10);

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
  timedOut = false;
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
      timedOut = true;
      break;
    }
  }
 
  
  if(timedOut == false)
  {  // Read the first line of the request
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
        
        Serial.println("Parsed first parm: " + firstParam + ", second: " + secondParam);
      
        // Return the response
        client.println("HTTP/1.1 200 OK");
        client.println("Content-Type: application/json");
        client.println("Access-Control-Allow-Origin: *");
        client.println(""); //  do not forget this one
      
        client.print("{\"" + firstParam + "\": \"" + secondParam + "\"}");
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
        Serial.println("Temperature Request");
        returnOk(client);
      }
      else
      {
        Serial.println("Undefined api call");
        returnOk(client);
      }
    }
  }

}

void returnOk(WiFiClient client)
{
 // Return the response
  client.println("HTTP/1.1 200 OK");
  client.println(""); //  do not forget this one
  delay(1);
  Serial.println("Client disonnected");
  Serial.println("");  
}
