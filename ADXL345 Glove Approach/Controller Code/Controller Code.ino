#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>
#include <esp_now.h>
#include <WiFi.h>

uint8_t broadcastAddress[] = {0xC8, 0xF0, 0x9E, 0xF2, 0x73, 0x68};

esp_now_peer_info_t peerInfo;

/* Assign a unique ID to this sensor at the same time */
Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified(12345);

char comm;

void setup(void) {
  Serial.begin(9600);
  
  accel.begin();

  accel.setRange(ADXL345_RANGE_16_G);

  WiFi.mode(WIFI_STA);

  esp_now_init();

  esp_now_register_send_cb(OnDataSent);

  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;

  esp_now_add_peer(&peerInfo);
}

void loop(void) 
{
  
  sensors_event_t event; 
  accel.getEvent(&event);

  float accelX = event.acceleration.x;
  float accelY = event.acceleration.y;
  float accelZ = event.acceleration.z;
 
  /* Display the results (acceleration is measured in m/s^2) */
  Serial.print("X: "); Serial.print(event.acceleration.x); Serial.print("  ");
  Serial.print("Y: "); Serial.print(event.acceleration.y); Serial.print("  ");
  Serial.print("Z: "); Serial.print(event.acceleration.z); Serial.print("  ");Serial.println("m/s^2 ");

  if (accelX < 3 && accelX > -3 && accelY < 3 && accelY > -3) {
    Serial.println("STOP");

    comm = 'S';
  }

  else if (accelX > 3 && accelY > 3) {
    Serial.println("FRONT RIGHT");

    comm = 'I';
  }

  else if (accelX < -3 && accelY > 3) {
    Serial.println("FRONT LEFT");

    comm = 'G';
  }

  else if (accelX < -3 && accelY < -3) {
    Serial.println("BACK LEFT");

    comm = 'H';
  }

  else if (accelX > 3 && accelY < -3) {
    Serial.println("BACK RIGHT");

    comm = 'J';
  }

  else if (accelX > 3) {
    Serial.println("RIGHT");

    comm = 'R';
  }

  else if (accelX < -3) {
    Serial.println("LEFT");

    comm = 'L';
  }

  else if (accelY < -3) {
    Serial.println("BACKWARD");

    comm = 'B';
  }

  else if (accelY > 3) {
    Serial.println("FORWARD");

    comm = 'F';
  }

  esp_now_send(broadcastAddress, (uint8_t *) &comm, sizeof(comm));
}

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}
