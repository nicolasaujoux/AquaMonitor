// Date and time functions using a DS1307 RTC connected via I2C and Wire lib
#include <Arduino.h>

#include <Wire.h>
#include "RTClib.h"

int ledPin = 11;
int brightness = 0;
bool fadeIN = false, fadeOUT = false;
 
RTC_DS1307 RTC;
 
void setup () {
  //Led setup
  pinMode(ledPin, OUTPUT);

  //RTC setup
  Serial.begin(57600);
  Wire.begin();
  RTC.begin();
 
  if (! RTC.isrunning()) {
    Serial.println("RTC is NOT running!");
    // following line sets the RTC to the date & time this sketch was compiled
    RTC.adjust(DateTime(__DATE__, __TIME__));
  }
 
}
 
void loop () {
  DateTime now = RTC.now();

  if (now.hour()>=16 && now.hour()<21) {
    fadeIN = true;
  }
  if (now.hour()==22 && now.minute()==30) {
    fadeOUT = true;
  }

  if (fadeIN && (brightness < 255)) {
    brightness++;
    if (brightness == 255) {
      fadeIN = false;
    }
  }
  if (fadeOUT && (brightness > 0)) {
    brightness--;
    if (brightness == 0) {
      fadeOUT = false;
    }
  }

  analogWrite(ledPin, brightness);

  delay(5000);
}
