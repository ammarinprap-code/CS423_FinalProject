# 🌱 โครงงานระบบสมาร์ทฟาร์ม (Smart Farm IoT System)

ระบบจำลองการรดน้ำต้นไม้อัตโนมัติด้วยบอร์ด ESP32 พัฒนาบนแพลตฟอร์ม Wokwi Simulator โดยส่งข้อมูลผ่านโพรโตคอล MQTT เพื่อแสดงผลและควบคุมสั่งการผ่านเว็บแอปพลิเคชัน (Node-RED Dashboard)

## ✨ ฟีเจอร์หลักของระบบ (Features)
- 📊 **Real-time Monitoring:** ตรวจวัดค่าความชื้นในดินและแสดงผลเป็นกราฟบนหน้าเว็บ
- 🤖 **Auto-Watering:** ระบบสั่งเปิดปั๊มน้ำ (Relay) อัตโนมัติเมื่อความชื้นต่ำกว่า 40%
- 🕹️ **Manual Control:** มีปุ่ม FORCE ON / FORCE OFF เพื่อสั่งรดน้ำด้วยตนเองผ่านหน้า Dashboard
- 💾 **Data Logging:** บันทึกข้อมูลความชื้นลงไฟล์ `.csv` เพื่อใช้เป็นฐานข้อมูล

---

## 📁 ไฟล์ใน Repository นี้
- `Wokwi_Code.cpp` : ซอร์สโค้ดภาษา C++ สำหรับบอร์ด ESP32
- `NodeRed.json` : ไฟล์ Flow (JSON) สำหรับ Import หน้า Dashboard และระบบ Backend ลงใน Node-RED
- `Architecture_Diagram.pdf` : แผนภาพโครงสร้างการทำงานของระบบ (Platform Stack)

---

## 🛠️ วิธีการติดตั้งและทดสอบระบบ (Setup & Usage)

### ส่วนที่ 1: ฝั่งฮาร์ดแวร์ (Wokwi Simulator)
1. เข้าสู่โปรเจกต์จำลอง Wokwi ได้ที่ลิงก์นี้: [คลิกเพื่อเปิดระบบปั๊มน้ำ](https://wokwi.com/projects/462640091995998209)
2. หากสร้างโปรเจกต์ใหม่เอง ให้ตรวจสอบในแท็บ **Library Manager** ว่าได้ติดตั้งปลั๊กอินต่อไปนี้แล้ว:
   - `LiquidCrystal I2C` (สำหรับหน้าจอ LCD)
   - `PubSubClient` (สำหรับเชื่อมต่อ MQTT)
3. กดปุ่ม **Play (สีเขียว)** เพื่อเริ่มการจำลอง
4. รอจนกว่าในหน้าต่าง Serial Monitor จะขึ้นคำว่า `WiFi connected` และ `Connecting to MQTT...connected`

### ส่วนที่ 2: ฝั่งเซิร์ฟเวอร์และหน้าจอ (Node-RED)
1. เปิดโปรแกรม Node-RED บนเครื่องคอมพิวเตอร์ของคุณ (รันคำสั่ง `node-red` ใน Command Prompt)
2. เปิดเบราว์เซอร์ไปที่ `http://localhost:1880`
3. ไปที่เมนูขวาบน (ขีด 3 ขีด) -> เลือก **Manage palette** -> แท็บ **Install**
4. ค้นหาและติดตั้งปลั๊กอินชื่อ `node-red-dashboard` 
5. วิธีการ Import โครงงาน:
   - ไปที่เมนู (ขีด 3 ขีด) -> เลือก **Import**
   - อัปโหลดไฟล์ `NodeRed.json` จากนั้นกดปุ่ม Import
6. *ข้อควรระวังสำหรับ Database:* ดับเบิ้ลคลิกที่โหนด `write file` (สีน้ำตาล) แล้วเปลี่ยน Path ในช่อง Filename ให้เป็นโฟลเดอร์ในคอมพิวเตอร์ของคุณ (เช่น `C:\Users\Public\smartfarm.csv`)
7. กดปุ่ม **Deploy** (สีแดงมุมขวาบน)
8. เปิดดูหน้าจอควบคุม (Dashboard) ได้ที่: `http://localhost:1880/ui`

---

## 🤖 การใช้ AI Agent ในการพัฒนา
ในโครงงานนี้ได้มีการใช้งาน **Gemini (Google)** เป็น AI Agent ผู้ช่วยในการพัฒนา ในส่วนต่อไปนี้:
1. ช่วยเขียนและปรับปรุงโครงสร้างโค้ด C++ (Library PubSubClient) สำหรับบอร์ด ESP32
2. ช่วยแก้ไขปัญหา (Debug) วงจร Relay ที่เป็นแบบ Active LOW
3. แนะนำการสร้างและจัดการ Flow ใน Node-RED
4. ช่วยร่างเอกสาร README.md และแนวทางการนำเสนอโครงงาน
