#include <Arduino.h>
#include <WiFi.h>
#include "DHT.h"
#include <FirebaseESP32.h>
#include "PubSubClient.h"
#include <Adafruit_Sensor.h>

/*==========Constants=========*/
const char* WIFI_SSID = "Vivo-Internet-BF17";
const char* WIFI_PW = "78814222";

const char* MQTT_BROKER = "test.mosquitto.org";
const uint16_t MQTT_PORT = 1883;
const char* MQTT_USERNAME = "";
const char* MQTT_PASSWORD = "";

/*========Variables=======*/
WiFiClient wifi_client;
PubSubClient mqtt_client(wifi_client);

FirebaseData fbdo;
FirebaseAuth fbauth;
FirebaseConfig fbconfig;
 
const uint8_t PIN_DHT = 33;
const uint8_t PIN_LED = 32;
long tempo = 0;
long tempo_led = 0;

#define DHTTYPE DHT11
 
DHT dht(PIN_DHT, DHTTYPE);

const char* FB_HOST = "https://iiot-dta-default-rtdb.firebaseio.com";
const char* FB_KEY = "Ag5gJMhAnTWQgDVhegkDRF1uTjJfpMUDkXB8WBEa";

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
  String message = "";
  Serial.printf("Mensagem recebida no tópico %s: ", topic);

  for (int i = 0; i < length; i++)
  {
    Serial.print((char)payload[i]);
    message+= (char)payload[i];
  }

  Serial.println();

  if ((strcmp(topic, "iiot-dta/check")) == 0)
  {
    if (message == "100")
    {
      mqtt_client.publish("iiot-dta/check", "testeee jessi");
    }
    if (message == "200")
    {
      digitalWrite(PIN_LED, HIGH);
      tempo_led = millis();
    }
  }
  if ((strcmp(topic, "iiot-dta/request")) == 0)
  {
    if (message == "100")
    {
      float temperature = dht.readTemperature();
      float humidity = dht.readHumidity();

      FirebaseJson json;

      json.set("/temperature/", temperature);
      json.set("/humidity/", humidity);

      bool status = Firebase.updateNode(fbdo, "/challenge02/subsys_03/", json);

      if (!status)
      {
        Serial.printf("\n", fbdo.errorReason().c_str());
      }  
    }
  }
}

void setup()
{
  dht.begin();
  pinMode(PIN_DHT, INPUT);
  pinMode(PIN_LED, OUTPUT);

  Serial.begin(115200);
  connectWiFi(WIFI_SSID, WIFI_PW);

  fbconfig.database_url = FB_HOST;
  fbconfig.signer.tokens.legacy_token = FB_KEY;
  fbdo.setBSSLBufferSize(4096, 1024);
  Firebase.reconnectWiFi(true);
  Firebase.begin(&fbconfig, &fbauth);

  mqtt_client.setServer(MQTT_BROKER, MQTT_PORT);
  mqtt_client.setCallback(callback);
 
  while (!mqtt_client.connected())
  {
    String client_id = "dta-Jessi";
    client_id += String(WiFi.macAddress());

    if(mqtt_client.connect(client_id.c_str(), MQTT_USERNAME, MQTT_PASSWORD))
    {
      Serial.println("Conexao MQTT bem sucedida");
    }
  }
  mqtt_client.subscribe("iiot-dta/request");
  mqtt_client.subscribe("iiot-dta/check");
}

void loop()
{
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  if ((millis()-tempo_led)>5000)
  {
    digitalWrite(PIN_LED, LOW);
    tempo_led = millis();
  }

  if ((millis()-tempo)>30000)
  {
    FirebaseJson json;

    json.set("/temperature/", temperature);
    json.set("/humidity/", humidity);

    if (temperature > 30)
    {
      mqtt_client.publish("iiot-dta/request", "10");
    }
    
    bool status = Firebase.updateNode(fbdo, "/challenge02/subsys_03/", json);

    Serial.printf("Temperature: %4.2f \t Humidity: %4.2f \n", temperature, humidity);

    if (!status)
    {
      Serial.printf("\n", fbdo.errorReason().c_str());
    }  

    tempo = millis();
  }

  mqtt_client.loop();

}
