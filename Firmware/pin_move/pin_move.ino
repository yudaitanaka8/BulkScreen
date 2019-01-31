int latchPin = 8;
int clockPin = 12;
int dataPin = 11;

int STBY = 10;

int PWM = A3;

byte writeBuffer[64];
byte dataModes[64];
byte readBuffer[64];


//モーターを動かす
void move_motor(byte motor) {
  digitalWrite(STBY, HIGH);
  digitalWrite(latchPin, LOW);
  digitalWrite(clockPin, LOW);
  shiftOut(dataPin, clockPin, LSBFIRST, motor);
  digitalWrite(latchPin, HIGH);
  analogWrite(PWM, 255);
}

void move_motor2(long motor) {
  digitalWrite(STBY, HIGH);
  digitalWrite(latchPin, LOW);
  digitalWrite(clockPin, LOW);
  shiftOut(dataPin, clockPin, LSBFIRST, motor);
  digitalWrite(latchPin, HIGH);
  analogWrite(PWM, 255);
}

void setup() {
  Serial.begin(9600);

  //バッファ配列の初期化
  for (int i = 0; i < 64; i++) {
    if (i % 2 == 0) {
      writeBuffer[i] = 0B01010101;
    } else {
      writeBuffer[i] = 0B00000110;
    }
    dataModes[i] = B11111111;
    readBuffer[i] = B11111111;
  }

  //latch, clock, data
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);

  pinMode(STBY, OUTPUT);

  analogWrite(PWM, 255);
}

void loop() {
  
  /*
  for (int i = 0; i < 64; i++) {
    //shiftOut(dataPin, clockPin, MSBFIRST, writeBuffer[i]);
    shiftOut(dataPin, clockPin, LSBFIRST, writeBuffer[i]);
    Serial.println(1 << i);
    Serial.println(writeBuffer[i]);
  }
  */

  move_motor(B10000000);
  delay(1000);
  digitalWrite(STBY, LOW);
  delay(1000);
  move_motor(B00100000);
  delay(1000);
  digitalWrite(STBY, LOW);
  delay(1000);
  move_motor(B00001000);
  delay(1000);
  digitalWrite(STBY, LOW);
  delay(1000);
  move_motor(B00000010);
  delay(1000);
  digitalWrite(STBY, LOW);
  delay(1000);
  move_motor(B01000000);
  delay(1000);
  digitalWrite(STBY, LOW);
  delay(1000);
}
