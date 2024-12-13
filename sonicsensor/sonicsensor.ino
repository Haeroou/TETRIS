#include <SoftwareSerial.h>
SoftwareSerial s(3,2);  //3:RX 2:TX

int trigPin = 13;
int echoPin = 12;
void setup() {
  Serial.begin(9600);
  s.begin(9600);
  pinMode(echoPin, INPUT);
  pinMode(trigPin, OUTPUT);
}

void loop() {
  long duration, distance, temp_distance;
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  temp_distance = ((float)(340 * duration) / 10000) / 2;

  if (temp_distance <= 35) {
    distance = temp_distance;
  }

  if (distance <= 10) {
    s.write('0');
  }
  else if (distance <= 22) {
    s.write('1');
  }
  else {
    s.write('2');
  }

  delay(100);
}
