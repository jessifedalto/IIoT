#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <Arduino.h>

Adafruit_MPU6050 mpu;

uint8_t time = 0;

void setup(void)
{
  Serial.begin(115200);
  while (!Serial)
    delay(10); // will pause Zero, Leonardo, etc until serial console opens

  Serial.println("Adafruit MPU6050 test!");

  // Try to initialize!
  if (!mpu.begin()) 
  {
    Serial.println("Failed to find MPU6050 chip");
    while (1)
    {
      delay(10);
    }
  }
  Serial.println("MPU6050 Found!");
}

void loop()
{
  time = millis();
  /* Get new sensor events with the readings */
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  float add_rad = 0;

  /*Used to calibrate the accelerometer*/
  for (int i = 0; i < 1000; i++) 
  {
    add_rad += g.giro.z; 
  }
  /*Mean of the acceleration of rotation*/
  float mean_rad = add_rad/1000;
 
  /*Angle of rotation in radians*/
  float rad = mean_rad * (millis() - time);

  Serial.print(", Z: ");
  Serial.printf("%2.20", rad);
  Serial.println(" rad");

}
