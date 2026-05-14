// L298N Pins
#define MOTOR_A_IN1 2
#define MOTOR_A_IN2 3
#define MOTOR_A_EN  5 

#define MOTOR_B_IN1 4
#define MOTOR_B_IN2 7
#define MOTOR_B_EN  6 

// Movement States
const int MOVE_FORWARD = 1;
const int MOVE_BACKWARD = 2;
const int MOVE_LEFT    = 3;
const int MOVE_RIGHT   = 4;
const int MOVE_STOP    = 5;

void setup() {
  pinMode(MOTOR_A_IN1, OUTPUT);
  pinMode(MOTOR_A_IN2, OUTPUT);
  pinMode(MOTOR_A_EN,  OUTPUT);
  pinMode(MOTOR_B_IN1, OUTPUT);
  pinMode(MOTOR_B_IN2, OUTPUT);
  pinMode(MOTOR_B_EN,  OUTPUT);
}

// Low-level: Controls one physical motor
void setMotor(int in1, int in2, int en, int speed, bool reverse) {
  digitalWrite(in1, reverse ? LOW : HIGH);
  digitalWrite(in2, reverse ? HIGH : LOW);
  analogWrite(en, speed);
}

// High-level: Controls the "Car"
void moveCar(int direction, int speed) {
  switch (direction) {
    case MOVE_FORWARD:
      setMotor(MOTOR_A_IN1, MOTOR_A_IN2, MOTOR_A_EN, speed, false);
      setMotor(MOTOR_B_IN1, MOTOR_B_IN2, MOTOR_B_EN, speed, false);
      break;
    case MOVE_LEFT:
      setMotor(MOTOR_A_IN1, MOTOR_A_IN2, MOTOR_A_EN, speed, true);  // Motor A Back
      setMotor(MOTOR_B_IN1, MOTOR_B_IN2, MOTOR_B_EN, speed, false); // Motor B Forward
      break;
    case MOVE_STOP:
      analogWrite(MOTOR_A_EN, 0);
      analogWrite(MOTOR_B_EN, 0);
      break;
  }
}
void moveForward(){
  
}

void loop() {
  moveCar(MOVE_FORWARD, 200);
  delay(2000);
  moveCar(MOVE_STOP, 0);
  delay(1000);
}