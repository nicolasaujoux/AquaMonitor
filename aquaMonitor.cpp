
#include <Arduino.h>

#include "Time.h"
#include "DS1307RTC.h"

#include "Wire.h"
#include "LiquidCrystal_I2C.h"
#include "i2ckeypad.h"

#include "LedCycle.h"

void digitalClockDisplay();

LiquidCrystal_I2C lcd(0x38, 20, 4);
LedCycle led;
int ledPin = 11;

i2ckeypad kpd(0x39, 4, 3);

time_t time;

void setup()  {
  time_t start, fadeIn, stop, fadeOut;

  Wire.begin();

  setSyncProvider(RTC.get);

  lcd.init();
  lcd.noBacklight();

  start = hoursToTime_t(15) + minutesToTime_t(30);
  fadeIn = minutesToTime_t(30);
  stop = hoursToTime_t(23);
  fadeOut = minutesToTime_t(30);

  led.setStartTime(start);
  led.setFadeInTime(fadeIn);
  led.setStopTime(stop);
  led.setFadeOutTime(fadeOut);

  pinMode(ledPin, OUTPUT);

  kpd.init();

}

void loop(){
  char key;
  uint8_t percent;

  percent = led.getOutputPercent(now());
  analogWrite(ledPin, 255 * percent / 100);

  digitalClockDisplay();  
  lcd.setCursor(0,1);
  lcd.print(percent);

  key = kpd.get_key();
  if (key != '\0') {
    lcd.setCursor(0,2);
    lcd.print(key);
  }
}

void digitalClockDisplay(){
  // digital clock display of the time
  lcd.setCursor(0,0);
  lcd.print(hour());
  lcd.print(":");
  lcd.print(minute());
  lcd.print(":");
  lcd.print(second());
}

