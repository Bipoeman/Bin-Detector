#include <Servo.h>

#define SERVO_PIN 9
#define PROXIMITY_PIN 13
#define IR_PIN 2
#define LEDR_PIN 7
#define LEDG_PIN 8

#define IDLE_POSITION 90
#define LEFT_POSITION 45
#define RIGHT_POSITION 135

int readIR(){
  return !digitalRead(IR_PIN);
}

int readProximity(){
  return digitalRead(PROXIMITY_PIN);
}

Servo canSelector;
void setup() {
  Serial.begin(115200);
  canSelector.attach(SERVO_PIN);
  pinMode(PROXIMITY_PIN,INPUT);
  pinMode(IR_PIN,INPUT);
  pinMode(LEDR_PIN,OUTPUT);
  pinMode(LEDG_PIN,OUTPUT);
  digitalWrite(LEDG_PIN,HIGH);
  digitalWrite(LEDR_PIN,HIGH);
}

void loop() {

  if (Serial.available()){
    String data = Serial.readStringUntil('\n');
    canSelector.write(data.toInt());
  }
  if (readIR()){
    canSelector.write(IDLE_POSITION);
    int state = readProximity();
    for (int i = 0; i < 350;i++){
      state |= readProximity();
      delay(1);
    }
    delay(1500);
    if (state){
      digitalWrite(LEDG_PIN,HIGH);
      digitalWrite(LEDR_PIN,LOW);
      canSelector.write(LEFT_POSITION);
    }
    else{
      digitalWrite(LEDR_PIN,HIGH);
      digitalWrite(LEDG_PIN,LOW);
      canSelector.write(RIGHT_POSITION);
    }
    delay(1000);
    digitalWrite(LEDR_PIN,HIGH);
    digitalWrite(LEDG_PIN,HIGH);
    canSelector.write(IDLE_POSITION);
  }
  // digitalWrite(LEDG_PIN,!digitalRead(LEDG_PIN));
  // digitalWrite(LEDR_PIN,!digitalRead(LEDR_PIN));
  // delay(100);
  // Serial.print(readIR());
  // Serial.print(" ");
  // Serial.print(readProximity());
  // Serial.println();
}
