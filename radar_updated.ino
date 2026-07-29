#include <ESP32Servo.h>

Servo radar;

const int SERVO_PIN = 13;

const int TRIG_PIN = 12;
const int ECHO_PIN = 27;

void setup() {
  Serial.begin(115200);

  radar.setPeriodHertz(50);
  radar.attach(SERVO_PIN, 500, 2400);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  digitalWrite(TRIG_PIN, LOW);
}

float getDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);

  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH, 17500);

  if (duration == 0) {
    return -1;
  }

  return duration * 0.0343 / 2.0;
}

void loop() {
  // Sweep from 0° to 180°
  for (int angle = 0; angle <= 180; angle++) {
    radar.write(angle);

    Serial.print("Angle: ");
    Serial.print(angle);
    Serial.print(", Distance: ");

    float distance = getDistance();

    if (distance < 0) {
      Serial.println("Out of Range");
    } else {
      Serial.print(distance, 1);
      Serial.println(" cm");
    }

    delay(10);
  }

  // Sweep from 180° back to 0°
  for (int angle = 180; angle >= 0; angle--) {
    radar.write(angle);

    Serial.print("Angle: ");
    Serial.print(angle);
    Serial.print(", Distance: ");

    float distance = getDistance();

    if (distance < 0 || distance > 300) {
      Serial.println("Out of Range");
    } else {
      Serial.print(distance, 1);
      Serial.println(" cm");
    }

    delay(10);
  }
}