#include "Wire.h"   
#include <ESP8266WiFi.h>
#include <WiFiClient.h>

#include "gpio.h"
extern "C" {
#include "user_interface.h"
  bool wifi_set_sleep_type(sleep_type_t);
  sleep_type_t wifi_get_sleep_type(void);
}

void     I2Cscan();
void     writeByte(uint8_t address, uint8_t data);
void     readChannels(uint8_t address, uint8_t * dest);

const uint8_t blue = 15;
const uint8_t MAX11607_ADDRESS = 0x34; // see https://datasheets.maximintegrated.com/en/ds/MAX11606-MAX11611.pdf
uint16_t ADC[4];
uint8_t data[8];

void setup() 
{
    Serial.begin(115200);
    pinMode(blue, OUTPUT);

    Wire.begin(0,2); // SDA (0), SCL (2) on ESP8285
    Wire.setClock(400000); // choose 400 kHz I2C rate
    Wire.setClockStretchLimit(25000); // 25 ms

//     initWifi();

    Serial.println("Light sleep enabled");
    wifi_set_sleep_type(LIGHT_SLEEP_T); // Enable light sleep mode to save power

    I2Cscan(); // which devices are on the bus?

    // Configure the MAX11607 for single-ended mode, use VDD for reference
    writeByte(MAX11607_ADDRESS, 0x80 | 0x02); // set up  (bit 7 = 1), use VDD as reference, internal clock, unipolar
    writeByte(MAX11607_ADDRESS, 0x06 | 0x01); // configure (bit 7 = 0), all channels, single ended (bit 0 = 1)
}

void loop() 
{

    readChannels(MAX11607_ADDRESS, &data[0]);
  
    for(uint8_t i = 0; i < 4; i++) {
      ADC[i] = (uint16_t) data[2*i + 1] << 8 | data[2*i];
    }
    
    Serial.print("ADC channel 0 = "); Serial.println((float) (ADC[0] >> 6)*3.3/1023.0, 4);
    Serial.print("ADC channel 1 = "); Serial.println((float) (ADC[1] >> 6)*3.3/1023.0, 4);   
    Serial.print("ADC channel 2 = "); Serial.println((float) (ADC[2] >> 6)*3.3/1023.0, 4);   
    Serial.print("ADC channel 3 = "); Serial.println((float) (ADC[3] >> 6)*3.3/1023.0, 4);
        
    digitalWrite(blue, 1);
    delay(100);
    digitalWrite(blue, 0);
    delay(100);
    digitalWrite(blue, 1);
    delay(100);
  
    digitalWrite(blue, 0);
    delay(700);

}

/* Useful function */

void initWifi() {
  const char* ssid     = "xxxxxxxx";
  const char* password = "xxxxxxxxx";
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

  // simple function to scan for I2C devices on the bus
void I2Cscan() 
{
    // scan for i2c devices
  byte error, address;
  int nDevices;

  Serial.println("Scanning...");

  nDevices = 0;
  for(address = 1; address < 127; address++ ) 
  {
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();

    if (error == 0)
    {
      Serial.print("I2C device found at address 0x");
      if (address<16) 
        Serial.print("0");
      Serial.print(address,HEX);
      Serial.println("  !");

      nDevices++;
    }
    else if (error==4) 
    {
      Serial.print("Unknown error at address 0x");
      if (address<16) 
        Serial.print("0");
      Serial.println(address,HEX);
    }    
  }
  if (nDevices == 0)
    Serial.println("No I2C devices found\n");
  else
    Serial.println("done\n");
}


  // I2C read/write functions for the MAX11607
    void writeByte(uint8_t address, uint8_t data)
{
    Wire.beginTransmission(address);  // Initialize the Tx buffer
    Wire.write(data);                 // Put data in Tx buffer
    Wire.endTransmission();           // Send the Tx buffer
}

    void readChannels(uint8_t address, uint8_t * dest)
    {
    Wire.beginTransmission(address);   // Initialize the Tx buffer
    Wire.endTransmission(false);       // Send the Tx buffer, but send a restart to keep connection alive
    delay(1); // wait for conversion
    uint8_t data[8] = {0, 0, 0, 0, 0, 0, 0, 0};
    uint8_t i = 0;
    Wire.requestFrom(address, 8);  // Read bytes from slave register address 
    while (Wire.available()) 
    {
    dest[i++] = Wire.read(); // Put read results in the Rx buffer
    }

    }
