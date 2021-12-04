#include <WiFi.h>
#include <WiFiAP.h>
#include <WiFiClient.h>
#include <WiFiGeneric.h>
#include <WiFiMulti.h>
#include <WiFiScan.h>
#include <WiFiServer.h>
#include <WiFiSTA.h>
#include <WiFiType.h>
#include <WiFiUdp.h>


//LIBRERÍAS
#include <WiFi.h>
#include <HTTPClient.h>

//VARIABLES
const char* ssid = "Galaxy A20F6F8";
const char* password = "jepg3576";

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
    delay(4000);
    WiFi.begin(ssid, password);
   
    while (WiFi.status() != WL_CONNECTED) {
      delay(1000);
      Serial.println("Connecting to WiFi..");
    }
   
    Serial.println("Connected to the WiFi network");
}

void loop() {
  // put your main code here, to run repeatedly:
 if ((WiFi.status() == WL_CONNECTED)) { //Check the current connection status
 
    HTTPClient http;
 
    http.begin("https://catfact.ninja/fact"); //Specify the URL
    int httpCode = http.GET(); //Make the request
 
    if (httpCode > 0) { //Check for the returning code
 
        String payload = http.getString();
        Serial.println(httpCode);
        Serial.println(payload);
      }
 
    else {
      Serial.println("Error on HTTP request");
    }
 
    http.end(); //Free the resources
  }
 
  delay(10000);
}
