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

//* Server Details
#define MQTT_SERVER
#define MQTT_PORT
#define MQTT_USERNAME
#define MQTT_PASSWORD
#define MQTT_NAME

//* Wifi Details
#define WIFI_STA_NAME
#define WIFI_STA_PASSWORD

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

//* Object && Create Connection

//* Addition Function

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200);
}

void loop()
{
  // put your main code here, to run repeatedly:
}