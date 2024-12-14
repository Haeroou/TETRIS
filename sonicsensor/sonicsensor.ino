#include <SoftwareSerial.h>
SoftwareSerial s(3,2);  //3:RX 2:TX

int rotateTrigPin = 13;
int rotateEchoPin = 12;

int downTrigPin = 11;
int downEchoPin = 10;

int RightMotor_E_pin = 4;                                        // 오른쪽 모터의 Enable & PWM
int LeftMotor_E_pin = 5;                                         // 왼쪽 모터의 Enable & PWM
int RightMotor_1_pin = 6;                                        // 오른쪽 모터 제어선 IN1
int RightMotor_2_pin = 7;                                        // 오른쪽 모터 제어선 IN2
int LeftMotor_3_pin = 8;                                        // 왼쪽 모터 제어선 IN3
int LeftMotor_4_pin = 9;                                        // 왼쪽 모터 제어선 IN4

int L_MAX_MotorSpeed = 80;                                          // 왼쪽 모터 속도
int R_MAX_MotorSpeed = 80;                                          // 오른쪽 모터 속도

int L_MotorSpeed = 0;                                          // 왼쪽 모터 속도
int R_MotorSpeed = 0;                                          // 오른쪽 모터 속도

void setup() {
  Serial.begin(9600);
  s.begin(9600);
  pinMode(rotateEchoPin, INPUT);
  pinMode(rotateTrigPin, OUTPUT);
  pinMode(downEchoPin, INPUT);
  pinMode(downTrigPin, OUTPUT);

  pinMode(RightMotor_E_pin, OUTPUT);
  pinMode(LeftMotor_E_pin, OUTPUT);
  pinMode(RightMotor_1_pin, OUTPUT);
  pinMode(RightMotor_2_pin, OUTPUT);
  pinMode(LeftMotor_3_pin, OUTPUT);
  pinMode(LeftMotor_4_pin, OUTPUT);
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

  if (rotateDistance > 10 && downDistance > 10) {
    s.write('1');
    Serial.println("1");
  }

  if (s.available()) {
    char motor_data = s.read();
    motor_pattern = int(motor_data - '0');
    if (motor_pattern == 3 && rotateDistance <= 10) {
      L_MotorSpeed = L_MAX_MotorSpeed;
      R_MotorSpeed = R_MAX_MotorSpeed;
      motor_role(HIGH, HIGH);
    }
    else if (motor_pattern == 4 && downDistance <= 10) {
      L_MotorSpeed = L_MAX_MotorSpeed;
      R_MotorSpeed = R_MAX_MotorSpeed;
      motor_role(LOW, LOW);
    }
    else if (motor_pattern == 5 && rotateDistance > 10 && downDistance > 10) {
      analogWrite(RightMotor_E_pin, 0);
      analogWrite(LeftMotor_E_pin, 0);
    }
  }
  delay(100);
}

void motor_role(int R_motor, int L_motor) {
  digitalWrite(RightMotor_1_pin, R_motor);
  digitalWrite(RightMotor_2_pin, !R_motor);
  digitalWrite(LeftMotor_3_pin, L_motor);
  digitalWrite(LeftMotor_4_pin, !L_motor);

  analogWrite(RightMotor_E_pin, R_MotorSpeed);  // 우측 모터 속도값
  analogWrite(LeftMotor_E_pin, L_MotorSpeed);   // 좌측 모터 속도값
}
