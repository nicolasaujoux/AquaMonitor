/*
 * LedCycle.cpp
 */

#include "LedCycle.h"
#include "Arduino.h"
#include "EEPROM.h"

#define PWM_MAX_OUTPUT 255
#define EEPROM_MAX_ADDRESS 512

LedCycle::LedCycle()
{
  applyDefaultTime();

  eepromAddress = 0;

  // We consider the light is off when we start
  isLedFullOn = false;
  isLedOff = true;
}

LedCycle::LedCycle(time_t _startTime,
                    time_t _fadeInTime, 
                    time_t _stopTime,
                    time_t _fadeOutTime)
{
  startTime = _startTime;
  fadeInTime = _fadeInTime;
  stopTime = _stopTime;
  fadeOutTime = _fadeOutTime;

  eepromAddress = 0;

  // We consider the light is off when we start
  isLedFullOn = false;
  isLedOff = true;
}

void LedCycle::applyDefaultTime()
{
  startTime = hoursToTime_t(15) + minutesToTime_t(30);
  fadeInTime = minutesToTime_t(30);
  stopTime = hoursToTime_t(23) + minutesToTime_t(0);
  fadeOutTime = minutesToTime_t(30);
}

uint8_t LedCycle::getOutputPercent(time_t currentTime)
{
  double brightnessPercent;
  time_t timeInDay;
  timeInDay = hoursToTime_t(hour(currentTime)) + 
      minutesToTime_t(minute(currentTime)) + 
      second(currentTime);

  // If we are fading in the led
  if ((timeInDay >= startTime) && (timeInDay <= (startTime + fadeInTime))) {
    brightnessPercent = ((timeInDay - startTime) * 100) / (fadeInTime);
    if (brightnessPercent >= 100) {
        brightnessPercent = 100;
    }
    return (uint8_t)brightnessPercent;
  }
  // If we are fading out the led
  if ((timeInDay >= (stopTime - fadeOutTime)) && (timeInDay <= stopTime)) {
    brightnessPercent = 100 - (((timeInDay - (stopTime - fadeOutTime))
                         * 100 / fadeOutTime));
    if (brightnessPercent <= 0) {
      brightnessPercent = 0;
    }
    return (uint8_t)brightnessPercent;
  }
  // If it is day time
  if ((timeInDay >= startTime) && (timeInDay < stopTime)) {
    brightnessPercent = 100;
    return (uint8_t)brightnessPercent;
  }
  // If it is night time
  brightnessPercent = 0;
  return (uint8_t)brightnessPercent;
}

// For every setters, we make sure the time_t variable
// only contains hour and minute.
void LedCycle::setStartTime(time_t _startTime)
{
  startTime = hoursToTime_t(hour(_startTime)) +
      minutesToTime_t(minute(_startTime));
}

void LedCycle::setStopTime (time_t _stopTime)
{
  stopTime = hoursToTime_t(hour(_stopTime)) +
      minutesToTime_t(minute(_stopTime));
}

void LedCycle::setFadeInTime(time_t _fadeInTime)
{
  fadeInTime = hoursToTime_t(hour(_fadeInTime)) +
      minutesToTime_t(minute(_fadeInTime));
}

void LedCycle::setFadeOutTime(time_t _fadeOutTime)
{
  fadeOutTime = hoursToTime_t(hour(_fadeOutTime)) +
      minutesToTime_t(minute(_fadeOutTime));
}

bool LedCycle::setEepromStartAddress(uint8_t _address)
{
  if ((_address + 4*2) > EEPROM_MAX_ADDRESS) {
    eepromAddress = 0;
    return false;
  }
  eepromAddress = _address;
  return true;
}

bool LedCycle::loadEepromDatas()
{
  uint8_t tempValue;
  time_t tempTime;
  uint8_t readValues[8];

  for (int i = 0; i < 8; i++) {
    tempValue = EEPROM.read(eepromAddress + i);
    if (tempValue > 60) {
      applyDefaultTime();
      return false;
    }
    readValues[i] = tempValue;
  }

  setStartTime(hoursToTime_t(readValues[0]) + 
               minutesToTime_t(readValues[1]));
  setFadeInTime(hoursToTime_t(readValues[2]) + 
               minutesToTime_t(readValues[3]));
  setStopTime(hoursToTime_t(readValues[4]) + 
               minutesToTime_t(readValues[5]));
  setFadeOutTime(hoursToTime_t(readValues[6]) + 
               minutesToTime_t(readValues[7]));
  return true;
}

void LedCycle::saveEepromDatas()
{
  EEPROM.write(eepromAddress + 0, (uint8_t)hour(startTime));
  EEPROM.write(eepromAddress + 1, (uint8_t)minute(startTime));

  EEPROM.write(eepromAddress + 2, (uint8_t)hour(fadeInTime));
  EEPROM.write(eepromAddress + 3, (uint8_t)minute(fadeInTime));

  EEPROM.write(eepromAddress + 4, (uint8_t)hour(stopTime));
  EEPROM.write(eepromAddress + 5, (uint8_t)minute(stopTime));

  EEPROM.write(eepromAddress + 6, (uint8_t)hour(fadeOutTime));
  EEPROM.write(eepromAddress + 7, (uint8_t)minute(fadeOutTime));
}

/*
 * End of file LedCycle.cpp
 */