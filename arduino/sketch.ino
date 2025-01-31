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
String right_key = "000";
String current_key = "001";
String loaded_key = "002";
String min_key = "003";
int current_key_int;
int right_key_int;
int loaded_key_int;
int min_key_int;

void setup() {
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
void loop() {
  if (Serial.available() > 0) {
    String data = Serial.readStringUntil('\n');
    
    



    Serial.print("You sent me: ");
    Serial.println(data);
    if (data == "unloading"){
      delay(50);
      String right_key = Serial.readStringUntil('\n');
      String current_key = Serial.readStringUntil('\n');
      right_key_int = right_key.toInt();
      current_key_int = current_key.toInt();
      Serial.println(right_key_int);
      Serial.println(current_key_int); 
      unloading();
    }
    
    if (data == "loading"){
      delay(50);
      String loaded_key = Serial.readStringUntil('\n');
      String min_key = Serial.readStringUntil('\n');
      loaded_key_int = loaded_key.toInt();
      min_key_int = min_key.toInt();
      Serial.println(loaded_key_int);
      Serial.println(min_key_int); 
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
  String min_key = Serial.readStringUntil('\n');
  min_key_int = min_key.toInt();

  stepper.step(133*(min_key_int - loaded_key_int));
  
  digitalWrite(redpin, 0);
  digitalWrite(greenpin, 1);
}


void unloading(){
  Serial.println(right_key_int);
  Serial.println(current_key_int);
  digitalWrite(redpin, 1);
  digitalWrite(greenpin, 0);

  stepper.step(133*(right_key_int - current_key_int));

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
  if (digitalRead(button)==0){
    exit(0);
  }
}
