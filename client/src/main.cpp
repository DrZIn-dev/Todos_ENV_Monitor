/**
 * Todo : Wifi Connect
 * Todo : MQTT Connect 
 * Todo : MQTT Reconnect
 * Todo : JSON Process Funciton
 * Todo : Pub JSON Functuion
*/

/**
 * * JSON Schema [ Todo ]
 * {
 *    "title"      : "string"
 *    "label"      : "string"
 *    "date"       : "string"
 *    "status"     : "string"
 *    "storage_id" : "string"
 * }
*/

/**
 * *JSON Schema [ ENV ]
 * {
 *    "temp"      : "int"
 *    "hump"      : "int"
 *    "pressure"  : "int"
 * }
*/

#include <Arduino.h>
#include <M5Stack.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <WiFi.h>
#include <env.h>

//* Prototype
void wifiSetup();

//* Server Details
#define MQTT_SERVER
#define MQTT_PORT
#define MQTT_USERNAME
#define MQTT_PASSWORD
#define MQTT_NAME

//* MQTT Topic
#define MQTT_SUB_TEST "/NODE_RED/Test" //? Test MQTT Only
#define MQTT_SUB_JSON "/NODE_RED/JSON" //? JSON Format
#define MQTT_PUB_JSON "/ESP32_1/JSON_PUB"

//* Wifi Details
#define WIFI_STA_NAME MY_WIFI_USER
#define WIFI_STA_PASSWORD MY_WIFI_PASSWORD

//! Storage Params
const char **title;
const char **label;
const char **date;
const char **status;
const char **storage_id;

int **temp;
int **hump;
int **pressure;

char buffer[200];

//* Basic Params
unsigned long previousMillis_WiFi = 0, previousMillis_MQTT = 0;
#define Intervals 500
//* Object && Create Connection

//* Addition Function
void wifiSetup()
{
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WIFI_STA_NAME);

  //* WiFi Connection
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_STA_NAME, WIFI_STA_PASSWORD);

  Serial.print("Status: [");
  while (WiFi.status() != WL_CONNECTED)
  {
    //* Time Loop Uss Millis
    unsigned long currentMillis = millis(); //? Store Current time
    if (currentMillis - previousMillis_WiFi > Intervals)
    {
      previousMillis_WiFi = currentMillis; //? Save Current time
      Serial.print("=");
    }
  }
  Serial.println("] 100%");

  Serial.println("--- WiFi Connected ---");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println();
  wifiSetup();
}

void loop()
{
  // put your main code here, to run repeatedly:
}