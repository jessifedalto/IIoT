//Contador de binario em c++, adaptável para até qual número você quiser contar
const uint8_t PINOS_LED[] = {25, 26,27, 14};

void setup() 
{
  for(auto pino: PINOS_LED)
  {
    pinMode(pino, OUTPUT);
  }
}

void loop() 
{
  for(uint8_t i = 0; i < 15; i++)
  {
    for (uint8_t j = 0; j < 4; j++)
    {
      digitalWrite(PINOS_LED[j], ((i % (2 << j)) - (i % (1 << j))) > 0);
    }

    delay(1000);

    // digitalWrite(PINOS_LED[1], (i%2)); - Feito para apenas 4 leds
    // digitalWrite(PINOS_LED[2], ((i%4) - (i%2))>0);
    // digitalWrite(PINOS_LED[3], ((i%8) - (i%4))>0);
    // digitalWrite(PINOS_LED[0], ((i%16) - (i%8))>0);
  }
}
