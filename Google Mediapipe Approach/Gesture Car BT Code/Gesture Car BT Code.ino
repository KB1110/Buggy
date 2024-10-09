#include <esp_now.h>
#include <WiFi.h>
#include "BluetoothSerial.h"

BluetoothSerial SerialBT;

const int ENA = 14;
const int IN1 = 27;
const int IN2 = 26;

const int ENB = 25;
const int IN3 = 33;
const int IN4 = 32;

const int freq = 500;
const int pwmChannel0 = 0;
const int pwmChannel1 = 1;
const int resolution = 8;

int speed = 255;

char c;

void setup() {
  // Initialize Serial Monitor
  Serial.begin(9600);
  
  SerialBT.begin("Gesture Car");

  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT); 
 
  pinMode(ENB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT); 
  
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);

  digitalWrite(ENA, HIGH);
  digitalWrite(ENB, HIGH);

  ledcSetup(pwmChannel0, freq, resolution);
  ledcAttachPin(ENA, pwmChannel0);

  ledcSetup(pwmChannel1, freq, resolution);
  ledcAttachPin(ENB, pwmChannel1);
}

void loop() {
  // put your main code here, to run repeatedly:
  ledcWrite(pwmChannel0, speed);  
  ledcWrite(pwmChannel1, speed);

  char c = SerialBT.read();

  if (c == 'F'){
    //Forward
    leftForward();
    rightForward();   
  }

  else if (c == 'B') {
    //Backward
    leftBackward();
    rightBackward();
  }

  else if (c == 'L') {
    //Left
    leftBackward();
    rightForward();
  }

  else if (c == 'R') {
    //Right
    rightBackward();
    leftForward();  
  }

  else if (c == 'S') {
    //Backward Left
    leftStop();
    rightStop();
  }

}

void leftForward() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
}

void rightForward() {
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void leftBackward() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
}

void rightBackward() {
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void leftStop() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
}

void rightStop() {
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}
