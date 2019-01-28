int latchPin = 8;
int clockPin = 12;
int dataPin = 11;

int STBY = 10;

int PWM = A3;

byte writeBuffer[64];
byte dataModes[64];
byte readBuffer[64];

void setup() {
  Serial.begin(9600);

  //バッファ配列の初期化
  for (int i = 0; i < 64; i++) {
    writeBuffer[i] = 1;
    dataModes[i] = 1;
    readBuffer[i] = 1;
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
  for (int i = 0; i < 7; i++) {
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, LSBFIRST, 1 << i);
    digitalWrite(latchPin, HIGH);
     Serial.println(i);
    delay(1000);
  }
  */

  /*
  for (int i = 0; i < 512; i++) {
    int bytenum = i / 8;
    int offset = i % 8;
    byte b = writeBuffer[bytenum];
    if (offset % 2 == 0) bitWrite(b, offset, LOW);
    else bitWrite(b , offset, HIGH);
    writeBuffer[bytenum] = b;
  }
 */
  digitalWrite(STBY, HIGH);
  digitalWrite(latchPin, LOW);
  digitalWrite(clockPin, LOW);
  for (int i = 0; i < 64; i++) {
    shiftOut(dataPin, clockPin, MSBFIRST, writeBuffer[i]);
    Serial.println(writeBuffer[i]);
    delay(1000);
  }
  digitalWrite(latchPin, HIGH);
  analogWrite(PWM, 255);
}
