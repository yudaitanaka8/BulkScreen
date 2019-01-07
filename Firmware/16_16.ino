//16*16がうごきそうなやつ

#include <Shifty.h>

Shifty shift;

int num = 512; // 16 x 16 x 2
String inString = "";
int STBY = 10;

/* 
 7 x 8 shape display
 
 --  --  --  --  --  --  --  --  
  8  10  12  14   0   2   4   6 
 24  26  28  30  16  18 (20) 22
 40  42  44  46  32  34  36  38
 x   x   60  62  48 (50)  52  54
 72  74  76  78  64  66  68  70
 x   90  92  94  80  82  84  x
104 106 108 110  96  98 100 102 
 ^
wire  
*/

/*
// Chair
int sizes[3] = { 3, 9, 4 };
int data[3][9] = { 
  {92, 94, 80}, 
  {60, 62, 48, 76, 78, 64, 92, 94, 80}, 
  {60, 48, 92, 80} 
};
*/

/*
// Stair
int sizes[3] = { 3, 6, 9 };
int data[3][9] = {
  { 12, 14, 0 },
  { 12, 14, 0, 28, 30, 16 },
  { 12, 14, 0, 28, 30, 16, 44, 46, 32 }
};
*/

// Table
int sizes[2] = { 9, 4 };
int data[2][9] = { 
  {60, 62, 48, 76, 78, 64, 92, 94, 80}, 
  {60, 48, 92, 80} 
};





void setup() {
  shift.setBitCount(num);
  Serial.begin(9600);

  //DATA / CLOCK / LATCH
  shift.setPins(11, 12, 8);
  analogWrite(A3, 255);

  pinMode(STBY, OUTPUT);

}

void loop() {

  if (Serial.available() > 0) {
    int inChar = Serial.read();
    if (isDigit(inChar)) {
      inString += (char)inChar;
    }

    if (inChar == '\n') {
      int type = inString.toInt();
      
      Serial.print("I received: ");
      Serial.println(type);

      int layer = type % 10;
      /*
       * move(layer, type)
       * - layer: int 0-10
       * - type: { 0: down, 1: up) 
       */
      if (type < 10) {
        move(layer-1, 1);
      } else if (type < 20) {
        move(layer-1, 0);
      }
  
      
      if (type > 20) {
        /*
         60  62  48 
         76  78  64 
         92  94  80
        */        
        int id = type;
        shift.writeBit(id, HIGH);
        delay(1000);
        shift.writeBit(id, LOW);
        delay(1000);        
      }

      inString = "";
    }
  }
  
}


void move(int layer, int type) {
  /*
   * type: 0 -> down, 1 -> up
   */
  int len = sizes[layer];
  for (int i = 0; i < len; i++) {
    int id = data[layer][i] + type;
    Serial.println(id);
    shift.writeBit(id, HIGH);
  }
  if (type == 1) {
    delay(1000);
  } else {
    delay(1000);    
  }
  for (int i = 0; i < len; i++) {
    int id = data[layer][i] + type;
    Serial.println(id);    
    shift.writeBit(id, LOW);
  }
  delay(1000);
}  

void stop(){
//enable standby
digitalWrite(STBY, LOW);
}
