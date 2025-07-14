#include <IRremoteESP8266.h>
#include <IRac.h>
#include <IRutils.h>

#define BT 32
#define LED 21
int STATUS_AC = 0;

const uint16_t kIrLed = 19;  // Chân GPIO của ESP32 điều khiển diode hồng ngoại
IRac ac(kIrLed);             // Tạo đối tượng điều hòa với GPIO 19 để gửi tín hiệu

void setup() {
  Serial.begin(115200);
  delay(200);

  pinMode(BT, INPUT_PULLUP);
  pinMode(LED, OUTPUT);
  digitalWrite(LED, 0);

  ac.next.protocol = decode_type_t::SAMSUNG;      // Chọn giao thức điều khiển Samsung
  ac.next.model = 1;                              // Chọn model điều hòa (mặc định là 1)
  ac.next.mode = stdAc::opmode_t::kCool;          // Bật chế độ làm mát (Cool mode)
  ac.next.celsius = true;                         // Sử dụng đơn vị độ C
  ac.next.degrees = 25;                           // Đặt nhiệt độ 25 độ C
  ac.next.fanspeed = stdAc::fanspeed_t::kMedium;  // Tốc độ quạt ở mức trung bình
  ac.next.swingv = stdAc::swingv_t::kOff;         // Tắt chế độ quay dọc
  ac.next.swingh = stdAc::swingh_t::kOff;         // Tắt chế độ quay ngang
  ac.next.light = false;                          // Tắt đèn LED (nếu có)
  ac.next.beep = false;                           // Tắt âm báo (nếu có)
  ac.next.econo = false;                          // Tắt chế độ tiết kiệm điện (Economy)
  ac.next.filter = false;                         // Tắt bộ lọc không khí (nếu có)
  ac.next.turbo = false;                          // Tắt chế độ làm mát nhanh (Turbo)
  ac.next.quiet = false;                          // Tắt chế độ yên tĩnh (Quiet)
  ac.next.sleep = -1;                             // Không cài đặt chế độ ngủ
  ac.next.clean = false;                          // Tắt chế độ tự làm sạch
  ac.next.clock = -1;                             // Không cài đặt giờ
  ac.next.power = false;                          // Khởi động với điều hòa ở trạng thái tắt
  Serial.println("IR RUN SAMSUNG...");
}

void loop() {
  static String data_read_uart = "";
  if (Serial.available() > 0) {
    char c = Serial.read();
    if (c != '\n') {
      data_read_uart += c;
    } else {

      // Reset lại mạch điều khiển
      if (data_read_uart.lastIndexOf("RESET") != -1) {
        Serial.println("RESET_OK");
        delay(500);
        ESP.restart();
      }

      // Mở máy lạnh
      else if (data_read_uart.lastIndexOf("TURN_ON:") != -1) {
        int colonIndex = data_read_uart.indexOf(':');
        if (colonIndex != -1) {
          String numberPart = data_read_uart.substring(colonIndex + 1);
          int TempSet = numberPart.toInt();
          if (TempSet >= 16 && TempSet <= 30) {
            Turn_On_AC(TempSet);
          } else {
            Serial.println("NGOÀI NGƯỠNG CÀI ĐẶC CỦA MÁY LẠNH");
          }
        }
      }

      // Tắc máy lạnh
      else if (data_read_uart.lastIndexOf("TURN_OFF") != -1) {
        Turn_Off_AC();
      }

      // Set nhiệt độ máy lạnh
      else if (data_read_uart.lastIndexOf("SET_TEMP:") != -1) {
        int colonIndex = data_read_uart.indexOf(':');
        if (colonIndex != -1) {
          String numberPart = data_read_uart.substring(colonIndex + 1);
          int TempSet = numberPart.toInt();
          if (TempSet >= 16 && TempSet <= 30) {
            Set_Temp_AC(TempSet);
          } else {
            Serial.println("NGOÀI NGƯỠNG CÀI ĐẶC CỦA MÁY LẠNH");
          }
        }
      }

      data_read_uart = "";  // Reset biến data USB debug
    }
  }


  if (digitalRead(BT) == 0) {
    delay(200);
    if (STATUS_AC) {
      STATUS_AC = 0;
      Turn_Off_AC();
    } else {
      STATUS_AC = 1;
      Turn_On_AC(26);
    }
    while (digitalRead(BT) == 0) {
      delay(50);
    }
  }
}

void Turn_On_AC(int Temperature_Setting) {
  decode_type_t protocol = (decode_type_t)46;
  if (ac.isProtocolSupported(protocol)) {
    Serial.println("Protocol " + String(protocol) + " / " + typeToString(protocol) + " is supported.");
    ac.next.protocol = protocol;
    ac.next.fanspeed = stdAc::fanspeed_t::kMedium;  // Start the fan at medium.
    ac.next.swingv = stdAc::swingv_t::kHighest;     // Don't swing the fan up or down.
    ac.next.celsius = true;                         // Use Celsius for temp units. False = Fahrenheit
    ac.next.degrees = Temperature_Setting;
    ac.next.power = true;
    Serial.println("BẬT MÁY LẠNH: " + String(Temperature_Setting));
    ac.sendAc();
    digitalWrite(LED, 1);
    delay(100);
    digitalWrite(LED, 0);
    delay(100);
    digitalWrite(LED, 1);
    delay(100);
    digitalWrite(LED, 0);
    delay(100);
  }
}

void Turn_Off_AC() {
  decode_type_t protocol = (decode_type_t)46;
  if (ac.isProtocolSupported(protocol)) {
    Serial.println("Protocol " + String(protocol) + " / " + typeToString(protocol) + " is supported.");
    ac.next.protocol = protocol;  // Change the protocol used.
    ac.next.power = false;        // Now we want to turn the A/C off.
    Serial.println("TẮC MÁY LẠNH");
    ac.sendAc();
    digitalWrite(LED, 1);
    delay(100);
    digitalWrite(LED, 0);
    delay(100);
    digitalWrite(LED, 1);
    delay(100);
    digitalWrite(LED, 0);
    delay(100);
  }
}

void Set_Temp_AC(int Temperature_Setting) {
  decode_type_t protocol = (decode_type_t)46;
  if (ac.isProtocolSupported(protocol)) {
    Serial.println("Protocol " + String(protocol) + " / " + typeToString(protocol) + " is supported.");
    ac.next.protocol = protocol;
    ac.next.degrees = Temperature_Setting;
    Serial.println("SET NHIỆT ĐỘ: " + String(Temperature_Setting));
    ac.sendAc();
    digitalWrite(LED, 1);
    delay(100);
    digitalWrite(LED, 0);
    delay(100);
    digitalWrite(LED, 1);
    delay(100);
    digitalWrite(LED, 0);
    delay(100);
  }
}
