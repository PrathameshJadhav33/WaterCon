#include "ThingSpeak.h"
#include <ESP8266WiFi.h>
int relayInput = 2; // the input to the relay pin
const char ssid[] = "OMKAR-PC 4865";  // network SSID
const char pass[] = "omkar2911";   // network password         
WiFiClient  client;
int statusCode=0;

//---------Channel Details---------//
unsigned long counterChannelNumber = 947610;            // Channel ID
unsigned long counterChannelNumber2 = 1006876;
const char * myCounterReadAPIKey = "BLYN7HLT3I7PVT5A"; // Read API Key
const char * myCounterReadAPIKey2 = "J969G6O99ST8CKKD";
const int FieldNumber1 = 3;  // motor
const int FieldNumber2 = 5;  // mode
const int FieldNumber3 = 1;  // level
//-------------------------------//

void setup()
{
  Serial.begin(9600);
  WiFi.mode(WIFI_STA);
  ThingSpeak.begin(client);
  pinMode(relayInput, OUTPUT); // initialize pin as OUTPUT

}

void loop()
{
  //----------------- Network -----------------//
  if (WiFi.status() != WL_CONNECTED)
  {
    Serial.print("Connecting to ");
    Serial.print(ssid);
    Serial.println(" ....");
    while (WiFi.status() != WL_CONNECTED)
    {
      WiFi.begin(ssid, pass);
      delay(5000);
    }
    Serial.println("Connected to Wi-Fi Succesfully.");
  }
  //--------- End of Network connection--------//
  
  //---------------- Field 5 ----------------//
  long mode1 = ThingSpeak.readLongField(counterChannelNumber, FieldNumber2, myCounterReadAPIKey);
  statusCode = ThingSpeak.getLastReadStatus();
  if (statusCode == 200)
  {
    if (mode1==1)
    {
      //automatic
      //---------------- Field 2 ----------------//
      long value = ThingSpeak.readLongField(counterChannelNumber2, FieldNumber3, myCounterReadAPIKey2);
      statusCode = ThingSpeak.getLastReadStatus();
      if (statusCode == 200)
      {
        Serial.print("Motor: ");
        if (value<=10)
        {
          Serial.println("Off");
          digitalWrite(relayInput, LOW); // turn relay off
        }
        else if(value>=55)
        {
          Serial.println("On");
          digitalWrite(relayInput, HIGH); // turn relay off
        }
      }
      else
      {
         Serial.println("Unable to read channel / No internet connection");
      }
    //-------------- Field 2 -------------//
    }
    else if (mode1==0)
    {
      //manual
      //---------------- Field 3 ----------------//
      long state = ThingSpeak.readLongField(counterChannelNumber, FieldNumber1, myCounterReadAPIKey);
      statusCode = ThingSpeak.getLastReadStatus();
      if (statusCode == 200) 
      {
        Serial.print("Motor: ");
        if (state==1)
        {
          Serial.println("On");
          digitalWrite(relayInput, HIGH); // turn relay on
          long value = ThingSpeak.readLongField(counterChannelNumber2, FieldNumber3, myCounterReadAPIKey2);
          statusCode = ThingSpeak.getLastReadStatus();
          if (statusCode == 200)
          {
            Serial.print("Motor: ");
            if (value<=10)
            {
              Serial.println("Off");
              digitalWrite(relayInput, LOW); // turn relay off
            }
          }
        }
        else if (state==0)
        {
          Serial.println("Off");
          digitalWrite(relayInput, LOW); // turn relay off
        }
      }
      else
      {
         Serial.println("Unable to read channel / No internet connection");
      }
    //-------------- Field 3 -------------//
    }
  }
  else
  {
    Serial.println("Unable to read channel / No internet connection");
  }
}
