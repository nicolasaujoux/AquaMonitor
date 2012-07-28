
#include <Arduino.h>

#include "Time.h"

#include "Wire.h"
#include "LiquidCrystal_I2C.h"

void digitalClockDisplay();

LiquidCrystal_I2C lcd(0x38, 16, 2);

time_t time;

void setup()  {
  lcd.init();
  lcd.backlight();
  setTime(18,0,0,0,0,0);

  time = hoursToTime_t(16) + minutesToTime_t(30);

}

void loop(){
  digitalClockDisplay();  
  lcd.setCursor(0,1);

  lcd.print((hour(time)));
  lcd.print(":");
  lcd.print(minute(time));

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

