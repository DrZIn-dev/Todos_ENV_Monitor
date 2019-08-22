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
void callback(char *topic, byte *payload, unsigned int length);
void countTopic(String payload);
void mqttReconnect();
void mqttSetup();
void mqttTestPub();
void printStorage();
void pubJSON();
void pubRequest();
void todoTopic(byte *payload, unsigned int length);
void wifiSetup();

//* Server Details
#define MQTT_SERVER MY_MQTT_SERVER
#define MQTT_PORT MY_MQTT_PORT
#define MQTT_USERNAME MY_MQTT_USERNAME
#define MQTT_PASSWORD MY_MQTT_PASSWORD
#define MQTT_NAME MY_MQTT_NAME

//* MQTT Topic
#define MQTT_SUB_TEST "/NODE_RED/Test"   //? Test MQTT Only
#define MQTT_SUB_TODOS "/NODE_RED/Todos" //? JSON Format
#define MQTT_PUB_START "/ESP32_1/Start"  //? Tell isStart
#define MQTT_SUB_COUNT "/NODE_RED/Count" //? Sub Count From MQTT
//* Wifi Details
#define WIFI_STA_NAME MY_WIFI_USER
#define WIFI_STA_PASSWORD MY_WIFI_PASSWORD

//! Storage Params
String str_title[100];
String str_label[100];
String str_date[100];
String str_status_[100];
String str_store_id[100];

//* Basic Params
unsigned long previousMillis_WiFi = 0, previousMillis_MQTT = 0, previousMillis_PUB = 0;
#define Intervals 500
int count = 0;
int cur_count = 0;
bool isFetch = false;

//* Object && Create Connection
WiFiClient client;         //? Create TCP Connection
PubSubClient mqtt(client); //? Create Mqtt over WiFiClient

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

void mqttSetup()
{
  mqtt.setServer(MQTT_SERVER, MQTT_PORT); //? Set Destination Server
  mqtt.setCallback(callback);             //? Set Callback Function
  Serial.println("--- MQTT Connected ---");
}

void callback(char *topic, byte *payload, unsigned int length)
{
  payload[length] = '\0';
  String topic_str = topic, payload_str = (char *)payload;
  Serial.println("[" + topic_str + "]: " + payload_str);
  if (topic_str == MQTT_SUB_COUNT)
  {
    countTopic(payload_str);
  }
  if (topic_str == MQTT_SUB_TODOS)
  {
    todoTopic(payload, length);
  }
}

void mqttReconnect()
{
  Serial.println("MQTT Connecting ");
  Serial.print("Status: [");
  while (mqtt.connected() == false)
  {
    mqtt.connect(MQTT_NAME, MQTT_USERNAME, MQTT_PASSWORD); //? Reconnect To Server

    //* Time Loop
    unsigned long currentMillis = millis(); //? Store Current time
    if (currentMillis - previousMillis_MQTT > Intervals)
    {
      previousMillis_MQTT = currentMillis; //? Save Current time
      Serial.print("=");
    }
  }
  Serial.println("] 100%");
  Serial.println("--- MQTT Reconnected ---");

  if (mqtt.connect(MQTT_NAME, MQTT_USERNAME, MQTT_PASSWORD))
  {
    Serial.println("connected : " + String(MQTT_SUB_TEST));
    mqtt.subscribe(MQTT_SUB_TEST);
    Serial.println("connected : " + String(MQTT_SUB_TODOS));
    mqtt.subscribe(MQTT_SUB_TODOS);
    Serial.println("connected : " + String(MQTT_SUB_COUNT));
    mqtt.subscribe(MQTT_SUB_COUNT);
  }
  else
  {
    Serial.println("--- failed ---");
  }
}

void mqttTestPub()
{
  unsigned long currentMillis = millis(); //? Store Current time
  if (currentMillis - previousMillis_PUB > Intervals)
  {
    previousMillis_PUB = currentMillis; //? Save Current time

    mqtt.publish(MQTT_SUB_TEST, "Hello From M5GO");
  }
}

void pubJSON()
{
  unsigned long currentMillis = millis(); //? Store Current time
  if (currentMillis - previousMillis_PUB > 500)
  {
    previousMillis_PUB = currentMillis; //? Save Current time
    const size_t capacity = JSON_OBJECT_SIZE(3);
    DynamicJsonDocument doc(capacity);
    char buffer[512];
    // Todo : Change To Pub ENV
    doc["title"] = "test1";
    doc["duration"] = 10;
    doc["status"] = "Todo";

    serializeJson(doc, buffer);

    mqtt.publish("", buffer);
  }
}

void pubRequest()
{
  mqtt.publish(MQTT_PUB_START, "1");
}

void countTopic(String payload)
{
  if (payload.toInt() != count)
  {
    count = payload.toInt();
    cur_count = 0;
    isFetch = false;
    memset(str_title, 0, sizeof(str_title));
    memset(str_label, 0, sizeof(str_label));
    memset(str_date, 0, sizeof(str_date));
    memset(str_status_, 0, sizeof(str_status_));
    memset(str_store_id, 0, sizeof(str_store_id));
    printStorage();
    Serial.println("new count : " + String(count));
  }
}

void todoTopic(byte *payload, unsigned int length)
{
  payload[length] = '\0';
  Serial.println("--- Start JSON Process ---");
  const size_t capacity = JSON_OBJECT_SIZE(5); //? Object Size
  DynamicJsonDocument doc(capacity);           //? Declare variable for store json

  deserializeJson(doc, payload, length); //? deserialize JSON

  const char *title = doc["title"];       // "Test"
  const char *label = doc["label"];       // "Test"
  const char *date = doc["date"];         // "2019-08-21"
  const char *status_ = doc["status_"];   // "Todo"
  const char *store_id = doc["store_id"]; // "e70a70"

  //* Store in Storage
  str_title[cur_count] = title;
  str_label[cur_count] = label;
  str_date[cur_count] = date;
  str_status_[cur_count] = status_;
  str_store_id[cur_count] = store_id;

  cur_count++;
  isFetch = (cur_count + 1) == count ? !isFetch : isFetch;
  /*Serial.println(" title : " + String(title) +
                 " label : " + String(label) +
                 " date : " + String(date) +
                 " status_ : " + String(status_) +
                 " store_id : " + String(store_id) + String(isFetch));*/

  if (isFetch)
  {
    printStorage();
  }
}

void printStorage()
{
  int i = 0;
  Serial.println("---- Print In Storage ----");
  while (str_title[i] != '\0')
  {
    Serial.println(" title : " + str_title[i] +
                   " label : " + str_label[i] +
                   " date : " + str_date[i] +
                   " status_ : " + str_status_[i] +
                   " store_id : " + str_store_id[i]);

    i++;
  }
}
void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println();
  wifiSetup();
  mqttSetup();
}

void loop()
{
  // put your main code here, to run repeatedly:
  if (mqtt.connected() == false)
  {
    mqttReconnect();
    pubRequest();
  }
  else
  {
    mqtt.loop();
  }
}