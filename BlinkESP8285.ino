#include "Wire.h"   
#include <SPI.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
//#include <ArduinoOTA.h>

#include "gpio.h"
extern "C" {
#include "user_interface.h"
  bool wifi_set_sleep_type(sleep_type_t);
  sleep_type_t wifi_get_sleep_type(void);
}

const uint8_t blue = 15;
const uint8_t yellow = 9;
const uint8_t red = 10;

void setup() 
{
  Serial.begin(115200);
  pinMode(blue, OUTPUT);
 // pinMode(red, OUTPUT);
//  pinMode(yellow, OUTPUT);

//     initWifi();

     Serial.println("Light sleep enabled");
     wifi_set_sleep_type(LIGHT_SLEEP_T); // Enable light sleep mode to save power

}

void loop() 
{
  digitalWrite(blue, 1);
  delay(100);
  digitalWrite(blue, 0);
  delay(100);
  digitalWrite(blue, 1);
  delay(100);
  
  Serial.println("test blue");
  
  digitalWrite(blue, 0);
  delay(700);

  /*
  digitalWrite(red, 1);
  delay(100);
  digitalWrite(red, 0);
  delay(100);
  digitalWrite(red, 1);
  delay(100);
  
  Serial.println("test red");
  
  digitalWrite(red, 0);
  delay(700);

   digitalWrite(yellow, 1);
  delay(100);
  digitalWrite(yellow, 0);
  delay(100);
  digitalWrite(yellow, 1);
  delay(100);
  
  Serial.println("test yellow");
  
  digitalWrite(yellow, 0);
  delay(700);
  */

}

void initWifi() {
  const char* ssid     = "yourssid";
  const char* password = "yourpasswd";
 // Connect to WiFi network
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("\n\r \n\rWorking to connect");

 // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("ESP8285 Environmental Data Server");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
  }
