#include <Stepper.h>

#define motor1_A 1
#define motor1_B 2
#define motor2_A 3
#define motor2_B 4

#define redpin 5
#define greenpin 6

#define button 7

Stepper stepper(200, 8, 9);

int buttonState = digitalRead(button);

int redpinState;
int greenpinState;

String data;
int loaded_key;
int right_key;
int current_key;
int min_key;


void setup(){
  Serial.begin(9600);
  stepper.setSpeed(1000);
  
  pinMode(motor1_A, OUTPUT);
  pinMode(motor1_B, OUTPUT);
  pinMode(motor2_A, OUTPUT);
  pinMode(motor2_B, OUTPUT);

  pinMode(button, INPUT_PULLUP);
  pinMode(redpin, OUTPUT);
  pinMode(greenpin, OUTPUT);
}

void loop(){
  buttonState = digitalRead(button);

  if (Serial.available() > 0) {
    data = Serial.readString();
  }

  if (data == "loading"){
    loaded_key = Serial.readString().toInt();
    loading();
  }

  if (data == "unloading"){
    right_key = Serial.readString().toInt();
    current_key = Serial.readString().toInt();
    unloading();
  }

  if (data == "red"){
    redpinState = 1;
    greenpinState = 0;
  }
  if (data == "green"){
    redpinState = 0;
    greenpinState = 1;
  }

  digitalWrite(redpin, redpinState);
  digitalWrite(greenpin, greenpinState);
}


void motor1_left(){
  digitalWrite(motor1_A, 1);
  digitalWrite(motor1_B, 0);
}


void motor1_right(){
  digitalWrite(motor1_A, 0);
  digitalWrite(motor1_B, 1);
}


void motor2_left(){
  digitalWrite(motor2_A, 1);
  digitalWrite(motor2_B, 0);
}


void motor2_right(){
  digitalWrite(motor2_A, 0);
  digitalWrite(motor2_B, 1);
}


void motors_stop(){
  digitalWrite(motor1_A, 0);
  digitalWrite(motor1_B, 0);
  digitalWrite(motor2_A, 0);
  digitalWrite(motor2_B, 0);
}


void loading(){
  while (buttonState == 1){
    digitalWrite(redpin, 1);
    digitalWrite(greenpin, 0);
  }
  digitalWrite(redpin, 1);
  digitalWrite(greenpin, 0);

  motor1_left();
  delay(1000);

  motors_stop();
  motor2_left();
  delay(3000);

  motors_stop();
  motor1_right();
  delay(3000);

  motors_stop();
  motor2_right();
  delay(3000);

  motors_stop();
  motor1_left();
  delay(1000);

  motors_stop();
  Serial.println("loading end");

  while (Serial.available() < 0){
    digitalWrite(redpin, 1);
    digitalWrite(greenpin, 0);
  }
  min_key = Serial.readString().toInt();

  stepper.step(133*(min_key - loaded_key));

  
  digitalWrite(redpin, 0);
  digitalWrite(greenpin, 1);
}


void unloading(){
  digitalWrite(redpin, 1);
  digitalWrite(greenpin, 0);

  stepper.step(133*(right_key - current_key));

  delay(500);

  digitalWrite(motor1_A, 1);
  digitalWrite(motor1_B, 0);
  delay(1000);

  digitalWrite(motor1_A, 0);
  digitalWrite(motor2_A, 1);
  digitalWrite(motor2_B, 0);
  delay(3000);

  digitalWrite(motor2_A, 0);
  digitalWrite(motor1_A, 0);
  digitalWrite(motor1_B, 1);
  delay(3000);

  digitalWrite(motor1_B, 0);
  digitalWrite(motor2_A, 0);
  digitalWrite(motor2_B, 1);
  delay(3000);

  digitalWrite(motor2_B, 0);
  digitalWrite(motor1_A, 1);
  digitalWrite(motor1_B, 0);
  delay(1000);

  digitalWrite(motor1_A, 0);

  

  Serial.println("unloading end");
  digitalWrite(redpin, 0);
  digitalWrite(greenpin, 1);
}

void yield(){
  if (digitalRead(button)){
    exit(0);
  }
}
