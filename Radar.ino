#include <ESP32Servo.h>

Servo radar;

const int SERVO_PIN = 13;

void setup() {
  Serial.begin(115200);

  radar.setPeriodHertz(50);
  radar.attach(SERVO_PIN, 500, 2400);
}

void loop() {
  // Sweep from 0° to 180°
  for (int angle = 0; angle <= 180; angle++) {
    radar.write(angle);
    Serial.print("Angle: ");
    Serial.println(angle);
    delay(10);
  }

  // Sweep from 180° back to 0°
  for (int angle = 180; angle >= 0; angle--) {
    radar.write(angle);
    Serial.print("Angle: ");
    Serial.println(angle);
    delay(10);
  }
}