
#include <Arduino.h>

// Time.h was modified from the original library
#include "Time.h"
#include "DS1307RTC.h"

#include "Wire.h"
#include "LiquidCrystal_I2C.h"
#include "FiniteStateMachine.h"

#include "i2ckeypad.h"

#include "LedCycle.h"

#include "QueueList.h"

#include <EEPROM.h>

/***********************
 * Function declaration
 ***********************/

void printWaitScreen();
void digitalClockDisplay(time_t time);
void printDigits(int digits, char separator);
void computeKeypadInput(char key);
void initMenuItems();
time_t computeUserInput(uint8_t index, uint8_t key);

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
void emptyState();
void waitState();
void menuState();
void changeHourState();
void changeStartLedState();
void changeStopLedState();
void changeFadeInState();
void changeFadeOutState();

State _emptyState = State(emptyState);
State _waitState = State(waitState);
// We have to add NULL for update and exit functions of the state
// or the fsm will continually call menuState when in this state
State _menuState = State(menuState, NULL, NULL);
State _changeHourState = State(changeHourState, NULL, NULL);
State _changeStartLedState = State(changeStartLedState, NULL, NULL);
State _changeStopLedState = State(changeStopLedState, NULL, NULL);
State _changeFadeInState = State(changeFadeInState, NULL, NULL);
State _changeFadeOutState = State(changeFadeOutState, NULL, NULL);

FSM fsm = FSM(_waitState);

// Menu declaration
typedef struct menuItem {
  uint8_t id;
  String message;
}menuItem;
QueueList <menuItem> menuItems;

/*********************
 * Global definition
 *********************/

#define NUMBER_OF_MENU_LINES 3

/*********************
 * Setup function
 * *******************/
void setup()  {
  time_t start, fadeIn, stop, fadeOut;

  Wire.begin();

  setSyncProvider(RTC.get);

  lcd.init();
  lcd.noBacklight();

  led.loadEepromDatas();

  pinMode(ledPin, OUTPUT);

  kpd.init();

  initMenuItems();
}

/********************
 * Loop function
 * ******************/
void loop(){
  char key;

  percent = led.getOutputPercent(now());
  analogWrite(ledPin, 255 * percent / 100);

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

void emptyState()
{
}

void waitState()
{
  printWaitScreen();
}

void menuState()
{
  menuItem tempItem;

  for (int i = 0; i < NUMBER_OF_MENU_LINES; i++) {
    tempItem = menuItems.pop();
    if (tempItem.message != NULL) {
      lcd.setCursor(0,i);
      lcd.print(i+1);
      lcd.print(tempItem.message);
    }
    menuItems.push(tempItem);
  }

  lcd.setCursor(0,3);
  if (menuItems.count() > NUMBER_OF_MENU_LINES) {
    lcd.print("Next : * Back : 0");
  }
  else{
    lcd.print("Back : #");
  }
}

void changeHourState()
{
  lcd.setCursor(0,0);
  lcd.print("Type the time :");
  lcd.setCursor(0,1);
  digitalClockDisplay(now());
  lcd.setCursor(0,2);
  lcd.print("^");
  lcd.setCursor(0,3);
  lcd.print("Back : #");
}

void changeStartLedState()
{
  lcd.setCursor(0,0);
  lcd.print("Start light time :");
  lcd.setCursor(0,1);
  digitalClockDisplay(led.getStartTime());
  lcd.setCursor(0,2);
  lcd.print("^");
  lcd.setCursor(0,3);
  lcd.print("Back : #");
}

void changeStopLedState()
{
  lcd.setCursor(0,0);
  lcd.print("Stop light time :");
  lcd.setCursor(0,1);
  digitalClockDisplay(led.getStopTime());
  lcd.setCursor(0,2);
  lcd.print("^");
  lcd.setCursor(0,3);
  lcd.print("Back : #");
}

void changeFadeInState()
{
  lcd.setCursor(0,0);
  lcd.print("Fade in light time :");
  lcd.setCursor(0,1);
  digitalClockDisplay(led.getFadeInTime());
  lcd.setCursor(0,2);
  lcd.print("^");
  lcd.setCursor(0,3);
  lcd.print("Back : #");
}

void changeFadeOutState()
{
  lcd.setCursor(0,0);
  lcd.print("Fade out light time :");
  lcd.setCursor(0,1);
  digitalClockDisplay(led.getFadeOutTime());
  lcd.setCursor(0,2);
  lcd.print("^");
  lcd.setCursor(0,3);
  lcd.print("Back : #");
}

/*********************
 * Internal functions
 * *******************/

void printWaitScreen()
{
  lcd.setCursor(0,0);
  digitalClockDisplay(now());

  lcd.setCursor(0,1);
  lcd.print("Output : ");
  lcd.print(percent);
  lcd.print("%");

  lcd.setCursor(0,3);
  lcd.print("Menu : #");
}

void computeKeypadInput(char key)
{
  static uint8_t index = 0;
  static time_t modifiedTime;
  if (fsm.isInState(_waitState))
  {
    switch (key)
    {
      case '#':
        lcd.clear();
        fsm.transitionTo(_menuState);
        break;
    }
  }
  if (fsm.isInState(_menuState))
  {
    switch (key)
    {
      case '#':
        lcd.clear();
        fsm.transitionTo(_waitState);
        break;
      case '*':
        lcd.clear();
        // We have to call an empty state or the menuState function
        // will not be called
        fsm.immediateTransitionTo(_emptyState);
        fsm.transitionTo(_menuState);
        break;
      case '1':
        lcd.clear();
        fsm.transitionTo(_changeHourState);
        break;
      case '2':
        lcd.clear();
        fsm.transitionTo(_changeStartLedState);
        break;
      case '3':
        lcd.clear();
        fsm.transitionTo(_changeStopLedState);
        break;
    }
  }
  if (fsm.isInState(_changeHourState) or 
      fsm.isInState(_changeStartLedState) or
      fsm.isInState(_changeStopLedState) or
      fsm.isInState(_changeFadeInState) or
      fsm.isInState(_changeFadeOutState))
  {
    switch (key)
    {
      case '*':
        if (index != 6) {
          break;
        }
        index = 0;
        lcd.clear();
        if (fsm.isInState(_changeHourState)) {
          RTC.set(modifiedTime);
          setSyncProvider(RTC.get);
          fsm.transitionTo(_waitState);
        }
        else if (fsm.isInState(_changeStartLedState)) {
          led.setStartTime(modifiedTime);
          led.saveEepromDatas();
          fsm.transitionTo(_changeFadeInState);
        }
        else if (fsm.isInState(_changeStopLedState)) {
          led.setStopTime(modifiedTime);
          led.saveEepromDatas();
          fsm.transitionTo(_changeFadeOutState);
        }
        else if (fsm.isInState(_changeFadeInState)) {
          led.setFadeInTime(modifiedTime);
          led.saveEepromDatas();
          fsm.transitionTo(_waitState);
        }
        else if (fsm.isInState(_changeFadeOutState)) {
          led.setFadeOutTime(modifiedTime);
          led.saveEepromDatas();
          fsm.transitionTo(_waitState);
        }
        break;
      case '#':
        index = 0;
        lcd.clear();
        fsm.transitionTo(_waitState);
        break;
      default:
        modifiedTime = computeUserInput(index, atoi(&key));
        index++;
        break;
    }
  }
}

void digitalClockDisplay(time_t time)
{
  // digital clock display of the time
  printDigits(hour(time), NULL);
  printDigits(minute(time), ':');
  printDigits(second(time), ':');
}

void printDigits(int digits, char separator)
{
  if (separator != NULL)
    lcd.print(separator);
  if(digits < 10)
    lcd.print('0');
  lcd.print(digits);
}

void initMenuItems()
{
  menuItem changeHour = {1, "Change hour"};
  menuItems.push(changeHour);
  menuItem test = {2, "Start LED time"};
  menuItems.push(test);
  menuItem test2 = {3, "Stop LED time"};
  menuItems.push(test2);

  // Complete the queue with NULL element to get a modulo
  // of the line number
  menuItem nullItem = {0, NULL};
  for (int i = 0; i < menuItems.count()%NUMBER_OF_MENU_LINES; i++) {
    menuItems.push(nullItem);
  }
}

time_t computeUserInput(uint8_t index, uint8_t number)
{
  static time_t time;
  uint8_t numberPosition = index;
  uint8_t nextPosition = index + 1;

  switch (index)
  {
    case 0:
      if (number > 2)
        number = 0;
      time = 0;
      time = hoursToTime_t(number * 10);
      break;
    case 1:
      nextPosition = 3;
      if (hour(time) >= 20 && number > 3)
        number = 0;
      time += hoursToTime_t(number);
      break;
    case 2:
      numberPosition = 3;
      nextPosition = 4;
      if (number > 5)
        number = 0;
      time += minutesToTime_t(number * 10);
      break;
    case 3:
      numberPosition = 4;
      nextPosition = 6;
      time += minutesToTime_t(number);
      break;
    case 4:
      numberPosition = 6;
      nextPosition = 7;
      if (number > 5)
        number = 0;
      time += number * 10;
      break;
    case 5:
      time += number;
      numberPosition = 7;
      nextPosition = 0;
      lcd.setCursor(10,3);
      lcd.print("OK : *");
      break;
    default:
      return time;
  }

  lcd.setCursor(numberPosition,1);
  lcd.print(number);
  lcd.setCursor(numberPosition,2);
  lcd.print(" ");
  lcd.setCursor(nextPosition,2);
  lcd.print("^");

  return time;
}
