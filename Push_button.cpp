const uint8_t PUSH_BUTTON = 25;
const uint8_t LED = 26;

void setup()
{
  pinMode(PUSH_BUTTON, INPUT);
  pinMode(LED, OUTPUT);
}

void loop()
{
  bool pressionado = digitalRead(PUSH_BUTTON);
  digitalWrite(LED, pressionado);
}
