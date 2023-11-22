const uint8_t PIN_INPUT = 26;

const uint16_t ADC_MAX = 4095;
const float VCC = 3.3f;

void setup()
{
  pinMode(PIN_INPUT, INPUT);
  Serial.begin(115200);
}

void loop() 
{
  uint16_t input = analogRead(PIN_INPUT);
  float voltage = input *  VCC / ADC_MAX;

  Serial.println(voltage);
}
