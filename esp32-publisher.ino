#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <WiFi.h>
DynamicJsonDocument jsonDoc(200);

const char *ssid = "";
const char *password = "";

const char *mqtt_broker = "broker.hivemq.com";
const char *mqtt_username = "esp32-mqtt-pub41";
const char *mqtt_password = "12345678";
const int mqtt_port = 1883;
const int buttonPins[8] = {12, 13, 14, 15, 16, 17, 18, 19};
String topic_pub = "rifki-mqttesp32/datapub";
WiFiClient client;
PubSubClient mqtt(client);
bool conn_state;
String jsonVal;
int counter[8];
String buttonPairs[24];
String buttonFlag[8] = {"a", "b", "c", "d", "e", "f", "g", "h"};
unsigned long previousMillis = 0;
const long interval = 500;
const long debounceDelay = 50;
int lastButtonState[8];
bool buttonState;
unsigned long lastDebounceTime[8];
char jsonString[200];
void connectWifi();
bool checkConnection();
bool compareButtonValues();

void setup()
{
  // put your setup code here, to run once:
  for (int i = 0; i < 8; i++)
  {
    pinMode(buttonPins[i], INPUT_PULLUP);
    counter[i] = 0;
  }
  Serial.begin(9600);
  connectWifi();
  mqtt.setServer(mqtt_broker, mqtt_port);
  while (!mqtt.connected())
  {
    String mqtt_id = "esp32-client-";
    mqtt_id += String(WiFi.macAddress());
    Serial.printf("The client %s connects to the public MQTT broker\n", mqtt_id.c_str());
    if (mqtt.connect(mqtt_id.c_str(), mqtt_username, mqtt_password))
    {
      Serial.println("Broker connected");
    }
    else
    {
      Serial.print("failed with state ");
      Serial.print(mqtt.state());
      delay(2000);
    }
  }
  delay(100);
}

void loop()
{
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval)
  {
    previousMillis = currentMillis;
    if (!checkConnection())
    {
      connectWifi();
      Serial.println("MQTT Connected");
    }

    for (int i = 0; i < 8; i++)
    {
      int buttonState = digitalRead((buttonPins[i]));
      if (buttonState == LOW && lastButtonState[i] == HIGH && (currentMillis - lastDebounceTime[i]) > debounceDelay)
      {
        counter[i]++;
        buttonPairs[i] = buttonFlag[i] + String(counter[i]);
        if (counter[i] == 3)
        {
          counter[i] = 0;
        }
        lastDebounceTime[i] = currentMillis;
      }
      lastButtonState[i] = buttonState;
    }
    if (!compareButtonValues())
    {
      jsonDoc.clear();
      for (int i = 0; i < 8; i++)
      {
        jsonDoc["button" + String(i + 1)] = buttonPairs[i];
        serializeJson(jsonDoc, jsonString);
      }
      mqtt.publish(topic_pub.c_str(), jsonString);
      Serial.print("Data sent: " + String(jsonString));
    }
  }
}

void connectWifi()
{
  Serial.println("Connecting To Wifi");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }

  Serial.println("Wifi Connected");
  Serial.println(WiFi.SSID());
  Serial.println(WiFi.RSSI());
  Serial.println(WiFi.macAddress());
  Serial.println(WiFi.localIP());
  Serial.println(WiFi.gatewayIP());
  Serial.println(WiFi.dnsIP());
}

bool checkConnection()
{
  if (WiFi.status() == WL_CONNECTED)
  {
    conn_state = true;
  }
  else if (WiFi.status() != WL_CONNECTED)
  {
    conn_state = false;
  }
  else if (!mqtt.connected())
  {
    conn_state = false;
  }
  return conn_state;
}

bool compareButtonValues()
{
  for (int i = 0; i < 8; i++)
  {
    if (jsonDoc["button" + String(i + 1)] != buttonPairs[i])
    {
      jsonDoc["button" + String(i + 1)] = buttonPairs[i];
      return false;
    }
  }
  return true;
}
