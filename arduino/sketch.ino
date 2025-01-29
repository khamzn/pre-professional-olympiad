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
    loading();
  }

  if (data == "unloading"){
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

  digtalWrite(redpin, redpinState);
  digtalWrite(greenpin, greenpinState);
}

void loading(){
  while (buttonState == 1){
    digitalWrite(redpin, 1);
    digitalWrite(greenpin, 0);
  }
  digitalWrite(redpin, 1);
  digitalWrite(greenpin, 0);

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

  Serial.println("loading end")
  digitalWrite(redpin, 0);
  digitalWrite(greenpin, 1);
}


void unloading(){
  digitalWrite(redpin, 1);
  digitalWrite(greenpin, 0);

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

  Serial.println("unloading end")
  digitalWrite(redpin, 0);
  digitalWrite(greenpin, 1);
}
