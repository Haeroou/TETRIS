int row[8] = {
  B00000001,
  B00000010,
  B00000100,
  B00001000,
  B00010000,
  B00100000,
  B01000000,
  B10000000
};

int row7[8] = {
  B10000001,
  B01000010,
  B00100100,
  B00011000,
  B00011000,
  B00100100,
  B01000010,
  B10000001
};
int latchpin = 5;
int clockpin = 4;
int datapin = 6;
void setup() {
  // put your setup code here, to run once:
  pinMode(latchpin, OUTPUT);
  pinMode(clockpin, OUTPUT);
  pinMode(datapin, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  for (int i = 0; i < 8; i++) {
    digitalWrite(latchpin, LOW);
    shiftOut(datapin, clockpin, LSBFIRST, row[i]);
    shiftOut(datapin, clockpin, LSBFIRST, 0xff);
    shiftOut(datapin, clockpin, LSBFIRST, row[i]);
    shiftOut(datapin, clockpin, LSBFIRST, 0x00);
    shiftOut(datapin, clockpin, LSBFIRST, 0x00);
    digitalWrite(latchpin, HIGH);
    delay(1);
  }}
