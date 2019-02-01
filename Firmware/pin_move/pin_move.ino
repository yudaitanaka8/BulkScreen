int latchPin = 8;
int clockPin = 12;
int dataPin = 11;

int STBY = 10;

int PWM = A3;

byte writeBuffer[64];
byte dataModes[64];
byte readBuffer[64];

//ピンを出す配列
byte pin_move[64];
//ピンを戻す配列
byte pin_return[64];

//テスト用のモーターの挙動
//単位はcm
int test_array[16][16] = {{1, 2, 3, 4, 5, 6, 7, 8, 9, 8, 7, 6, 5, 4, 3, 2},
                          {9, 8, 7, 6, 5, 4, 3, 2, 1, 2, 3, 4, 5, 6, 7, 8},
                          {1, 2, 3, 4, 5, 6, 7, 8, 9, 8, 7, 6, 5, 4, 3, 2},
                          {9, 8, 7, 6, 5, 4, 3, 2, 1, 2, 3, 4, 5, 6, 7, 8},
                          {1, 2, 3, 4, 5, 6, 7, 8, 9, 8, 7, 6, 5, 4, 3, 2},
                          {9, 8, 7, 6, 5, 4, 3, 2, 1, 2, 3, 4, 5, 6, 7, 8},
                          {1, 2, 3, 4, 5, 6, 7, 8, 9, 8, 7, 6, 5, 4, 3, 2},
                          {9, 8, 7, 6, 5, 4, 3, 2, 1, 2, 3, 4, 5, 6, 7, 8},
                          {1, 2, 3, 4, 5, 6, 7, 8, 9, 8, 7, 6, 5, 4, 3, 2},
                          {9, 8, 7, 6, 5, 4, 3, 2, 1, 2, 3, 4, 5, 6, 7, 8},
                          {1, 2, 3, 4, 5, 6, 7, 8, 9, 8, 7, 6, 5, 4, 3, 2},
                          {9, 8, 7, 6, 5, 4, 3, 2, 1, 2, 3, 4, 5, 6, 7, 8},
                          {1, 2, 3, 4, 5, 6, 7, 8, 9, 8, 7, 6, 5, 4, 3, 2},
                          {9, 8, 7, 6, 5, 4, 3, 2, 1, 2, 3, 4, 5, 6, 7, 8},
                          {1, 2, 3, 4, 5, 6, 7, 8, 9, 8, 7, 6, 5, 4, 3, 2},
                          {9, 8, 7, 6, 5, 4, 3, 2, 1, 2, 3, 4, 5, 6, 7, 8},
                          };

int motor_rotate;  //1cm進むのに必要な回転数

//所定の位置まで進むのに必要なループ回数
void motorLoopCount(int array[][], int rotate) {
  for (int i = 0; i < 16; i++) {
    for (int j = 0; j < 16; j++) {
      int loopCount = array[i][j];
      array[i][j] = loopCount * rotate;
    }
  }
}



//モーターを動かす
//1,2,3は例、4で個々に動かす
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

void move_motor3(byte motor) {
  digitalWrite(STBY, HIGH);
  digitalWrite(latchPin, HIGH);
  digitalWrite(clockPin, LOW);
  shiftOut(dataPin, clockPin, LSBFIRST, motor);
  digitalWrite(latchPin, HIGH);
  analogWrite(PWM, 255);
}

//全部動かす
void move_motor4(byte motor[]) {
  digitalWrite(STBY, HIGH);
  digitalWrite(latchPin, LOW);
  digitalWrite(clockPin, LOW);
  for (int i = 0; i < 64; i++) {
    shiftOut(dataPin, clockPin, LSBFIRST, motor[i]);
  }
  digitalWrite(latchPin, HIGH);
  analogWrite(PWM, 255);
}

void setup() {
  Serial.begin(9600);

  //バッファ配列の初期化
  //writebuffer: 奇数番目は3,4個目、偶数番目は1,2個目のモーターが動く
  for (int i = 0; i < 64; i++) {
    if (i % 2 == 0) {
      writeBuffer[i] = B10100000;
    } else {
      writeBuffer[i] = B00001010;
    }
    dataModes[i] = B11111111;
    readBuffer[i] = B11111111;
  }

  //pin_moveの初期化
  for (int i = 0; i < 64; i++) {
    pin_move[i] = B10101010;
  }

  //latch, clock, data
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);

  pinMode(STBY, OUTPUT);

  analogWrite(PWM, 255);
}

void loop() {

  move_motor4(writeBuffer);
  delay(1000);
  digitalWrite(STBY, LOW);
  delay(1000);

  //pin_moveの更新
  for (int i = 0; i < 64; i++) {
    int a = i / 4;
    int b = i % 4;
    for (int j = 0; j < 4; j++) {
      byte pin_i = pin_move[i];
      if (test_array[a][b * 4 + 1] == 0) bitWrite(pin_i, (9 - b * 2), 0);
      else bitWrite(pin_i, (9 - b * 2), 1);
      pin_move[i] = pin_i;
    }
  }

  //test_arrayの更新
  for (int i = 0; i < 16; i++) {
    for (int j = 0; j < 16; j++) {
      int array_ij = test_array[i][j];
      array_ij--;
      test_array[i][j] = array_ij;
    }
  }
}
