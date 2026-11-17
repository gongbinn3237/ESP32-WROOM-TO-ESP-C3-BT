#include <esp_now.h>
#include <WiFi.h>

const int led1Pin = 3; // LED ตัวที่ 1 ต่อที่ขา GPIO 13
//const int led2Pin = 12; // LED ตัวที่ 2 ต่อที่ขา GPIO 12

bool led1State = LOW;
//bool led2State = LOW;
unsigned long lastReceiveTime = 0; 

typedef struct struct_message {
  int command;
} struct_message;

struct_message myData;

void OnDataRecv(const esp_now_recv_info_t *info, const uint8_t *incomingData, int len) {
  memcpy(&myData, incomingData, sizeof(myData));
  
  if (millis() - lastReceiveTime < 200) {
    return; 
  }
  lastReceiveTime = millis();

  Serial.print("Bytes received: ");
  Serial.println(len);
  
  // ตรวจสอบคำสั่งที่ส่งมาจากปุ่มแต่ละตัว
  if (myData.command == 1) {
    led1State = !led1State;
    digitalWrite(led1Pin, led1State);
    Serial.print("LED 1 State changed to: ");
    Serial.println(led1State ? "ON" : "OFF");
  } 
  /*else if (myData.command == 2) {
    led2State = !led2State;
    digitalWrite(led2Pin, led2State);
    Serial.print("LED 2 State changed to: ");
    Serial.println(led2State ? "ON" : "OFF");
  }*/
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  pinMode(led1Pin, OUTPUT);
 // pinMode(led2Pin, OUTPUT);
  
  // ตั้งค่าสถานะเริ่มต้นเป็นปิดทั้งหมด
  digitalWrite(led1Pin, LOW);
 // digitalWrite(led2Pin, LOW);

  WiFi.mode(WIFI_STA);
  WiFi.begin();
  delay(500);

  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  esp_now_register_recv_cb(OnDataRecv);
  Serial.println("Receiver Ready for 2 Buttons!");
}

void loop() {
}
