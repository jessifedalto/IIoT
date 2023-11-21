const uint8_t PIN_INPUT = 26;

/*------Constants-----*/
const uint16_t ADC_MAX = 4095;
const float VCC = 3.3;

void setup() 
{
  pinMode(PIN_INPUT, INPUT);
}

void loop() 
{
  int input = analogRead(PIN_INPUT);
  float tensao = static_cast<float>(input) * VCC / ADC_MAX;  
}
