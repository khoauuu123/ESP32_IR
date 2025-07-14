/* IRremoteESP8266: IRsendDemo - demonstrates sending IR codes with IRsend.

   Version 1.1 January, 2019
   Based on Ken Shirriff's IrsendDemo Version 0.1 July, 2009,
   Copyright 2009 Ken Shirriff, http://arcfn.com

   An IR LED circuit *MUST* be connected to the ESP8266 on a pin
   as specified by kIrLed below.

   TL;DR: The IR LED needs to be driven by a transistor for a good result.

   Suggested circuit:
       https://github.com/crankyoldgit/IRremoteESP8266/wiki#ir-sending

   Common mistakes & tips:
 *   * Don't just connect the IR LED directly to the pin, it won't
       have enough current to drive the IR LED effectively.
 *   * Make sure you have the IR LED polarity correct.
       See: https://learn.sparkfun.com/tutorials/polarity/diode-and-led-polarity
 *   * Typical digital camera/phones can be used to see if the IR LED is flashed.
       Replace the IR LED with a normal LED if you don't have a digital camera
       when debugging.
 *   * Avoid using the following pins unless you really know what you are doing:
 *     * Pin 0/D3: Can interfere with the boot/program mode & support circuits.
 *     * Pin 1/TX/TXD0: Any serial transmissions from the ESP8266 will interfere.
 *     * Pin 3/RX/RXD0: Any serial transmissions to the ESP8266 will interfere.
 *   * ESP-01 modules are tricky. We suggest you use a module with more GPIOs
       for your first time. e.g. ESP-12 etc.
*/


#include <Arduino.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <Arduino_JSON.h>

#if defined(ARDUINO_ARCH_SAMD)
#define Serial SerialUSB
#endif

const uint16_t kIrLed = D4;  // ESP8266 GPIO pin to use. Recommended: 4 (D2).

const char* ssid = "*************************";
const char* password = "*************************";

// Your Domain name with URL path or IP address with path
String openWeatherMapApiKey = "*************************";
// Example:
//String openWeatherMapApiKey = "bd939aa3d23ff33d3c8f5dd1dd435";

// Replace with your country code and city
String city = "Shanghai";
String countryCode = "CN";

// THE DEFAULT TIMER IS SET TO 10 SECONDS FOR TESTING PURPOSES
// For a final application, check the API call limits per hour/minute to avoid getting blocked/banned
unsigned long lastTime = 0;
// Timer set to 10 minutes (600000)
//unsigned long timerDelay = 600000;
// Set timer to 10 seconds (10000)
unsigned long timerDelay = 10000;

String jsonBuffer;

IRsend irsend(kIrLed);  // Set the GPIO to be used to sending the message.

// Example of data captured by IRrecvDumpV2.ino
// Example of data captured by IRrecvDumpV2.ino
uint16_t powerOn[] = {9058, 4438,  512, 1728,  512, 1726,  512, 610,  510, 608,  510, 610,  510, 608,  510, 608,  510, 1728,  510, 610,  510, 1728,  510,
                      1728,  510, 610,  510, 608,  510, 610,  510, 610,  510, 608,  510, 610,  510, 608,  510, 1728,  512, 608,  510, 610,  510, 608,  510, 610,  510, 610,  510,
                      610,  510, 1730,  510, 610,  510, 610,  510, 608,  510, 610,  510, 608,  510, 608,  510, 610,  510, 608,  510, 608,  510, 608,  510, 608,  510, 608,  510,
                      608,  510, 610,  510, 608,  510, 608,  510, 610,  510, 608,  510, 608,  510, 608,  510, 608,  510, 610,  510
                     };  // UNKNOWN AB89254F
// UNKNOWN F44662C3

uint16_t ModeCool[] =  {8986, 4510,  532, 1704,  534, 1706,  532, 614,  504, 614,  506, 614,  506, 588,  530, 612,  506, 1706,  532, 614,  506, 1706,  534,
                        1706,  532, 588,  530, 614,  506, 588,  530, 590,  528, 614,  506, 614,  506, 612,  506, 588,  530, 614,  506, 614,  506, 612,  506, 614,  506, 614,  506,
                        614,  506, 1706,  532, 612,  508, 612,  506, 590,  528, 614,  506, 614,  504, 592,  530, 612,  504, 590,  530, 614,  506, 590,  530, 588,  532, 588,  530,
                        590,  528, 612,  506, 614,  506, 612,  504, 614,  506, 588,  532, 614,  504, 592,  528, 614,  506, 612,  506
                       };  // UNKNOWN 8E180C61
void setup() {
  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());

  Serial.println("Timer set to 10 seconds (timerDelay variable), it will take 10 seconds before publishing the first reading.");
  irsend.begin();
#if ESP8266
  Serial.begin(115200, SERIAL_8N1, SERIAL_TX_ONLY);
#else  // ESP8266
  Serial.begin(115200, SERIAL_8N1);
#endif  // ESP8266
}

uint16_t fetch_weather()
{
  if ((millis() - lastTime) > timerDelay) {
    // Check WiFi connection status
    if (WiFi.status() == WL_CONNECTED) {
      String serverPath = "http://api.openweathermap.org/data/2.5/weather?units=metric&q=" + city + "," + countryCode + "&APPID=" + openWeatherMapApiKey;

      jsonBuffer = httpGETRequest(serverPath.c_str());
      Serial.println(jsonBuffer);
      JSONVar myObject = JSON.parse(jsonBuffer);

      // JSON.typeof(jsonVar) can be used to get the type of the var
      if (JSON.typeof(myObject) == "undefined") {
        Serial.println("Parsing input failed!");
        //return;
      }
      Serial.print("JSON object = ");
      Serial.println(myObject);
      Serial.print("Temperature: ");
      Serial.println(myObject["main"]["temp"]);
      Serial.print("Pressure: ");
      Serial.println(myObject["main"]["pressure"]);
      Serial.print("Humidity: ");
      Serial.println(myObject["main"]["humidity"]);
      Serial.print("Wind Speed: ");
      Serial.println(myObject["wind"]["speed"]);
      if (int(myObject["main"]["temp"]) >= 30)
      {
        Serial.print("so fridggggging hot!\n");
        return (int(myObject["main"]["temp"]));
      }
    }
    else {
      Serial.println("WiFi Disconnected");
    }
    lastTime = millis();
  }
}

void loop() {
  if (fetch_weather()) {
    Serial.println("a rawData capture from IRrecv");
    irsend.sendRaw(powerOn, sizeof(powerOn) / sizeof(powerOn[0]), 38);  // Send a raw data capture at 38kHz.
    irsend.sendRaw(ModeCool, sizeof(ModeCool) / sizeof(ModeCool[0]), 38);
    //Serial.println("a Samsung A/C state from IRrecv");
    //irsend.sendSamsungAC(samsungState);
    //delay(2000);
    Serial.println("it's working!!!!");
    delay(10000);
  }
}
String httpGETRequest(const char* serverName) {
  HTTPClient http;

  // Your IP address with path or Domain name with URL path
  http.begin(serverName);

  // Send HTTP POST request
  int httpResponseCode = http.GET();

  String payload = "{}";

  if (httpResponseCode > 0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    payload = http.getString();
  }
  else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  // Free resources
  http.end();

  return payload;
}
