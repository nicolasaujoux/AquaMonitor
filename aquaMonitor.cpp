
#include <Arduino.h>

#include "Time.h"
#include "DS1307RTC.h"

#include "Wire.h"
#include "LiquidCrystal_I2C.h"

#include "LedCycle.h"

void digitalClockDisplay();

LiquidCrystal_I2C lcd(0x38, 16, 2);
LedCycle led();

time_t time;

void setup()  {
  time_t start, fadeIn, stop, fadeOut;

  setTime(16,0,0,0,0,0);
  //setSyncProvider(RTC.get);

  lcd.init();
  lcd.backlight();

  start = hoursToTime_t(15) + minutesToTime_t(30);
  fadeIn = minutesToTime_t(1);
  stop = hoursToTime_t(23);
  fadeOut = minutesToTime_t(1);

  led.setStartTime(start);
  led.setFadeInTime(fadeIn);
  led.setStopTime(stop);
  led.setFadeOutTime(fadeOut);
}

void loop(){
  uint8_t percent;
  percent = led.getOutputPercent(now());

  digitalClockDisplay();  
  lcd.setCursor(0,1);
  lcd.print(percent);
  delay(1000);
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

