#include <ESP8266WiFi.h>
#include <espnow.h>

const int FORW = 5;    
const int BACK = 4;    
const int LEFT = 12;   
const int RIGHT = 14;  

String strg_msg;

int varX;
int varY;
typedef struct struct_message {
    char a[32];
} struct_message;

struct_message myData;

void OnDataRecv(uint8_t * mac, uint8_t *incomingData, uint8_t len) {
  memcpy(&myData, incomingData, sizeof(myData));
  
  strg_msg = String(myData.a);
  int delimiterIndex = strg_msg.indexOf('|');
  if (delimiterIndex != -1) {
        String part1 = strg_msg.substring(0, delimiterIndex);
        String part2 = strg_msg.substring(delimiterIndex + 1,delimiterIndex + 4);
        varX = part1.toInt();
        varY = part2.toInt();
        // Serial.print(varX);
        // Serial.print(" | ");
        // Serial.println(varY);
    }
}
void setup() {
  pinMode(FORW, OUTPUT);
  pinMode(BACK, OUTPUT);
  pinMode(LEFT, OUTPUT);
  pinMode(RIGHT, OUTPUT);
  Serial.begin(9600);
  WiFi.mode(WIFI_STA);
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);
  esp_now_register_recv_cb(OnDataRecv);
}
 
void loop() {

  if(varX > 370){   
      digitalWrite(FORW, HIGH);
      digitalWrite(BACK, LOW);
    }

    if(varX < 330 && varX > 0){
      digitalWrite(FORW, LOW);
      digitalWrite(BACK, HIGH);     
    }

    if(varY > 170){
      digitalWrite(LEFT, HIGH);
      digitalWrite(RIGHT,LOW);    
    }

    if(varY < 130 && varY > 0){
      digitalWrite(LEFT, LOW);
      digitalWrite(RIGHT,HIGH);      
    }

    if(varX > 330 && varX < 370){ 
      digitalWrite(FORW, LOW);
      digitalWrite(BACK, LOW);      
    }

    if(varY > 120 && varY < 180){
      digitalWrite(LEFT, LOW);
      digitalWrite(RIGHT,LOW);      
    }  
}
