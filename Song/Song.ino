#include <esp_now.h>
#include <WiFi.h>

// MAC Address ของบอร์ดรับ
uint8_t broadcastAddress[] = {0xE8, 0x3D, 0xC1, 0x79, 0x11, 0xC8}; //E8:3D:C1:79:11:C8
 

const int button1Pin = 4;  // ปุ่มที่ 1 ต่อที่ขา GPIO 4
const int button2Pin = 16; // ปุ่มที่ 2 ต่อที่ขา GPIO 16

bool lastButton1State = HIGH;
bool lastButton2State = HIGH;

// โครงสร้างข้อมูล รองรับการส่งรหัสคำสั่ง (command)
typedef struct struct_message {
  int command;
} struct_message;

struct_message myData;

void OnDataSent(const wifi_tx_info_t *info, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  // กำหนดให้ขาพินเป็น INPUT_PULLUP (ใช้สวิตช์ต่อลง GND)
  pinMode(button1Pin, INPUT_PULLUP);
  pinMode(button2Pin, INPUT_PULLUP);

  WiFi.mode(WIFI_STA);
  WiFi.begin();
  delay(500);

  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_register_send_cb(OnDataSent);
  
  esp_now_peer_info_t peerInfo = {};
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  } else {
    Serial.println("Peer added successfully!");
  }
}

void loop() {
  bool currentButton1State = digitalRead(button1Pin);
  bool currentButton2State = digitalRead(button2Pin);

  // เช็คการกดปุ่มที่ 1 (ส่งคำสั่ง command = 1)
  if (lastButton1State == HIGH && currentButton1State == LOW) {
    myData.command = 1; 
    
    esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
    
    if (result == ESP_OK) {
      Serial.println("Button 1 Sent with success");
    } else {
      Serial.println("Error sending Button 1 data");
    }
    delay(200); // หน่วงเวลากองกันส่งซ้ำ
  }

  // เช็คการกดปุ่มที่ 2 (ส่งคำสั่ง command = 2)
  if (lastButton2State == HIGH && currentButton2State == LOW) {
    myData.command = 2; 
    
    esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
    
    if (result == ESP_OK) {
      Serial.println("Button 2 Sent with success");
    } else {
      Serial.println("Error sending Button 2 data");
    }
    delay(200); // หน่วงเวลากองกันส่งซ้ำ
  }
  
  lastButton1State = currentButton1State;
  lastButton2State = currentButton2State;
}
