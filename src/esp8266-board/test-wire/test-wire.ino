#include <Wire.h>

#define SDA_PIN 4
#define SCL_PIN 5
String DEVICE_NAME = "p01-esp-001";

void setup() {
  Serial.begin(115200); // start serial for debug
  while (!Serial) continue;
  Serial.println("fog-explorer-v001 v0.1");

  Wire.begin(SDA_PIN, SCL_PIN); // new syntax: join i2c bus (address required for slave)
}

void loop() {
  Wire.requestFrom(8, 32); /* request & read data of size 13 from slave */
  while (Wire.available()) {
    char c = Wire.read();
    Serial.print(c);
  }
  Serial.println();
  delay(1000);
}
