#include <esp_now.h>
#include <WiFi.h>

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
  Serial.begin(115200);
  
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

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

  // Init ESP-NOW
  esp_now_init();
  
  // Once ESPNow is successfully Init, we will register for recv CB to
  // get recv packer info
  esp_now_register_recv_cb(OnDataRecv);
}

void loop() {
  // put your main code here, to run repeatedly:
  ledcWrite(pwmChannel0, speed);  
  ledcWrite(pwmChannel1, speed);  

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

  else if (c == 'I') {
    //Front Right
    leftForward();
    rightStop();
  }

  else if (c == 'G') {
    //Front Left
    leftStop();
    rightForward();
  }

  else if (c == 'J') {
    //Backward Right
    leftBackward();
    rightStop();
  }

  else if (c == 'H') {
    //Backward Left
    leftStop();
    rightBackward();
  }

  else if (c == 'S') {
    //Stop
    leftStop();
    rightStop();
  }

}

void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&c, incomingData, sizeof(c));
  Serial.print("Bytes received: ");
  Serial.println(len);
  Serial.print("Char: ");
  Serial.println(c);

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
