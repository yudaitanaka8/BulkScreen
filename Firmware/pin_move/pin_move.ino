int latchPin = 8;
int clockPin = 12;
int dataPin = 11;

int STBY = 10;

int PWM = A3;

//ピンを出す配列、最初は全部B10101010
byte pin_move[64];
//ピンを戻す配列、全部B01010101
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

//モーターを動かす
void move_motor(byte motor[]) {
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

  //pin_moveの初期化
  for (int i = 0; i < 64; i++) {
    pin_move[i] = B10101010;
  }

  //pin_returnの初期化
  for (int i = 0; i < 64; i++) {
    pin_return[i] = B01010101;
  }

  //latch, clock, data
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);

  pinMode(STBY, OUTPUT);

  analogWrite(PWM, 255);
}

//順回転の時は1ループで1cm動かす、逆回転の時は1ループで完結
void loop() {

  move_motor(pin_move);
  delay(1000);  // (1000 / (1秒で進む距離)[cm])
  digitalWrite(STBY, LOW);

  //pin_moveの更新
  for (int i = 0; i < 64; i++) {
    int a = i / 4;
    int b = i % 4;
    for (int j = 0; j < 4; j++) {
      byte pin_i = pin_move[i];
      if (test_array[a][b * 4 + j] == 0) bitWrite(pin_i, (7 - j * 2), 0);
      else bitWrite(pin_i, (7 - j * 2), 1);
      pin_move[i] = pin_i;
    }
  }

  //モーターを順回転させるか逆回転させるか判定, 0なら逆回転になる
  int rotate_direction = 0;
  
  //test_arrayの更新
  for (int i = 0; i < 16; i++) {
    for (int j = 0; j < 16; j++) {
      int array_ij = test_array[i][j];
      if (array_ij != 0) array_ij--;
      else rotate_direction++;
      test_array[i][j] = array_ij;
    }
  }

  if (rotate_direction == 0) {
    move_motor(pin_return);
    delay(3000);   // ((1000 / (1秒で進む距離)[cm]) * (最大のピンの長さ))
    digitalWrite(STBY, LOW);
  }
}
