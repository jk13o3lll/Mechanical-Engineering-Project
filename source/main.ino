#include <SoftwareSerial.h>
#include "AFMotor_Mega4DC.h"
//#include <AFMotor.h>

// === Define bluetooth message ===
//only use read() (1 byte), that means the range of number is 0~255 (enough)
//keydown
#define BM_MOVE_FORWORD   1
#define BM_MOVE_BACKWORD  2
#define BM_TURN_LEFT      3
#define BM_TURN_RIGHT     4
#define BM_RISE_UP        5
#define BM_RISE_DOWN      6
#define BM_IN_PUSH        7
#define BM_IN_PULL        8
#define BM_OUT_PUSH       9
#define BM_OUT_PULL       10
#define BM_INOUT_PUSH     11
#define BM_INOUT_PULL     12
//key up
#define BM_FOOT_STOP    13
#define BM_RISE_STOP    14
#define BM_INOUT_STOP   15
//slide bar
#define BM_ADJUST_SPEED_FOOT  16
#define BM_ADJUST_SPEED_RISE  17
#define BM_ADJUST_SPEED_INOUT 18

// === Motor structure (for L298N) ===
//L293D just use the class in library
struct Motor
{
  int pin[2];//pin1, pin2
  int speeed;// (0~255 or ...) critical value define in app slide bar
  bool dir_correction;//0 normal, 1 opposite dir
  
  void init(int p1, int p2, bool dir_correc);
  void setSpeeed(byte v);
  void ruun(int dir);//1 positive, 0 stop, -1 negative
};
void Motor::init(int p1, int p2, bool dir_correc){
  pin[0] = p1; pinMode(p1, OUTPUT); analogWrite(p1, 0);
  pin[1] = p2; pinMode(p2, OUTPUT); analogWrite(p2, 0);
  dir_correction = dir_correc;
  speeed = 0;
}
void Motor::setSpeeed(byte v){
  speeed = v>=0 && v<=255 ? v : 0;
}
void Motor::ruun(int dir){
  dir = dir_correction? -dir : dir;
  if(dir == 0){
    analogWrite(pin[0], 0);
    analogWrite(pin[1], 0);
  }else if(dir == 1){
    analogWrite(pin[0], speeed);
    analogWrite(pin[1], 0);
  }else if(dir == -1){
    analogWrite(pin[0], 0);
    analogWrite(pin[1], speeed);
  }
}

// === Global variables ===
SoftwareSerial BT(51, 53);
Motor left, right;
AF_DCMotor rise(4), in(1), out(2);
byte bt_msg;

// === functions ===
void setup()
{
  Serial.begin(9600); // for test
  Serial.println("BTTTTTTTT is ready");
  BT.begin(38400);

  left.init(13, 7, false);
  right.init(4, 2, false);
  rise.setSpeed(0); rise.run(RELEASE);
  in.setSpeed(0);   in.run(RELEASE);
  out.setSpeed(0);  out.run(RELEASE);
}

void loop()
{
  byte temp;
  if(BT.available())
  {
    bt_msg = BT.read();
    //Serial.println(bt_msg);
    switch(bt_msg)
    {
      case BM_MOVE_FORWORD:   left.ruun(1);  right.ruun(1);   break;
      case BM_MOVE_BACKWORD:  left.ruun(-1); right.ruun(-1);  break;
      case BM_TURN_LEFT:      left.ruun(-1); right.ruun(1);   break;
      case BM_TURN_RIGHT:     left.ruun(1);  right.ruun(-1);  break;
      case BM_RISE_UP:          rise.run(FORWARD);  break;
      case BM_RISE_DOWN:        rise.run(BACKWARD); break;
      case BM_IN_PUSH:          in.run(FORWARD);    break;
      case BM_IN_PULL:          in.run(BACKWARD);   break;
      case BM_OUT_PUSH:         out.run(FORWARD);   break;
      case BM_OUT_PULL:         out.run(BACKWARD);  break;
      case BM_INOUT_PUSH:       in.run(FORWARD);  out.run(FORWARD);   break;
      case BM_INOUT_PULL:       in.run(BACKWARD); out.run(BACKWARD);  break;
      case BM_FOOT_STOP:        left.ruun(0); right.ruun(0);  break;
      case BM_RISE_STOP:        rise.run(RELEASE); break;  //or use rise.run(BRAKE);
      case BM_INOUT_STOP:       in.run(RELEASE); out.run(RELEASE);  break;
      case BM_ADJUST_SPEED_FOOT:
        while(1){
          if(BT.available()){
            temp = BT.read();
            if(temp>0 && temp <=255)
               break;
          }
        }
        //Serial.println(temp);
        left.setSpeeed(temp); right.setSpeeed(temp);
        break;
      case BM_ADJUST_SPEED_RISE:
        while(1){
          if(BT.available()){
            temp = BT.read();
            if(temp>0 && temp <=255)
               break;
          }
        }
        //Serial.println(temp);
        rise.setSpeed(temp);
        break;
      case BM_ADJUST_SPEED_INOUT:
        while(1){
          if(BT.available()){
            temp = BT.read();
            if(temp>0 && temp <=255)
               break;
          }
        }
        //Serial.println(temp);
        in.setSpeed(temp);  out.setSpeed(temp);
        break;
    }
  }
}

