//なんかよくわからんけど全モーターが同時に動くやつ

#include <Shifty.h>

Shifty shift;

int STBY = 10; //standby

//Motor A
int PWM = A3; //Speed control
int AIN1 = 0; //Direction
int AIN2 = 1; //Direction

//Motor B
int BIN1 = 2; //Direction
int BIN2 = 3; //Direction

int CIN1 = 4; //Direction
int CIN2 = 5; //Direction

int DIN1 = 6; //Direction
int DIN2 = 7; //Direction

void setup(){
Serial.begin(9600);
shift.setBitCount(8);
shift.setPins(11, 12, 8); // Data. Clocl, Latch
  
pinMode(STBY, OUTPUT);

analogWrite(PWM, 255);
}

void loop(){
move(1, 255, 1); //motor 1, full speed, left
move(2, 255, 1); //motor 2, full speed, left

delay(1000); //go for 1 second
//stop(); //stop
//delay(250); //hold for 250ms until move again

move(1, 255, 0); //motor 1, half speed, right
move(2, 255, 0); //motor 2, half speed, right

delay(1000);
//stop();
//delay(250);
}

void move(int motor, int speed, int direction){
//Move specific motor at speed and direction
//motor: 0 for B 1 for A
//speed: 0 is off, and 255 is full speed
//direction: 0 clockwise, 1 counter-clockwise

digitalWrite(STBY, HIGH); //disable standby

boolean inPin1 = HIGH;
boolean inPin2 = LOW;

if(direction == 1){
inPin1 = LOW;
inPin2 = HIGH;
}

if(motor == 1){
shift.writeBit(AIN1, inPin1);
Serial.println(shift.readBit(AIN1));
shift.writeBit(AIN2, inPin2);
Serial.println(shift.readBit(AIN2));
shift.writeBit(BIN1, inPin1);
Serial.println(shift.readBit(BIN1));
shift.writeBit(BIN2, inPin2);
Serial.println(shift.readBit(BIN2));
shift.writeBit(CIN1, inPin1);
Serial.println(shift.readBit(CIN1));
shift.writeBit(CIN2, inPin2);
Serial.println(shift.readBit(CIN2));
shift.writeBit(DIN1, inPin1);
Serial.println(shift.readBit(DIN1));
shift.writeBit(DIN2, inPin2);
Serial.println(shift.readBit(DIN2));
analogWrite(PWM, 255);
//Serial.println(inPin1);
}


}

void stop(){
//enable standby
digitalWrite(STBY, LOW);
}
