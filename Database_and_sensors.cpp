/*=============Includes==========*/
#include <Arduino.h>
#include <WiFi.h>
#include "FirebaseESP32.h"
#include <vector>

/*==========Constants=========*/
const char* WIFI_SSID = "Moto G20";
const char* WIFI_PW = "jessi0706";

/*========Variables=======*/
FirebaseData fbdo;
FirebaseAuth fbauth;
FirebaseConfig fbconfig;
 
template<class T>
using Vector = std::vector<T>;
 
const uint8_t PIN_NTC1 = 35;
const uint8_t PIN_NTC2 = 32;
const uint8_t PIN_LDR = 33;

const uint32_t ADC_MAX = (1 << 12) - 1;
const float VREF = 3.3f;
const float R1 = 10000;
 
Vector<float> NTC_10K_temp = { -55, -50, -45, -40, -35,
                               -30, -25, -20, -15, -10,
                               -5, 0, 5, 10, 15,
                               20, 25, 30, 35, 40,
                               45, 50, 55, 60, 65,
                               70, 75, 80, 85, 90,
                               95, 100, 105, 110, 115,
                               120, 125 };
 
Vector<float> NTC_10K_res = { 1214600, 844390, 592430, 419380, 299480,
                              215670, 156410, 114660, 84510, 62927,
                              47077, 35563, 27119, 20860, 16204,
                              12683, 10000, 7942, 6327, 5074,
                              4103, 3336, 2724, 2237, 1846,
                              1530, 1275, 1068, 899.3, 760.7,
                              645.2, 549.4, 470.0, 403.6, 347.4,
                              300.1, 260.1 };


const char* FB_HOST = "https://banco-de-dados-9f2cd-default-rtdb.firebaseio.com/";
const char* FB_KEY = "AV4ZTyBcYK2k6b3qLG4dEnAXXipQcIWTzg7GOfLW";

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

void setup()
{
  pinMode(PIN_NTC1, INPUT);
  pinMode(PIN_NTC2, INPUT);
  pinMode(PIN_LDR, INPUT);

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
  uint16_t input = analogRead(PIN_LDR);
  float voltage = input *  VREF / ADC_MAX;

  uint16_t input_value1 = analogRead(PIN_NTC1);

  uint16_t input_value2 = analogRead(PIN_NTC2);

  float voltage1 = input_value1 * VREF / ADC_MAX;
  
  float resistence1 = R1 * voltage1 /(VREF - voltage1);

  float voltage2 = input_value2 * VREF / ADC_MAX;
  
  float resistence2 = R1 * voltage2 /(VREF - voltage2);

  int index;
  int indice;

  for (int i = 0; i < NTC_10K_res.size(); i ++)
  {
    if (resistence1 > NTC_10K_res[i])
    {
      index = i;
      break;
    }
  }

  for (int i = 0; i < NTC_10K_res.size(); i ++)
  {
    if (resistence2 > NTC_10K_res[i])
    {
      indice = i;
      break;
    }
  }
  float temperature1 = NTC_10K_temp[index] + (resistence1 - NTC_10K_res[index]) * 
    (NTC_10K_temp[index] - NTC_10K_temp[index-1]) / (NTC_10K_res[index] - NTC_10K_res[index - 1]); 

  Serial.printf("\naaaaaaaaaaaaaaaADC: %8i Voltage: %8.2f Resistence: %8.2f Temperature: %8.2f", input_value1, voltage1, resistence1, temperature1); 

  float temperature2 = NTC_10K_temp[indice] + (resistence2 - NTC_10K_res[indice]) * 
    (NTC_10K_temp[indice] - NTC_10K_temp[indice-1]) / (NTC_10K_res[indice] - NTC_10K_res[indice - 1]); 

  Serial.printf("\nADC: %8i Voltage: %8.2f Resistence: %8.2f Temperature: %8.2f", input_value2, voltage2, resistence2, temperature2); 

  bool luminosity_state;
  if (voltage > 2.2)
  {
    luminosity_state = false;
  }
  else
  {
    luminosity_state = true;
  }
  
  FirebaseJson json;

  json.set("/subsys_00/temperature/temp_sensor_00", temperature1);
  json.set("/subsys_00/temperature/temp_sensor_01", temperature2);
  json.set("/subsys_00/luminosity", luminosity_state);


  bool status = Firebase.updateNode(fbdo, "/banco-de-dados-9f2cd/valor", json);

  if (!status)
  {
    Serial.printf("\n", fbdo.errorReason().c_str());
  }
  delay(15000);
}
