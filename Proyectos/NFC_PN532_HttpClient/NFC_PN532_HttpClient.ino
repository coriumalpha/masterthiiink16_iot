/**
 * Basic PN532 NFC reading&forwarding via WiFi Http Client
 *
 *  Created on: 2017-01-25
 *
 */

#include <PN532_HSU.h>
#include <PN532.h>
   
#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

#include <ESP8266HTTPClient.h>

#define USE_SERIAL Serial

ESP8266WiFiMulti WiFiMulti;

PN532_HSU pn532hsu(Serial);
PN532 nfc(pn532hsu);

/*  [BEGIN]
* Configuration
*/

const bool debug = true;

const String serverIp = "10.100.200.247";
const String apiPath = "/node/api.php";
const String varName = "data";
const char* essid = "AVG";
const char* wpass = "DDDDDAAAAA00000DDDDDAAAAA00000";
const int greenLed = 16;
const int redLed = 16;

/*  [END]
* Configuration
*/

String cardIdentifier = "";

void sendCard(String data, String serverIp, String path, String varName);
void statusIndicator(bool type);

void setup() {

  if(debug) { USE_SERIAL.println("System start"); }

    USE_SERIAL.begin(115200);

    USE_SERIAL.println();
    USE_SERIAL.println();
    USE_SERIAL.println();

  if(debug)
  { 
      for(uint8_t t = 4; t > 0; t--)
      {
          USE_SERIAL.printf("[SETUP] WAIT %d...\n", t);
          USE_SERIAL.flush();
          delay(1000);
      }
  }
  
  WiFiMulti.addAP(essid, wpass);

  if(debug) { USE_SERIAL.println("PN532 startup"); }

  nfc.begin();

  uint32_t versiondata = nfc.getFirmwareVersion();
  if (! versiondata) {
    if(debug) { USE_SERIAL.print("Didn't find PN53x board"); }
    while (1); // halt
  }

  if(debug)
  { 
    // Got ok data, print it out!
    USE_SERIAL.print("Found chip PN5"); USE_SERIAL.println((versiondata>>24) & 0xFF, HEX); 
    USE_SERIAL.print("Firmware ver. "); USE_SERIAL.print((versiondata>>16) & 0xFF, DEC); 
    USE_SERIAL.print('.'); USE_SERIAL.println((versiondata>>8) & 0xFF, DEC);
  }
  // Set the max number of retry attempts to read from a card
  // This prevents us from waiting forever for a card, which is
  // the default behaviour of the PN532.
  nfc.setPassiveActivationRetries(0xFF);

  // configure board to read RFID tags
  nfc.SAMConfig();

  if(debug) { USE_SERIAL.println("Waiting for an ISO14443A card"); }

  pinMode(greenLed, OUTPUT);
  pinMode(redLed, OUTPUT);

}

void loop() {

  boolean success;
  uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };  // Buffer to store the returned UID
  uint8_t uidLength;                        // Length of the UID (4 or 7 bytes depending on ISO14443A card type)

  // Wait for an ISO14443A type cards (Mifare, etc.).  When one is found
  // 'uid' will be populated with the UID, and uidLength will indicate
  // if the uid is 4 bytes (Mifare Classic) or 7 bytes (Mifare Ultralight)
  success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, &uid[0], &uidLength);

  if (success)
  {
    if(debug)
    { 
        USE_SERIAL.println("Found a card!");
        USE_SERIAL.print("UID Length: ");USE_SERIAL.print(uidLength, DEC);USE_SERIAL.println(" bytes");
        USE_SERIAL.print("UID Value: ");
    }

      for (uint8_t i=0; i < uidLength; i++) 
      {
        if(debug) { USE_SERIAL.print(" 0x");USE_SERIAL.print(uid[i], HEX); }
        cardIdentifier += String(uid[i], HEX); 
      }

    if(debug) { USE_SERIAL.println("cardIdentifier -> " + cardIdentifier); USE_SERIAL.println(); }
    sendCard(cardIdentifier, serverIp, apiPath, varName);
    cardIdentifier = "";
    // Wait 1 second before continuing
    delay(1000);
  }
  else
  {
    // PN532 probably timed out waiting for a card
    if(debug) { USE_SERIAL.println("Timed out waiting for a card"); }
  }


  
}


void sendCard(String data, String serverIp, String path, String varName)
{
    // wait for WiFi connection
    if((WiFiMulti.run() == WL_CONNECTED)) {

        HTTPClient http;

        if(debug) { USE_SERIAL.print("[HTTP] begin...\n"); }

        http.begin("http://"+ serverIp + path + "?" + varName + "=" + data); //HTTP

        if(debug) { USE_SERIAL.print("[HTTP] GET...\n"); }

        // start connection and send HTTP header
        int httpCode = http.GET();

        // httpCode will be negative on error
        if(httpCode > 0)
        {
            // HTTP header has been send and Server response header has been handled
            if(debug) { USE_SERIAL.printf("[HTTP] GET... code: %d\n", httpCode); }

            // file found at server
            if(httpCode == HTTP_CODE_OK)
            {
                String payload = http.getString();
                if(debug) { USE_SERIAL.println(payload); }
                if(payload == "ACCESS_GRANTED")
                {
                  //Success function call
                  statusIndicator(true);
                }
                else
                {
                  //Fail function call
                  statusIndicator(false);
                }
            }
        }
        else
        {
            if(debug) { USE_SERIAL.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str()); }
        }

        http.end();
    }
    return;
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
        digitalWrite(greenLed, ledStatus);
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
        digitalWrite(redLed, ledStatus);
      }
      ledStatus = !ledStatus;
    }
    digitalWrite(redLed, LOW); 
  }
  
  return;  
}




