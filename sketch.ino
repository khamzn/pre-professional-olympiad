void setup() {
 // Начинаем работу с Serial портом на скорости 9600 бит/с
 Serial.begin(9600);
 pinMode(13, OUTPUT);
}
void loop() {
  if (Serial.available()>0){
    String data = Serial.readStringUntil('\n');
    
    if (data == "loading"){
      load();
    }
    if (data == "unloading"){
      unload();
    }
  }
}

void load(){
  digitalWrite(13, HIGH);
  delay(1000);
  digitalWrite(13, LOW);
  delay(1000);
}

void unload(){
  digitalWrite(13, HIGH);
  delay(3000);
  digitalWrite(13, LOW);
  delay(3000);
}
