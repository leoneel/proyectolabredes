#include <Wire.h>
#include <LoRa.h>
#include <Adafruit_BMP085.h>

/*************************************************** 
  This is an example for the BMP085 Barometric Pressure & Temp Sensor

  Designed specifically to work with the Adafruit BMP085 Breakout 
  ----> https://www.adafruit.com/products/391

  These displays use I2C to communicate, 2 pins are required to  
  interface
  Adafruit invests time and resources providing this open source code, 
  please support Adafruit and open-source hardware by purchasing 
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.  
  BSD license, all text above must be included in any redistribution
 ****************************************************/

// Connect VCC of the BMP085 sensor to 3.3V (NOT 5.0V!)
// Connect GND to Ground
// Connect SCL to i2c clock - on '168/'328 Arduino Uno/Duemilanove/etc thats Analog 5
// Connect SDA to i2c data - on '168/'328 Arduino Uno/Duemilanove/etc thats Analog 4
// EOC is not used, it signifies an end of conversion
// XCLR is a reset pin, also not used here

Adafruit_BMP085 bmp;
#define ss 5
#define rst 14
#define dio0 2

//10 , 9 , 2
int counter = 0;  
void setup() {
  Serial.begin(9600);
  if (!bmp.begin()) {
	Serial.println("Could not find a valid BMP085 sensor, check wiring!");
	while (1) {}
  }


  //LoRa.setPins(ss, rst, dio0);

  
  //while (!LoRa.begin(866E6)) {
  //while (!LoRa.begin(866E6)) {  
    //Serial.println(".");
    //delay(500);
  //}
if (!LoRa.begin(915E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }

  //LoRa.setSyncWord(0xF3);
  //Serial.println("LoRa Initializing OK!");
  
}
  
void loop() {
//    Serial.print("Temperature = ");
  //  Serial.print(bmp.readTemperature());
    //Serial.println(" *C");
    
    Serial.print("Pressure = ");
    Serial.print(bmp.readPressure());
    //Serial.println(" Pa");
    
    // Calculate altitude assuming 'standard' barometric
    // pressure of 1013.25 millibar = 101325 Pascal
    //Serial.print("Altitude = ");
    //Serial.print(bmp.readAltitude());
    /*Serial.println(" meters");

    Serial.print("Pressure at sealevel (calculated) = ");
    Serial.print(bmp.readSealevelPressure());
    Serial.println(" Pa");

  // you can get a more precise measurement of altitude
  // if you know the current sea level pressure which will
  // vary with weather and such. If it is 1015 millibars
  // that is equal to 101500 Pascals.
    Serial.print("Real altitude = ");
    Serial.print(bmp.readAltitude(101500));
    Serial.println(" meters");
    */
    Serial.println();



  Serial.print("Sending packet: ");

  //Send LoRa packet to receiver
  LoRa.beginPacket();
  //LoRa.print("Temperatura ");
  LoRa.print(bmp.readTemperature());
  //LoRa.print("Presion ");
  LoRa.print(bmp.readPressure());
  LoRa.endPacket();

  delay(5000);

    
}
