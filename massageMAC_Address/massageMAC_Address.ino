#include <WiFi.h>

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  WiFi.persistent(false);
  WiFi.mode(WIFI_STA);
  WiFi.begin(); // กระตุ้นการทำงานของระบบ Wi-Fi
  delay(500);

  Serial.println();
  Serial.print("Mac Address: ");
  Serial.println(WiFi.macAddress());
}

void loop() {}
