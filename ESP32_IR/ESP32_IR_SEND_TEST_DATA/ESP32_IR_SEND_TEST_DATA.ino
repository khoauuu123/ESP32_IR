#include <Arduino.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>

const uint16_t IrPin = 19;

IRsend irsend(IrPin);  // Set the GPIO to be used to sending the message.

uint16_t powerOn[] = { 9058, 4438, 512, 1728, 512, 1726, 512, 610, 510, 608, 510, 610, 510, 608, 510, 608, 510, 1728, 510, 610, 510, 1728, 510,
                       1728, 510, 610, 510, 608, 510, 610, 510, 610, 510, 608, 510, 610, 510, 608, 510, 1728, 512, 608, 510, 610, 510, 608, 510, 610, 510, 610, 510,
                       610, 510, 1730, 510, 610, 510, 610, 510, 608, 510, 610, 510, 608, 510, 608, 510, 610, 510, 608, 510, 608, 510, 608, 510, 608, 510, 608, 510,
                       608, 510, 610, 510, 608, 510, 608, 510, 610, 510, 608, 510, 608, 510, 608, 510, 608, 510, 610, 510 };  // UNKNOWN AB89254F
// UNKNOWN F44662C3

uint16_t poweroff[] = { 8986, 4510, 532, 1704, 534, 1706, 532, 614, 504, 614, 506, 614, 506, 588, 530, 612, 506, 1706, 532, 614, 506, 1706, 534,
                        1706, 532, 588, 530, 614, 506, 588, 530, 590, 528, 614, 506, 614, 506, 612, 506, 588, 530, 614, 506, 614, 506, 612, 506, 614, 506, 614, 506,
                        614, 506, 1706, 532, 612, 508, 612, 506, 590, 528, 614, 506, 614, 504, 592, 530, 612, 504, 590, 530, 614, 506, 590, 530, 588, 532, 588, 530,
                        590, 528, 612, 506, 614, 506, 612, 504, 614, 506, 588, 532, 614, 504, 592, 528, 614, 506, 612, 506 };  // UNKNOWN 8E180C61

uint8_t samsungState[kSamsungAcStateLength] = {
  0x02, 0x92, 0x0F, 0x00, 0x00, 0x00, 0xF0,
  0x01, 0xE2, 0xFE, 0x71, 0x40, 0x11, 0xF0
};

void setup() {
  Serial.begin(115200);
  delay(200);

  irsend.begin();
  Serial.println("Irsend Run ...");
}

void loop() {
  Serial.println("a powerOn capture from IRrecvDumpV2");
  irsend.sendRaw(powerOn, sizeof(powerOn) / sizeof(powerOn[0]), 38);  // Send a raw data capture at 38kHz.
  delay(2000);
  Serial.println("a Samsung A/C state from IRrecvDumpV2");
  irsend.sendSamsungAC(samsungState);
  delay(2000);
}


