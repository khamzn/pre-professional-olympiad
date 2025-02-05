#include <Stepper.h>

#define motor1_A 4
#define motor1_B 5
#define motor2_A 6
#define motor2_B 7

#define redpin 10
#define greenpin 9

#define button 11

Stepper stepper(200, 2, 3);

int buttonState = digitalRead(button);

int redpinState;
int greenpinState;

String data;
String right_key = "000";
String current_key = "001";
String loaded_key = "002";
String min_key = "003";
int current_key_int;
int right_key_int;
int loaded_key_int;
int min_key_int;
int steps_unl_int;
int steps_l_int;

void setup() {
  Serial.begin(9600);
  stepper.setSpeed(300);
  
  pinMode(motor1_A, OUTPUT);
  pinMode(motor1_B, OUTPUT);
  pinMode(motor2_A, OUTPUT);
  pinMode(motor2_B, OUTPUT);

  pinMode(button, INPUT_PULLUP);
  pinMode(redpin, OUTPUT);
  pinMode(greenpin, OUTPUT);
}
void loop() {
  if (Serial.available() > 0) {
    String data = Serial.readStringUntil('\n');
    
    if (data == "unloading"){
      String steps_unl = Serial.readStringUntil('\n');
      steps_unl_int = steps_unl.toInt();
      //Serial.println(right_key_int);
      //Serial.println(current_key_int); 
      unloading();
    }

    if (data == "loading"){
      String steps_l = Serial.readStringUntil('\n');
      steps_l_int = steps_l.toInt();
      //Serial.println(min_key_int); 
      loading();
    }

    if (data == "red"){
      redpinState = 1;
      greenpinState = 0;
    }
    if (data == "green"){
      redpinState = 0;
      greenpinState = 1;
    }
      
  }
  //stepper.step(1000);
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
  //buttonState = 1;
  //while (buttonState == 1){
    //digitalWrite(redpin, 1);
    //digitalWrite(greenpin, 0);
    //buttonState = digitalRead(button);
  //}
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

  stepper.step(steps_l_int);
  
  Serial.println("loading end");

  //digitalWrite(redpin, 0);
  //digitalWrite(greenpin, 1);
}


void unloading(){
  digitalWrite(redpin, 1);
  digitalWrite(greenpin, 0);

  stepper.step(steps_unl_int);

  delay(5000);

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
  data = "0";
}

void yield(){
  if (digitalRead(button)==0){
    exit(0);
  }
}
