// #include "secrets.h"
#include <WiFiClientSecure.h>
// #include <PubSubClient.h>
#include <ArduinoJson.h>
// #include "WiFi.h"
#include <TimeLib.h>

#include <PZEM004Tv30.h>

#define PZEM_RX_PIN 14
#define PZEM_TX_PIN 12

#define AWS_IOT_PUBLISH_TOPIC   "esp32/pub"
#define AWS_IOT_SUBSCRIBE_TOPIC "esp32/sub"

float voltage;
float current;
float power;
float energy;
float frequency;

PZEM004Tv30 pzem(Serial1,PZEM_RX_PIN, PZEM_TX_PIN);

// WiFiClientSecure net = WiFiClientSecure();
// PubSubClient client(net);

// void connectAWS()
// {
//   WiFi.mode(WIFI_STA);
//   WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

//   Serial.println("Connecting to Wi-Fi");

//   while (WiFi.status() != WL_CONNECTED)
//   {
//     delay(500);
//     Serial.print(".");
//   }

//   // Configure WiFiClientSecure to use the AWS IoT device credentials
//   net.setCACert(AWS_CERT_CA);
//   net.setCertificate(AWS_CERT_CRT);
//   net.setPrivateKey(AWS_CERT_PRIVATE);

//   // Connect to the MQTT broker on the AWS endpoint we defined earlier
//   client.setServer(AWS_IOT_ENDPOINT, 8883);

//   // Create a message handler
//   client.setCallback(messageHandler);

//   Serial.println("Connecting to AWS IoT");

//   while (!client.connect(THINGNAME))
//   {
//     Serial.print(".");
//     delay(100);
//   }

//   if (!client.connected())
//   {
//     Serial.println("AWS IoT Timeout!");
//     return;
//   }

//   // Subscribe to a topic
//   client.subscribe(AWS_IOT_SUBSCRIBE_TOPIC);

//   Serial.println("AWS IoT Connected!");
// }

// String getCurrentDateTime()
// {
//   char datetime[20];
//   sprintf(datetime, "%04d-%02d-%02dT%02d:%02d:%02d", year(), month(), day(), hour(), minute(), second());
//   return String(datetime);
// }

// void publishMessage()
// {
//   StaticJsonDocument<200> doc;
//   doc["voltage"] = voltage;
//   doc["current"] = current;
//   doc["power"] = power;
//   doc["energy"] = energy;
//   doc["frequency"] = frequency;
//   char jsonBuffer[512];
//   serializeJson(doc, jsonBuffer);

//   client.publish(AWS_IOT_PUBLISH_TOPIC, jsonBuffer);
// }

void messageHandler(char* topic, byte* payload, unsigned int length)
{
  Serial.print("incoming: ");
  Serial.println(topic);

  StaticJsonDocument<200> doc;
  deserializeJson(doc, payload);
  const char* message = doc["message"];
  Serial.println(message);
}

void setup()
{
  Serial.begin(9600);
  // connectAWS();
}

void loop()
{
    // Check if the data is valid
   // if(isnan(voltage)){
        Serial.println("Error reading voltage");
   // } else if (isnan(current)) {
        Serial.println("Error reading current");
   // } else if (isnan(power)) {
        Serial.println("Error reading power");
   // } else if (isnan(energy)) {
        Serial.println("Error reading energy");
   // } else if (isnan(frequency)) {
        Serial.println("Error reading frequency");
   // } else {
      
    voltage = pzem.voltage();
    current = pzem.current();
    power = pzem.power();
    energy = pzem.energy();
    frequency = pzem.frequency();


    Serial.print(F("Voltage: "));
    Serial.print(voltage);
    Serial.print(F("V \n  Current: "));
    Serial.print(current);
    Serial.print(F("A \n Power: "));
    Serial.print(power);
    Serial.print(F("W \n Energy: "));
    Serial.print(energy);
    Serial.print(F("Wh \n Frequency: "));
    Serial.println(frequency);


    // publishMessage();
    // client.loop();
 // }

  delay(2000);
}