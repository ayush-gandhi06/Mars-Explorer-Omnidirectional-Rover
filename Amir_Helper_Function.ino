#define FA_IN1 2
#define FA_IN2 3
#define FA_EN
#define FB_IN3 4
#define FB_IN4 5
#define FB_EN 

#define RA_IN1 6
#define RA_IN2 7
#define RA_EN
#define RB_IN3 8
#define RB_IN4 9
#define RB_EN

//use 110 to 110 for 11.1 li-po battery and approximatly 220 for the 4.8 volt battery
int speed =100;

void setup() {
  // put your setup code here, to run once:
  pinMode(FA_IN1, OUTPUT);
  pinMode(FA_IN2, OUTPUT);
  pinMode(FA_EN,  OUTPUT);

  pinMode(FB_IN3, OUTPUT);
  pinMode(FB_IN4, OUTPUT);
  pinMode(FB_EN,  OUTPUT);

  pinMode(RA_IN1, OUTPUT);
  pinMode(RA_IN2, OUTPUT);
  pinMode(RA_EN,  OUTPUT);

  pinMode(RB_IN3, OUTPUT);
  pinMode(RB_IN4, OUTPUT);
  pinMode(RB_EN,  OUTPUT);



}
//direction for the wheel will be determiend by binary, 1 is for cw and 0 is ccw of the wheel
void wheelDirection( int direction, int Motor_IN1, int Motor_IN2){
  if (direction ==1 ){
    digitalWrite(Motor_IN1, HIGH);
    digitalWrite( Motor_IN2, LOW);
    analogWrite(en, speed);
  }
  else if (direction==0){
    digitalWrite(Motor_IN1, LOW);
    digitalWrite( Motor_IN2, LOW);
    analogWrite(en, 0);
  }
  else{//stop mechanism
    digitalWrite(Motor_IN1, LOW);
    digitalWrite( Motor_IN2, HIGH);
    analogWrite(en, speed);
  }
}
void moveForward(){
  wheelDirection(1, FA_IN1,FA_IN2);
  wheelDirection(1, FB_IN3,FB_IN4);
  wheelDirection(1, RA_IN1,RA_IN2);
  wheelDirection(1, RB_IN3,RB_IN4);
}
void moveBackward(){
  wheelDirection(-1, FA_IN1,FA_IN2);
  wheelDirection(-1, FB_IN3,FB_IN4);
  wheelDirection(-1, RA_IN1,RA_IN2);
  wheelDirection(-1, RB_IN3,RB_IN4);
}
void moveRight(){
  wheelDirection(1, FA_IN1,FA_IN2);
  wheelDirection(-1, FB_IN3,FB_IN4);
  wheelDirection(1, RA_IN1,RA_IN2);
  wheelDirection(-1, RB_IN3,RB_IN4);
}
void moveLeft(){
  wheelDirection(-1, FA_IN1,FA_IN2);
  wheelDirection(1, FB_IN3,FB_IN4);
  wheelDirection(-1, RA_IN1,RA_IN2);
  wheelDirection(1, RB_IN3,RB_IN4);
}
void moveFR(){
  wheelDirection(1, FA_IN1,FA_IN2);
  wheelDirection(0, FB_IN3,FB_IN4);
  wheelDirection(1, RA_IN1,RA_IN2);
  wheelDirection(0, RB_IN3,RB_IN4);
}
void moveFL(){
  wheelDirection(0, FA_IN1,FA_IN2);
  wheelDirection(1, FB_IN3,FB_IN4);
  wheelDirection(0, RA_IN1,RA_IN2);
  wheelDirection(1, RB_IN3,RB_IN4);
}
void moveRR(){
  wheelDirection(0, FA_IN1,FA_IN2);
  wheelDirection(-1, FB_IN3,FB_IN4);
  wheelDirection(0, RA_IN1,RA_IN2);
  wheelDirection(-1, RB_IN3,RB_IN4);
}
void moveRL(){
  wheelDirection(-1, FA_IN1,FA_IN2); 
  wheelDirection(0, FB_IN3,FB_IN4);
  wheelDirection(-1, RA_IN1,RA_IN2);
  wheelDirection(0, RB_IN3,RB_IN4);
}

void moveCW(){
  wheelDirection(1, FA_IN1,FA_IN2);
  wheelDirection(-1, FB_IN3,FB_IN4);
  wheelDirection(-1, RA_IN1,RA_IN2);
  wheelDirection(1, RB_IN3,RB_IN4);
}
void moveCCW(){
  wheelDirection(-1, FA_IN1,FA_IN2);
  wheelDirection(1, FB_IN3,FB_IN4);
  wheelDirection(1, RA_IN1,RA_IN2);
  wheelDirection(-1, RB_IN3,RB_IN4);
}

void Stop(){
  wheelDirection(0, FA_IN1,FA_IN2);
  wheelDirection(0, FB_IN3,FB_IN4);
  wheelDirection(0, RA_IN1,RA_IN2);
  wheelDirection(0, RB_IN3,RB_IN4);
}

void loop() {
  // put your main code here, to run repeatedly:




}












