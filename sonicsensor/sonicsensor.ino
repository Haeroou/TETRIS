#include <SoftwareSerial.h>
SoftwareSerial s(3,2);  //3:RX 2:TX

int rotateTrigPin = 13;
int rotateEchoPin = 12;

int downTrigPin = 11;
int downEchoPin = 10;

void setup() {
  Serial.begin(9600);
  s.begin(9600);
  pinMode(rotateEchoPin, INPUT);
  pinMode(rotateTrigPin, OUTPUT);
  pinMode(downEchoPin, INPUT);
  pinMode(downTrigPin, OUTPUT);
}

void loop() {
  long rotateDuration, rotateDistance, rotateTempDistance;
  long downDuration, downDistance, downTempDistance;

  digitalWrite(rotateTrigPin, HIGH);

  digitalWrite(downTrigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(rotateTrigPin, LOW);
  rotateDuration = pulseIn(rotateEchoPin, HIGH);
  rotateTempDistance = ((float)(340 * rotateDuration) / 10000) / 2;

  digitalWrite(downTrigPin, LOW);
  downDuration = pulseIn(downEchoPin, HIGH);
  downTempDistance = ((float)(340 * downDuration) / 10000) / 2;

  if (rotateTempDistance <= 35) {
    rotateDistance = rotateTempDistance;
  }

  if (downTempDistance <= 35) {
    downDistance = downTempDistance;
  }

  if (rotateDistance <= 10) {
    s.write('0');
    Serial.println("0");
  }

  if (downDistance <= 10) {
    s.write('2');
    Serial.println("2");
  }
  delay(100);
}
