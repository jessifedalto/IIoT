#include <Arduino.h>
#include <WiFi.h>

void setup()
{
  Serial.begin(115200);

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();

  WiFi.begin("ssid","senha");
}

void loop()
{
  int qtd_wifi = WiFi.scanNetworks();

  if(qtd_wifi == 0)
  {
    return;
  }

  for (int i = 0; i < qtd_wifi; i++)
  {
    Serial.printf("\nQuantidade de redes disponiveis; %i", qtd_wifi);
    Serial.print("\nIndice: ");
    Serial.print(i + 1);
    Serial.print("\nSSID: ");
    Serial.print(WiFi.SSID(i));
    Serial.print("\nRSSI: ");
    Serial.print(WiFi.RSSI(i));
    Serial.print("\nAuth: ");
    Serial.print(WiFi.encryptionType(i));
    Serial.println(); /* == Serial.print("\n")*/
  }
  
  delay(5000);
}
