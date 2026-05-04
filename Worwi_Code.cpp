#include <WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// --- ตั้งค่า WiFi (ของ Wokwi) และ MQTT ---
const char* ssid = "Wokwi-GUEST";
const char* password = "";
const char* mqtt_server = "broker.hivemq.com";

WiFiClient espClient;
PubSubClient client(espClient);

// --- กำหนดขาอุปกรณ์ ---
const int moisturePin = 34; // ขาตัวต้านทานปรับค่าได้ (จำลองความชื้น)
const int relayPin = 5;     // ขาควบคุม Relay / LED

// --- ตั้งค่า LCD ---
LiquidCrystal_I2C lcd(0x27, 16, 2);

int moisturePercent = 0;
bool isWatering = false;
bool forceWater = false;    // ตัวแปรจำว่าเรากดปุ่ม Force ค้างไว้หรือไม่
const int threshold = 40;   // ถ้าน้อยกว่า 40% ให้รดน้ำอัตโนมัติ

// ฟังก์ชันเชื่อมต่อ WiFi
void setup_wifi() {
  delay(10);
  Serial.println("\nConnecting to WiFi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected");
}

// ฟังก์ชันรับคำสั่งจาก Node-RED (เมื่อกดปุ่ม Force)
void callback(char* topic, byte* payload, unsigned int length) {
  String message = "";
  for (int i = 0; i < length; i++) {
    message += (char)payload[i];
  }
  Serial.print("Command received: ");
  Serial.println(message);

  // รับคำสั่ง Force Water จาก Node-RED
  if (String(topic) == "smartplant/force") {
    if (message == "ON") {
      forceWater = true;
    } else if (message == "OFF") {
      forceWater = false;
    }
  }
}

// ฟังก์ชันเชื่อมต่อ MQTT กลับอัตโนมัติเมื่อหลุด
void reconnect() {
  while (!client.connected()) {
    Serial.print("Connecting to MQTT...");
    String clientId = "ESP32Client-";
    clientId += String(random(0xffff), HEX);
    
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      client.subscribe("smartplant/force"); // เมื่อต่อสำเร็จ ให้รอฟังคำสั่งปุ่มกด
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  
  // ตั้งค่าพินและปิดปั๊มน้ำตอนเริ่มต้น (Active LOW: สั่ง HIGH คือปิด)
  pinMode(moisturePin, INPUT);
  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, HIGH); 
  
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Connecting WiFi");
  
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  
  lcd.clear();
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  // อ่านค่าความชื้นจำลอง (0-4095) แปลงเป็นเปอร์เซ็นต์ (0-100%)
  int rawValue = analogRead(moisturePin);
  moisturePercent = map(rawValue, 0, 4095, 0, 100);

  // เงื่อนไขการทำงานของ Relay (จำไว้ว่าฮาร์ดแวร์คุณเป็น Active LOW)
  // ถ้าความชื้นต่ำกว่า 40 หรือ มีการกดปุ่ม Force ให้เปิดปั๊ม
  if (moisturePercent < threshold || forceWater) {
    digitalWrite(relayPin, LOW);  // สั่ง LOW เพื่อ 'เปิด' Relay (LED ติด)
    isWatering = true;
  } else {
    digitalWrite(relayPin, HIGH); // สั่ง HIGH เพื่อ 'ปิด' Relay (LED ดับ)
    isWatering = false;
  }

  // อัปเดตหน้าจอ LCD
  lcd.setCursor(0, 0);
  lcd.print("Moisture: ");
  lcd.print(moisturePercent);
  lcd.print("%   "); // เว้นวรรคเคลียร์ตัวเลขเก่า

  lcd.setCursor(0, 1);
  lcd.print("Pump: ");
  lcd.print(isWatering ? "ON       " : "OFF      ");

  // ส่งข้อมูลไปยัง Node-RED ทุกๆ 2 วินาที
  static unsigned long lastMsg = 0;
  unsigned long now = millis();
  if (now - lastMsg > 2000) {
    lastMsg = now;
    
    // ส่งความชื้นขึ้นกราฟ
    client.publish("smartplant/moisture", String(moisturePercent).c_str());
    Serial.print("Published Moisture: ");
    Serial.println(moisturePercent);

    // ----------------------------------------------------
    // ส่งสถานะปั๊มน้ำไปโชว์ใน Node-RED (เพิ่มใหม่ตรงนี้ครับ)
    String pumpStatus = isWatering ? "ON" : "OFF";
    client.publish("smartplant/status", pumpStatus.c_str());
    // ----------------------------------------------------
  }
}