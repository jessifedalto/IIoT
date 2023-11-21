#include <Arduino.h>

const uint8_t PUSH_BUTTON = 18;
const uint8_t LED = 5;

bool last_button = 0;
bool pressed = 0;
bool led_state = 0;
long time_spend = 0;

void setup() 
{
  pinMode(PUSH_BUTTON, INPUT);
  pinMode(LED, OUTPUT);  
}

void loop() 
{
  last_button = pressed;
  pressed = digitalRead(PUSH_BUTTON);

  if (pressed && !last_button && millis() - time_spend > 50)
  {
    led_state = !led_state;
    digitalWrite(LED, led_state);
    time_spend = millis();
  }
  
}
