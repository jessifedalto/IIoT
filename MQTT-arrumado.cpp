/*=============Includes==========*/
#include <Arduino.h>
#include <WiFi.h>
#include "PubSubClient.h"

/*==========Constants=========*/
const char* WIFI_SSID = "belisarius";
const char* WIFI_PW = "senhaaaa";
 
const char* MQTT_BROKER = "test.mosquitto.org";
const uint16_t MQTT_PORT = 1883;
const char* MQTT_USERNAME = "";
const char* MQTT_PASSWORD = "";

/*=======Variables=========*/
WiFiClient wifi_client;
PubSubClient mqtt_client(wifi_client);

bool connectWiFi(const char* ssid, const char* pw)
{
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  WiFi.begin(ssid, pw);
  Serial.print("\nConectando!");
  int tentativa = 0;
  while (WiFi.status() != WL_CONNECTED)
  {
    tentativa++;
    if (tentativa > 300)
    {
      return false;
    }
    
    Serial.print(".");
    delay(200);
  }
  Serial.print("\nConectado com o IP: ");
  Serial.println(WiFi.localIP());
  return true;
}

void callback(char* topic, byte* payload, unsigned int length)
{
  Serial.printf("Mensagem recebida no tópico %s: ", topic);

  for (int i = 0; i < length; i++)
  {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}


void setup()
{
  Serial.begin(115200);
  connectWiFi(WIFI_SSID, WIFI_PW);

  mqtt_client.setServer(MQTT_BROKER, MQTT_PORT);
  mqtt_client.setCallback(callback);
 

  while (!mqtt_client.connected())
  {
    String client_id = "mqttx_32fef669";
    client_id += String(WiFi.macAddress());

    if(mqtt_client.connect(client_id.c_str(), MQTT_USERNAME, MQTT_PASSWORD))
    {
      Serial.println("Conexao MQTT bem sucedida");
    }
  }
  mqtt_client.subscribe("testdta");
}

void loop()
{
  mqtt_client.publish("testdta", "testeee 987");

  //Receber

  mqtt_client.loop();

  delay(5000);
}
