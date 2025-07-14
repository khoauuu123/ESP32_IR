#include <Arduino.h>
#include "WiFi.h"
#include <IRremoteESP8266.h>
#include <IRac.h>
#include <IRutils.h>
#include <ArduinoJson.h>
DynamicJsonDocument jsonDoc(2048);
DynamicJsonDocument jsonDoc_send(2048); //DynamicJsonDocument jsonDoc_send(2048);
#include <SPI.h>
#include <Ethernet.h>
#include <EEPROM.h>
#include <HTTPClient.h>

const uint16_t kIrLed = 19;  // The ESP GPIO pin to use that controls the IR LED.
IRac ac(kIrLed);  // Create a A/C object using GPIO to sending messages with.

#define BT    32
#define LEDTT 21

#include <OneWire.h>
#include <DallasTemperature.h>
#define ONE_WIRE_BUS 18
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

void setup()
{
  Serial.begin(115200);
  sensors.begin();
  pinMode(BT,    INPUT_PULLUP);
  pinMode(LEDTT, OUTPUT);
  digitalWrite(LEDTT, 0);
  delay(1000);

  ac.next.protocol = decode_type_t::SAMSUNG;  // Set a protocol to use.
  ac.next.model = 1;  // Some A/Cs have different models. Try just the first.
  ac.next.mode = stdAc::opmode_t::kCool;  // Run in cool mode initially.
  ac.next.celsius = true;  // Use Celsius for temp units. False = Fahrenheit
  ac.next.degrees = 25;  // 25 degrees.
  ac.next.fanspeed = stdAc::fanspeed_t::kMedium;  // Start the fan at medium.
  ac.next.swingv = stdAc::swingv_t::kOff;  // Don't swing the fan up or down.
  ac.next.swingh = stdAc::swingh_t::kOff;  // Don't swing the fan left or right.
  ac.next.light = false;  // Turn off any LED/Lights/Display that we can.
  ac.next.beep = false;  // Turn off any beep from the A/C if we can.
  ac.next.econo = false;  // Turn off any economy modes if we can.
  ac.next.filter = false;  // Turn off any Ion/Mold/Health filters if we can.
  ac.next.turbo = false;  // Don't use any turbo/powerful/etc modes.
  ac.next.quiet = false;  // Don't use any quiet/silent/etc modes.
  ac.next.sleep = -1;  // Don't set any sleep time or modes.
  ac.next.clean = false;  // Turn off any Cleaning options if we can.
  ac.next.clock = -1;  // Don't set any current time if we can avoid it.
  ac.next.power = false;  // Initially start with the unit off.

  Serial.println("KHOI DONG THIET BI ...");

}

//==================================================================================================================================
//==================================================================================================================================
//==================================================================================================================================
int dem = 1;
float nhietdo = 0;

unsigned long t1 = 0;

void loop()
{
  if (millis() - t1 > 1000)
  {
    t1 = millis();
    sensors.requestTemperatures();
    nhietdo = sensors.getTempCByIndex(0);
    Serial.println(nhietdo);
  }
  int a = digitalRead(BT);
  if (a == 0)
  {
    dem++;
    if (dem % 2 == 0)
    {
      decode_type_t protocol = (decode_type_t)46;
      if (ac.isProtocolSupported(protocol))
      {
        Serial.println("Protocol " + String(protocol) + " / " +
                       typeToString(protocol) + " is supported.");
        ac.next.protocol = protocol;
        ac.next.fanspeed = stdAc::fanspeed_t::kMedium;  // Start the fan at medium.
        ac.next.swingv = stdAc::swingv_t::kHighest;  // Don't swing the fan up or down.
        ac.next.celsius = true;  // Use Celsius for temp units. False = Fahrenheit
        ac.next.degrees = 26;
        ac.next.power = true;
        Serial.println("BAT MAY LANH 26");
        ac.sendAc();
        delay(500);
      }
    }
    else
    {
      decode_type_t protocol = (decode_type_t)46;
      // If the protocol is supported by the IRac class ...
      if (ac.isProtocolSupported(protocol))
      {
        Serial.println("Protocol " + String(protocol) + " / " +
                       typeToString(protocol) + " is supported.");
        ac.next.protocol = protocol;  // Change the protocol used.
        ac.next.power = false;  // Now we want to turn the A/C off.
        Serial.println("TAT MAY LANH");
        ac.sendAc();
        delay(500);
      }
    }
    delay(200);
  }


  if (Serial.available())
  {
    char c = Serial.read();
    if (c == '1')
    {
      decode_type_t protocol = (decode_type_t)46;
      if (ac.isProtocolSupported(protocol))
      {
        Serial.println("Protocol " + String(protocol) + " / " +
                       typeToString(protocol) + " is supported.");
        ac.next.protocol = protocol;
        ac.next.fanspeed = stdAc::fanspeed_t::kMedium;  // Start the fan at medium.
        ac.next.swingv = stdAc::swingv_t::kHighest;  // Don't swing the fan up or down.
        ac.next.celsius = true;  // Use Celsius for temp units. False = Fahrenheit
        ac.next.degrees = 26;
        ac.next.power = true;
        Serial.println("BAT MAY LANH 26");
        ac.sendAc();
        delay(500);
      }
    }

    if (c == 'a')
    {
      decode_type_t protocol = (decode_type_t)46;
      if (ac.isProtocolSupported(protocol))
      {
        Serial.println("Protocol " + String(protocol) + " / " +
                       typeToString(protocol) + " is supported.");
        ac.next.protocol = protocol;
        ac.next.fanspeed = stdAc::fanspeed_t::kMedium;  // Start the fan at medium.
        ac.next.swingv = stdAc::swingv_t::kHighest;  // Don't swing the fan up or down.
        ac.next.celsius = true;  // Use Celsius for temp units. False = Fahrenheit
        ac.next.degrees = 30;
        ac.next.power = true;
        Serial.println("BAT MAY LANH 30");
        ac.sendAc();
        delay(500);
      }
    }

    if (c == '2')
    {
      decode_type_t protocol = (decode_type_t)46;
      // If the protocol is supported by the IRac class ...
      if (ac.isProtocolSupported(protocol))
      {
        Serial.println("Protocol " + String(protocol) + " / " +
                       typeToString(protocol) + " is supported.");
        ac.next.protocol = protocol;  // Change the protocol used.
        ac.next.power = false;  // Now we want to turn the A/C off.
        Serial.println("TAT MAY LANH");
        ac.sendAc();
        delay(500);
      }
    }

    if (c == '3')
    {
      decode_type_t protocol = (decode_type_t)46;
      if (ac.isProtocolSupported(protocol))
      {
        Serial.println("Protocol " + String(protocol) + " / " +
                       typeToString(protocol) + " is supported.");
        ac.next.protocol = protocol;
        ac.next.celsius = true;
        ac.next.degrees = 28;
        ac.next.power = false;
        Serial.println("BAT 28");
        ac.sendAc();  // Send the message.
        delay(500);  // Wait 1 second.
      }
    }


    if (c == '4')
    {
      decode_type_t protocol = (decode_type_t)46;
      // If the protocol is supported by the IRac class ...
      if (ac.isProtocolSupported(protocol))
      {
        Serial.println("Protocol " + String(protocol) + " / " +
                       typeToString(protocol) + " is supported.");
        ac.next.protocol = protocol;  // Change the protocol used.
        ac.next.celsius = true;  // Use Celsius for temp units. False = Fahrenheit
        ac.next.degrees = 25;  // 25 degrees.

        ac.next.power = false;  // Now we want to turn the A/C off.
        Serial.println("BAT 25");
        ac.sendAc();  // Send the message.
        delay(500);  // Wait 1 second.
      }
    }
  }



}//end loop

//==================================================================================================================================
//==================================================================================================================================
//==================================================================================================================================

void check_ML()
{
  // For every protocol the library has ...
  /*for (int i = 1; i < kLastDecodeType; i++)
    {
    decode_type_t protocol = (decode_type_t)i;
    // If the protocol is supported by the IRac class ...
    if (ac.isProtocolSupported(protocol))
    {
      Serial.println("Protocol " + String(protocol) + " / " +
                     typeToString(protocol) + " is supported.");
      ac.next.protocol = protocol;  // Change the protocol used.
      ac.next.power = true;  // We want to turn on the A/C unit.
      Serial.println("Sending a message to turn ON the A/C unit.");
      ac.sendAc();  // Have the IRac class create and send a message.
      delay(5000);  // Wait 5 seconds.
      ac.next.power = false;  // Now we want to turn the A/C off.
      Serial.println("Send a message to turn OFF the A/C unit.");
      ac.sendAc();  // Send the message.
      delay(1000);  // Wait 1 second.
    }
    }*/
  //Serial.println("Starting from the begining again ...");
}

void callback(char* topic, byte* payload, unsigned int length) {
  String TOPIC = String(topic);
  String PAYLOAD;
  for (uint8_t i = 0; i < length; i++) {
    PAYLOAD += char(payload[i]);
  }

  Serial.println("TOPIC: " + TOPIC + "   PAYLOAD: " + PAYLOAD);
  deserializeJson(jsonDoc, PAYLOAD);

  JsonObject obj = jsonDoc.as<JsonObject>();
  String method = obj[String("method")];
  String params = obj[String("params")];


  if (method.lastIndexOf("control") != -1)
  {
    deserializeJson(jsonDoc, params);
    JsonObject obj = jsonDoc.as<JsonObject>();
    String sreset = obj[String("power")];
    TT_ML = sreset.toInt();
    Serial.println(TT_ML);
    jsonDoc_send["temp"]       =  ND;
    jsonDoc_send["tempSet"]    = NHIETDO_SETCB ;
    jsonDoc_send["power"]      =  TT_ML;
    
    String jsonString;
    serializeJson(jsonDoc_send, jsonString);
    mqtt.publish("v1/devices/me/telemetry", jsonString.c_str());

    Serial.println("Sending message: ");
    Serial.println(jsonString);
  }

  if (method.lastIndexOf("update_setting") != -1) ESET = 1;
  if (method.lastIndexOf("update_schedule") != -1)
  {
    deserializeJson(jsonDoc, params);
    JsonObject obj = jsonDoc.as<JsonObject>();
    String slich = obj[String("update_schedule")];
    Serial.println("CAP NHAT LS");
    //Serial.println(slich);
    ELS = 1;

  }

}

//TOPIC: v1/devices/me/rpc/request/23   PAYLOAD: {"method":"control","params":{"power":1}}
// TOPIC: v1/devices/me/rpc/request/85   PAYLOAD: {"method":"update_schedule","params":{"update_schedule":true}}



void reconnect()
{
  if (MODE_INETRNET == 0)
  {
    while (!mqtt.connected())
    {
      eketnoi = 0;
      Serial.print("Attempting MQTT connection...");
      String clientId = "MEI.HN-";
      clientId += String(random(0xffff), HEX);
      //SKEY_ESP
      if (mqtt.connect(clientId.c_str(), SKEY_ESP.c_str(), mqtt_pass))
        //if (mqtt.connect(clientId.c_str(), mqtt_user, mqtt_pass))
      {
        Serial.println("connected");
        mqtt.subscribe("v1/devices/me/rpc/request/+");
      }
      else
      {
        Serial.print("failed, rc=");
        Serial.println(mqtt.state());
      }
      delay(5000);
    }
  }
  else
  {
    while (!mqtt2.connected())
    {

      Serial.print("Attempting MQTT connection...");
      String clientId = "MEI.HN-";
      clientId += String(random(0xffff), HEX);

      if (mqtt2.connect(clientId.c_str(), SKEY_ESP.c_str(), mqtt_pass))
        //if (mqtt.connect(clientId.c_str(), mqtt_user, mqtt_pass))
      {
        Serial.println("connected");
        mqtt2.subscribe("v1/devices/me/rpc/request/+");
      }
      else
      {
        Serial.print("failed, rc=");
        Serial.println(mqtt2.state());
      }
    }
  }

}

void KETNOI_MQTT()
{
  if (MODE_INETRNET == 0)
  {
    mqtt.setServer(mqtt_server, port);
    mqtt.setCallback(callback);
  }

  reconnect();
}

void IRAM_ATTR onTimer() //0.1
{
  DEM_IC++;
  if (DEM_IC >= 20)
  {
    DEM_IC = 0;
    digitalWrite(RS, !digitalRead(RS));
  }

  if (eketnoi == 0) DEM_MATKETNOI++;
  else DEM_MATKETNOI = 0;
  if (DEM_MATKETNOI >= THOIGIAN_MATKETNOI * 100) //10PHUT MAT KET NOI SE RESET LAI
  {
    DEM_MATKETNOI = 0;
    ESP.restart();
    delay(1000);
  }

  if (eketnoi == 1 && EWIFI == 0)
  {
    if (E_OK == 0)
    {
      demapi++;
      if (demapi >= 30)
      {
        demapi = 0;
        ESET = 1;
      }
    }
    DEM_STT++;
    if (DEM_STT >= 100) //1s
    {
      digitalWrite(LED_STT, !digitalRead(LED_STT));
      DEM_STT = 0;
    }
  }

  if (EWIFI == 1) digitalWrite(LED_STT, !digitalRead(LED_STT));  //BAO BAT DAU CHO KET NOI WIFI

}//END SU KIEN TIMER

void SET_TIMER()
{
  My_timer = timerBegin(0, 80, true);
  timerAttachInterrupt(My_timer, &onTimer, true);
  timerAlarmWrite(My_timer, 10000, true); //1.000.000 - 1s =
  timerAlarmEnable(My_timer); //Just Enable

  esp_task_wdt_init(WDT_TIMEOUT, true); // Initialize ESP32 Task WDT
  esp_task_wdt_add(NULL);               // Subscribe to the Task WDT
}
