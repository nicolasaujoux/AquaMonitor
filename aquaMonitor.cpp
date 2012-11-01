
#include <Arduino.h>

// Time.h was modified from the original library
#include "Time.h"
#include "DS1307RTC.h"

#include "Wire.h"
#include "LiquidCrystal_I2C.h"
#include "FiniteStateMachine.h"

#include "i2ckeypad.h"

#include "LedCycle.h"

/***********************
 * Function declaration
 ***********************/

void printWaitScreen();
void digitalClockDisplay();
void printDigits(int digits);
void computeKeypadInput(char key);

/***********************
 * Internal variables
 * *********************/

// LCD declaration
LiquidCrystal_I2C lcd(0x38, 20, 4);

// Led declaration
LedCycle led;
uint8_t percent;
int ledPin = 11;

// Keypad declaration
i2ckeypad kpd(0x39, 4, 3);

// Time declaration
time_t time;

// Finite State Machine declaration
void waitState();
void menuState();
void menuUpdate();

State _waitState = State(waitState);
State _menuState = State(menuState, menuUpdate, NULL);

FSM fsm = FSM(_waitState);

/*********************
 * Setup function
 * *******************/
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

/********************
 * Loop function
 * ******************/
void loop(){
  char key;

  percent = led.getOutputPercent(now());
  analogWrite(ledPin, 255 * percent / 100);

  if (fsm.isInState(_waitState))
  {
    printWaitScreen();
  }

  key = kpd.get_key();
  if (key != '\0')
  {
    computeKeypadInput(key);
  }
  fsm.update();
}

/*********************
 * State Functions
 *********************/

void waitState()
{
}

void menuState()
{
  lcd.setCursor(0,0);
  lcd.print(percent);

  lcd.setCursor(0,3);
  lcd.print("Next : * Back : 0");
}

void menuUpdate()
{

  lcd.setCursor(0,3);
  lcd.print("Next : * Back : 0");
}

/*********************
 * Internal functions
 * *******************/

void printWaitScreen()
{
  digitalClockDisplay();

  lcd.setCursor(0,1);
  lcd.print("Output : ");
  lcd.print(percent);
  lcd.print("%");

  lcd.setCursor(0,3);
  lcd.print("Menu : #");
}

void computeKeypadInput(char key)
{
  switch (key)
  {
    case '#':
      lcd.clear();
      fsm.transitionTo(_menuState);
      break;
    case '0':
      lcd.clear();
      fsm.transitionTo(_waitState);
      break;
    case '*':
      if (fsm.isInState(_menuState))
      {
        lcd.clear();
        fsm.update();
      }
      break;
  }
}

void digitalClockDisplay()
{
  // digital clock display of the time
  lcd.setCursor(0,0);
  lcd.print(hour());
  printDigits(minute());
  printDigits(second());
}

void printDigits(int digits)
{
  lcd.print(":");
  if(digits < 10)
    lcd.print('0');
  lcd.print(digits);
}
