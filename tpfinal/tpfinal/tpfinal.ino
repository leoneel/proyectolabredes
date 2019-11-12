/****************************************************************
 * ReadSHT2x
 *  An example sketch that reads the sensor and prints the
 *  relative humidity to the PC's serial port
 *
 *  Tested with:
 *    - SHT21-Breakout Humidity sensor from Modern Device
 *    - SHT2x-Breakout Humidity sensor from MisensO Electronics
 ***************************************************************/

#include <Wire.h>
#include <loRa.h>
#include <Sodaq_SHT2x.h>


#define ss 5
#define rst 14
#define dio0 2

//10 , 9 , 2

int counter = 0;
void setup()
{
  Wire.begin();
  Serial.begin(9600);


  LoRa.setPins(ss, rst, dio0);

  
  //while (!LoRa.begin(866E6)) {
  while (!LoRa.begin(866E6)) {  
    Serial.println(".");
    delay(500);
  }


  //LoRa.setSyncWord(0xF3);
  //Serial.println("LoRa Initializing OK!");
  
}

void loop()
{
  Serial.print("Humidity(%RH): ");
  Serial.print(SHT2x.GetHumidity());
  Serial.print("     Temperature(C): ");
  Serial.println(SHT2x.GetTemperature());
  Serial.print("     Dewpoint(C): ");
  Serial.println(SHT2x.GetDewPoint());
  
  delay(1000);

  Serial.print("Sending packet: ");
  Serial.println(counter);

  //Send LoRa packet to receiver
  LoRa.beginPacket();
  LoRa.print("hello ");
  LoRa.print(counter);
  LoRa.endPacket();

  counter++;

  delay(10000);

  
}
