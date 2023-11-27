/*=============Includes==========*/
#include <Arduino.h>
#include <WiFi.h>
#include "FirebaseESP32.h"

/*==========Constants=========*/
const char* WIFI_SSID = "Moto G20";
const char* WIFI_PW = "jessi0706";

/*========Variables=======*/
FirebaseData fbdo;
FirebaseAuth fbauth;
FirebaseConfig fbconfig;

const char* FB_HOST = "https://iiot-91106-default-rtdb.firebaseio.com/";
const char* FB_KEY = "vBcgOflnzNal2KzAm2dZhWPUGGcnFmnw5AJotqN7";

bool connectWiFi(const char* ssid, const char* pw)
{
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  int qtd_wifi = WiFi.scanNetworks();
  if(qtd_wifi == 0)
  {
    return;
  }
  WiFi.begin(ssid, pw);
  Serial.print("\nConectando!");
  int tentativas = 0;
  while (WiFi.status() != WL_CONNECTED)
  {
    tentativas++;
    if (tentativas > 300)
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

void setup()
{
  Serial.begin(115200);
  connectWiFi(WIFI_SSID, WIFI_PW);

  fbconfig.database_url = FB_HOST;
  fbconfig.signer.tokens.legacy_token = FB_KEY;
  fbdo.setBSSLBufferSize(4096, 1024);
  Firebase.reconnectWiFi(true);
  Firebase.begin(&fbconfig, &fbauth);
}

void loop()
{
  /*Enviando um unico valor para o banco de dados e analisa se deu certo ou nao*/
  float value = 3.14;

  bool status = Firebase.setFloat(fbdo, "/iiot-91106/valor", value);

  if (!status)
  {
    Serial.println(fbdo.errorReason().c_str());
  }
  delay(5000);
}
