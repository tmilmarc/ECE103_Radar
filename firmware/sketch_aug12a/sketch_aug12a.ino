#include <ESP32Servo.h>

Servo radar;
Servo laserServo;

const int SERVO_PIN = 13;

const int TRIG_PIN = 12;
const int ECHO_PIN = 27;

const int LASER_PIN = 32;
const int LASER_SERVO_PIN = 33;

const int MODE_LED_PIN = 4;

const int MODE_BUTTON_PIN = 21;
const int X_PIN = A0;
const int SW_PIN = 16;
const int MAX_ANALOG = 4095;

const float MAX_TARGET_DISTANCE = 150.0;

int targetAngle = -1;
float targetDistance = 151.0;

bool manualMode = false;

bool lastModeButtonState = HIGH;
bool lastJoystickButtonState = HIGH;

void setup() {
  Serial.begin(115200);
  Serial1.begin(9600, SERIAL_8N1, -1, 17); //sets up baud for DFPlayer

  radar.setPeriodHertz(50);
  radar.attach(SERVO_PIN, 500, 2400);

  laserServo.setPeriodHertz(50);
  laserServo.attach(LASER_SERVO_PIN, 500, 2400);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(LASER_PIN, OUTPUT);

  digitalWrite(TRIG_PIN, LOW);

  digitalWrite(LASER_PIN, LOW); //turns laser off
  laserServo.write(90); //centers laser
  delay(1000);

  DFPLAYER_COMMAND(0x06, 30);  // Set volume to 30
  radar.write(90);

  pinMode(MODE_LED_PIN, OUTPUT);
  digitalWrite(MODE_LED_PIN, LOW);

  pinMode(SW_PIN, INPUT_PULLUP);
  pinMode(MODE_BUTTON_PIN, INPUT_PULLUP);
}

//package to command dfplayer
void DFPLAYER_COMMAND(unsigned char command, unsigned short parameter) {
    unsigned char packet[10];
    packet[0] = 0x7E;
    packet[1] = 0xFF;
    packet[2] = 0x06;
    packet[3] = command;
    packet[4] = 0x00;
    packet[5] = parameter >> 8;
    packet[6] = parameter & 0xFF;

    short checksum = -(packet[1] + packet[2] + packet[3] + packet[4] + packet[5] + packet[6]);

    packet[7] = checksum >> 8;
    packet[8] = checksum & 0xFF;
    packet[9] = 0xEF;

    for (int i = 0; i < 10; i++) {
        Serial1.write(packet[i]);
    }
}

float getDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);

  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  unsigned long duration = pulseIn(ECHO_PIN, HIGH, 17500);

  if (duration == 0) {
    return -1;
  }

  return duration * 0.0343 / 2.0;
}

float GET_LASER_ANGLE(float radarAngle, float distance)
{
  //
  const float LASER_OFFSET_X = -12.7;  // 5 inches left
  const float LASER_OFFSET_Y = -2.54;  // 1 inch behind

  //converts angle to radians for sin and cos functions
  float radarRadians = radarAngle * PI / 180.0;

  // targets position 
  float targetX = distance * cos(radarRadians);
  float targetY = distance * sin(radarRadians);

  // targets position measured from the laser servo
  float laserToTargetX = targetX - LASER_OFFSET_X;
  float laserToTargetY = targetY - LASER_OFFSET_Y;

  // laser angle calculation
  float laserRadians = atan2(laserToTargetY, laserToTargetX);
  float laserAngle = laserRadians * 180.0 / PI;

  return laserAngle;
}

void MANUAL_CONTROL() {
  int xVal = analogRead(X_PIN);

  int servoAngle = map(xVal, 0, MAX_ANALOG, 0, 180);
  servoAngle = constrain(servoAngle, 0, 180);

  laserServo.write(servoAngle);

  bool joystickButtonState = digitalRead(SW_PIN);

  if (joystickButtonState == LOW && lastJoystickButtonState == HIGH) {
    DFPLAYER_COMMAND(0x03, 2);

    digitalWrite(LASER_PIN, HIGH);
    delay(3000);
    digitalWrite(LASER_PIN, LOW);
  }

  lastJoystickButtonState = joystickButtonState;
}

void loop() {


  //resets target
  targetAngle = -1;
  targetDistance = 151.0;

  // Sweep from 0° to 180°
  for (int angle = 0; angle <= 180; angle++) {
    radar.write(angle);

    if (manualMode) {
      MANUAL_CONTROL();
    }

    bool modeButtonState = digitalRead(MODE_BUTTON_PIN);

    if (modeButtonState == LOW && lastModeButtonState == HIGH) {
      manualMode = !manualMode;

      if (manualMode) {
        digitalWrite(MODE_LED_PIN, HIGH);
      }
      else {
        digitalWrite(MODE_LED_PIN, LOW);
      }

    delay(200);
    }
    lastModeButtonState = modeButtonState;

    delay(15);
    Serial.print(angle);
    Serial.print(",");

    float distance = getDistance();

    if (distance > 0 && distance <= MAX_TARGET_DISTANCE && distance < targetDistance) {
      targetDistance = distance;
      targetAngle = angle;
    }

    if (distance < 0) {
      Serial.println(-1);
    } else {
      Serial.println(distance);
      
    }
    delay(10);
  }

  if (!manualMode && targetAngle >= 0) {
    DFPLAYER_COMMAND(0x03, 1);  // “Target detected”
    delay(800);

    float laserAngle = GET_LASER_ANGLE(targetAngle, targetDistance);

    laserServo.write((int)laserAngle);
    delay(700);

    digitalWrite(LASER_PIN, HIGH);  // laser on
    DFPLAYER_COMMAND(0x03, 2);  // plays laser soudn effect
    delay(100);

    delay(3000);                    // stay on for 3 seconds
    digitalWrite(LASER_PIN, LOW);   // laser off

    
}
else {
  digitalWrite(LASER_PIN, LOW);
}
  // Sweep from 180° back to 0°
  for (int angle = 180; angle >= 0; angle--) {
    radar.write(angle);

    if (manualMode) {
      MANUAL_CONTROL();
    }

    bool modeButtonState = digitalRead(MODE_BUTTON_PIN);

    if (modeButtonState == LOW && lastModeButtonState == HIGH) {
      manualMode = !manualMode;

      if (manualMode) {
        digitalWrite(MODE_LED_PIN, HIGH);
      }
      else {
        digitalWrite(MODE_LED_PIN, LOW);
      }

    delay(200);
    }

lastModeButtonState = modeButtonState;
    
    delay(10);
    float distance = getDistance();
    Serial.print(angle);
    Serial.print(",");

    if (distance < 0) {
      Serial.println(-1);
    } else {
      Serial.println(distance);
      
    }
    delay(10);
  }
}