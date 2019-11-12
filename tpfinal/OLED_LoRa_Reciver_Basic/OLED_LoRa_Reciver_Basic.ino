#include <SPI.h>
#include <LoRa.h>
#include <Wire.h>  
#include "SSD1306.h" 
#include "images.h"
#include <ETH.h>
#include <WiFi.h>
#include <ESPmDNS.h>

#include <WiFiGeneric.h>
#include <WiFiMulti.h>
#include <WiFiType.h>
#include <WiFiUdp.h>
#include <PubSubClient.h>




#define SCK     5    // GPIO5  -- SX1278's SCK
#define MISO    19   // GPIO19 -- SX1278's MISO
#define MOSI    27   // GPIO27 -- SX1278's MOSI
#define SS      18   // GPIO18 -- SX1278's CS
#define RST     14   // GPIO14 -- SX1278's RESET
#define DI0     26   // GPIO26 -- SX1278's IRQ(Interrupt Request)
#define BAND    915E6

#define I2C_SDA                 21
#define I2C_SCL                 22

//SSD1306                         display(0x3c, 21, 22);

#define LED_PIN                 2

// nombres de topicos a los que nos suscribimos  (NUBE -> ESP32)
#define CANAL_SUSCRIPTOR_LED_INTERNO     "unlpam/ing/redes/ej001/led"      // 1=on, 0=off 

// nombres de topicos a publicar (ESP32 -> NUBE)
#define TEMP_TOPIC    "unlpam/ing/redes/ej001/temp"
#define HUMI_TOPIC    "unlpam/ing/redes/ej001/humi"
#define PRES_TOPIC    "unlpam/ing/redes/ej001/pres"

const char*       ssid = "Lab_Redes";
const char*       password = "redes6405";

WiFiClient        espClient;
PubSubClient      pubSubClient(espClient);

//const char*       mqttServer = "broker.hivemq.com"; 
const char*       mqttServer = "broker.mqtt-dashboard.com"; 

const int         mqttPort = 1883;
const char*       mqttUser = "";
const char*       mqttPassword = "";

//------------------------------------------------------------------

int             relePin = 4;





//------------------------------------------------------------------

//------------------------------------------------------------------

void WiFiReset() {
    WiFi.persistent(false);
    WiFi.disconnect();
    WiFi.mode(WIFI_OFF);
    WiFi.mode(WIFI_STA);
}

//------------------------------------------------------------------

void setup_wifi() 
{
int count=0;  
WiFiReset();  

WiFi.begin(ssid, password);

while (WiFi.status() != WL_CONNECTED) 
  {
  Serial.print("Connecting WiFi to SSID: ");
  Serial.println(ssid);
  if (++count > 5) {
    Serial.println("WiFi RESET .... ");
    WiFi.disconnect(true); delay(1000);
    WiFi.mode(WIFI_OFF);   delay(1000);
    WiFi.mode(WIFI_STA);   delay(1000);
    WiFi.begin(ssid, password);
    count=0;
  }
    
  delay(1000);
  }
Serial.println("");
Serial.println("WiFi connected. ");
Serial.print("IP address: ");
Serial.println(WiFi.localIP());
}

//------------------------------------------------------------------
void fn_ReceivedCallback(char* topic, byte* payload, unsigned int length) 
{
Serial.print("Message received: ");
Serial.println(topic);

Serial.print("payload: ");
for (int i = 0; i < length; i++) 
  Serial.print((char)payload[i]);
Serial.println();

if ((char)payload[0] == '1') 
  digitalWrite(LED_PIN, HIGH); 
else   
  digitalWrite(LED_PIN, LOW);
}

void setup_pubSub() 
{
pubSubClient.setServer(mqttServer, mqttPort);
pubSubClient.setCallback(fn_ReceivedCallback);
}

//------------------------------------------------------------------

void fn_MQTTconnect() 
{
while (!pubSubClient.connected()) 
  {
  Serial.print("MQTT connecting ...");
  String clientId = "ESP32Client";
//if (pubSubClient.connect("UNLPAM_FI_CLI_01", mqttUser, mqttPassword)) 
  if (pubSubClient.connect("UNLPAM_FI_CLI_01_Client")) 
    {
    Serial.println("connected");
    pubSubClient.subscribe(CANAL_SUSCRIPTOR_LED_INTERNO);
    } 
  else 
    {
    Serial.print("Error, status code =");
    Serial.print(pubSubClient.state());
    Serial.println(" Probar de nuevo en 5 segundos");
    delay(5000);
    }
  }
}

//------------------------------------------------------------------

// basic OLED Setup
/*
void setup_display()  {
display.init();
display.setFont(ArialMT_Plain_16);
display.setTextAlignment(TEXT_ALIGN_LEFT);
display.flipScreenVertically();
display.setContrast(255);
}
*/

//------------------------------------------------------------------
/*
void show_simple_msg_on_oled(char *msg)  {
  display.clear();
  display.drawString(0, 0, msg);
  display.display();
}
*/
//------------------------------------------------------------------

/*
  show_simple_msg_on_oled("Init. WiFi...");
  setup_wifi();
  show_simple_msg_on_oled("Init. MQTT...");
  setup_pubSub();
  show_simple_msg_on_oled("Init. BME 280...");
 */
  //bme280Setup();
/*
  pinMode(LED_PIN, OUTPUT);
  pinMode(relePin, OUTPUT);
  digitalWrite(relePin, LOW);
  */ 


//------------------------------------------------------------------


//------------------------------------------------------------------

//------------------------------------------------------------------

/*void bme_values_to_oled(float temperature, float humidity, float pressure) {

char            msg[100];

display.clear();
snprintf (msg, 20, "Temp: %.1lf", temperature);
Serial.println(msg);
display.drawString(0, 0, msg);
//PODEMOS COMENTAR
snprintf (msg, 20, "Hum: %.1lf", humidity);
Serial.println(msg);
display.drawString(0, 20, msg);
snprintf (msg, 20, "Pres: %.0lf", pressure);
Serial.println(msg);
display.drawString(0, 40, msg);

display.display();
}
*/
//------------------------------------------------------------------

void bme_values_to_mqtt(float temperature, float pressure) {

char            msg[100];

snprintf (msg, 20, "%.1lf", temperature);
pubSubClient.publish(TEMP_TOPIC, msg);
snprintf (msg, 20, "%.0lf", pressure);
pubSubClient.publish(PRES_TOPIC, msg);
}




  
//------------------------------------------------------------------

SSD1306 display(0x3c, 21, 22);
String rssi = "RSSI --";
String packSize = "--";
String packet ;
float temperature=0.0, pressure=0.0;

void read_sender(float * temperature, float * pressure)
{
  /*int packetSize = LoRa.parsePacket();
  if (packetSize) {
    // received a packet
    Serial.print("Received packet '");

    // read packet
    while (LoRa.available()) {
      
      Serial.print((char)LoRa.read());
    }
    Serial.println();
    
  }
  */
  String sub_temperature=packet.substring(0,5);
  String sub_pressure = packet.substring(5,10);

  * temperature = sub_temperature.toFloat();
  * pressure = sub_pressure.toFloat();
  Serial.println("Temp");
  Serial.println("asdasd");

  
}


void loraData(){
  display.clear();
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.setFont(ArialMT_Plain_10);
  display.drawString(0 , 15 , "Received "+ packSize + " bytes");
  display.drawStringMaxWidth(0 , 26 , 128, packet);
  display.drawString(0, 0, rssi);  
  display.display();
}

void cbk(int packetSize) {
  packet ="";
  packSize = String(packetSize,DEC);
  for (int i = 0; i < packetSize; i++) { packet += (char) LoRa.read(); }
  rssi = "RSSI " + String(LoRa.packetRssi(), DEC) ;
  Serial.println(packet);
  loraData();
  
}

void setup() {
  pinMode(16,OUTPUT);
  digitalWrite(16, LOW);    // set GPIO16 low to reset OLED
  delay(50); 
  digitalWrite(16, HIGH); // while OLED is running, must set GPIO16 in high、
  
  Serial.begin(9600);
  while (!Serial);
  Serial.println();
  Serial.println("LoRa Receiver Callback");
  SPI.begin(SCK,MISO,MOSI,SS);
  LoRa.setPins(SS,RST,DI0);  
  if (!LoRa.begin(915E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
  //LoRa.onReceive(cbk);
  LoRa.receive();
  Serial.println("init ok");
  display.init();
  display.flipScreenVertically();  
  display.setFont(ArialMT_Plain_10);
  setup_wifi();
  setup_pubSub();
  delay(1500);
}

void loop() {
  
  
  int packetSize = LoRa.parsePacket();
  if (packetSize) { 
    Serial.println("data received");
    cbk(packetSize);
    read_sender(&temperature,&pressure);    
    if (!pubSubClient.connected()){ 
        fn_MQTTconnect();

      
   }
    pubSubClient.loop();

    bme_values_to_mqtt(temperature, pressure);
    }
    
   
  delay(10);
}
