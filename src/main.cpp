#include <Arduino.h>
#include <vector>
#include <cmath>
 
template<class T>
using Vector = std::vector<T>;
 
const uint8_t PIN_NTC = 35;
                              
const uint32_t ADC_MAX = (1 << 12) - 1;
const float VREF = 3.3f;
const float R1 = 10000;
 
void setup()
{
  pinMode(PIN_NTC, INPUT);
  Serial.begin(115200);
}

void loop() 
{
  uint16_t input_value = analogRead(PIN_NTC);
  float voltage = input_value * VREF / ADC_MAX;
  
  float resistence = R1 * voltage /(VREF - voltage);

  float a = 12.9611833;
  float b = -0.0761498101;
  float c = 3.40889111;
  float d = -64.5957953;
  float e = 390.218793;
  float temperature = b*pow((log(resistence - a)), 3) + c*pow((log(resistence-a)), 2) + d * (log(resistence - a)) + e;

  Serial.printf("\nADC: %8i Voltage: %8.2f Resistence: %8.2f Temperature: %8.2f", input_value, voltage, resistence, temperature); 

  delay(1000);
}