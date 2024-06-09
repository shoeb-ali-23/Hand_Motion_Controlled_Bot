#include <esp_now.h>
#include <WiFi.h>
#include "Wire.h"       
#include "I2Cdev.h"     
#include  "MPU6050.h"    

MPU6050 mpu;
int16_t  ax, ay, az;
int16_t gx, gy, gz;

uint8_t broadcastAddress[] = {0x44,0x17,0x93,0x1F,0x74,0x3A};
// replace above broadcastaddress with the MAC address of your own ESP32 or ESP8266 which you've used in the receiver side
// for getting the MAC address you can use the "" code from this repository upload that code and replace the MAC address with line 11

esp_now_peer_info_t peerInfo;

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nDelivery Status: ");
  Serial.println(status == 0 ? "Delivered Successfully" : "Delivery Fail");
}
 
String msg_buffer;
void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  Wire.begin();
  mpu.initialize();    
  esp_now_register_send_cb(OnDataSent);
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }
}

 
void loop() {
  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

  int X_motion = map(ax,  -17000, 17000, 300, 400); 
  int Y_motion = map(ay, -17000, 17000,  100, 200);  

  String Xmt = String(X_motion); 
  String Ymt = String(Y_motion); 
  msg_buffer = Xmt + "|" + Ymt; 

  // for debugging purpose use below function
  //Serial.println(msg_buffer);

  esp_now_send(broadcastAddress, (uint8_t *) &msg_buffer, sizeof(msg_buffer));
  delay(100);
}
