#include <PubSubClient.h>
#include <WiFi.h>
#include <ArduinoJson.h>
const char *ssid = "";
const char *password = "";
const char *mqtt_server = "broker.hivemq.com";
const int mqtt_port = 1883;
const char *mqtt_username = "esp32-mqtt-pub42";
const char *mqtt_password = "12345678";
const char *topic_sub = "rifki-mqttesp32/datapub";

const int ledPin[8] = {12, 13, 14, 15, 16, 17, 18, 19};
WiFiClient espClient;
PubSubClient mqtt(espClient);
DynamicJsonDocument jsonDoc(200);
void led_behavior(String button);
void callback(char *topic, byte *msg, unsigned int msgLength);
void setup()
{
  Serial.begin(9600);
  for (int i = 0; i < 8; i++)
  {
    pinMode(ledPin[i], OUTPUT);
  }

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected");

  mqtt.setServer(mqtt_server, mqtt_port);
  mqtt.setCallback(callback);
  delay(100);
}

void callback(char *topic, byte *msg, unsigned int msgLength)
{
  char payload[200];
  for (int i = 0; i < msgLength; i++)
  {
    payload[i] = (char)msg[i];
  }
  payload[msgLength] = '\0';

  deserializeJson(jsonDoc, payload);

  if (DeserializationError error = deserializeJson(jsonDoc, payload))
  {
    Serial.print("deserializeJson gagal: ");
    Serial.println(error.c_str());
    return;
  }

  const char *button[9];
  for (int j = 1; j < 9; j++)
  {
    button[j] = jsonDoc["button" + String(j)].as<const char *>();
    led_behavior(String(button[j]));
  }
}

void led_behavior(String button)
{
  if (button == "a1")
  {
    ledcWrite(ledPin[0], 127.5);
  }
  else if (button == "a2")
  {
    ledcWrite(ledPin[0], 255);
  }
  else if (button == "a3")
  {
    ledcWrite(ledPin[0], 0);
  }
  else if (button == "b1")
  {
    ledcWrite(ledPin[1], 127.5);
  }
  else if (button == "b2")
  {
    ledcWrite(ledPin[1], 255);
  }
  else if (button == "b3")
  {
    ledcWrite(ledPin[1], 0);
  }
  else if (button == "c1")
  {
    ledcWrite(ledPin[2], 127.5);
  }
  else if (button == "c2")
  {
    ledcWrite(ledPin[2], 255);
  }
  else if (button == "c3")
  {
    ledcWrite(ledPin[2], 0);
  }
  else if (button == "d1")
  {
    ledcWrite(ledPin[3], 127.5);
  }
  else if (button == "d2")
  {
    ledcWrite(ledPin[3], 255);
  }
  else if (button == "d3")
  {
    ledcWrite(ledPin[3], 0);
  }
  else if (button == "e1")
  {
    ledcWrite(ledPin[4], 127.5);
  }
  else if (button == "e2")
  {
    ledcWrite(ledPin[4], 255);
  }
  else if (button == "e3")
  {
    ledcWrite(ledPin[4], 0);
  }
  else if (button == "f1")
  {
    ledcWrite(ledPin[5], 127.5);
  }
  else if (button == "f2")
  {
    ledcWrite(ledPin[5], 255);
  }
  else if (button == "f3")
  {
    ledcWrite(ledPin[5], 0);
  }
  else if (button == "g1")
  {
    ledcWrite(ledPin[6], 127.5);
  }
  else if (button == "g2")
  {
    ledcWrite(ledPin[6], 255);
  }
  else if (button == "g3")
  {
    ledcWrite(ledPin[6], 0);
  }
  else if (button == "h1")
  {
    ledcWrite(ledPin[7], 127.5);
  }
  else if (button == "h2")
  {
    ledcWrite(ledPin[7],255);
  }
  else if (button == "h3")
  {
    ledcWrite(ledPin[7], 0);
  }
}

void reconnect()
{

  Serial.print("Attempting MQTT connection...");

  if (mqtt.connect("esp32", mqtt_username, mqtt_password))
  {
    Serial.println("connected");

    mqtt.subscribe(topic_sub);
  }
}

void loop()
{
  if (!mqtt.connected())
  {
    reconnect();
  }
  mqtt.loop();
}
