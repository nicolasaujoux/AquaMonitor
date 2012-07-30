/*
 * LedCycle.cpp
 */

#include "LedCycle.h"
#include "Arduino.h"

#define PWM_MAX_OUTPUT 255

LedCycle::LedCycle (){
  ledNeedInit = true;
  applyDefaultTime();
}

LedCycle::LedCycle (time_t _startTime,
                    time_t _fadeInTime, 
                    time_t _stopTime,
                    time_t _fadeOutTime){
  ledNeedInit = true;

  startTime = _startTime;
  fadeInTime = _fadeInTime;
  stopTime = _stopTime;
  fadeOutTime = _fadeOutTime;
}

void LedCycle::applyDefaultTime (){
  startTime = hoursToTime_t(16) + minutesToTime_t(0);
  fadeInTime = minutesToTime_t(30);
  stopTime = hoursToTime_t(22) + minutesToTime_t(30);
  fadeOutTime = minutesToTime_t(30);
}

uint8_t LedCycle::getOutputPercent (time_t currentTime){
  uint8_t brightnessPercent;

  brightnessPercent = getBrightnessPercentage(currentTime);
  return brightnessPercent;
}

uint8_t LedCycle::getBrightnessPercentage (time_t currentTime){
  uint8_t brightnessPercent;
  //We keep only the hour and minutes in the day
  time_t timeInDay;
  timeInDay = hoursToTime_t(hour(currentTime)) + 
      minutesToTime_t(minute(currentTime));

  if (ledNeedInit) {
    if (isLightOn(timeInDay) == true) {
      brightnessPercent = 100;
      isLedFullOn = true;
      isLedOff = false;
    }
    else {
      brightnessPercent = 0;
      isLedFullOn = false;
      isLedOff = true;
    }
    ledNeedInit = false;
  }

  // If we are fading in the led
  if ((isLightOn(timeInDay) == true) && (isLedFullOn == false)) {
    isLedOff = false;
    brightnessPercent = ((timeInDay - startTime) * 100) / fadeInTime;
      if (brightnessPercent == 100) {
        isLedFullOn = true;
      }
  }
  // If we are fading out the led
  else if ((isLightOn(timeInDay + fadeOutTime) == false) &&
           (isLedOff == false)) {
    isLedFullOn = false;
    brightnessPercent = ((timeInDay - (stopTime - fadeOutTime))
                         * 100 / fadeOutTime);
    if (brightnessPercent == 0) {
      isLedOff = true;
    }
  }
  // Else we do noting
  return brightnessPercent;
}

bool LedCycle::isLightOn (time_t currentTimeInDay){
  if ((currentTimeInDay >= startTime) && (currentTimeInDay < stopTime))
    return true;
  return false;
}

// For every setters, we make sure the time_t variable
// only contains hour and minute.
void LedCycle::setStartTime (time_t _startTime){
  startTime = hoursToTime_t(hour(_startTime)) +
      minutesToTime_t(minute(_startTime));
}

void LedCycle::setStopTime (time_t _stopTime){
  stopTime = hoursToTime_t(hour(_stopTime)) +
      minutesToTime_t(minute(_stopTime));
}

void LedCycle::setFadeInTime (time_t _fadeInTime){
  fadeInTime = hoursToTime_t(hour(_fadeInTime)) +
      minutesToTime_t(minute(_fadeInTime));
}

void LedCycle::setFadeOutTime (time_t _fadeOutTime){
  fadeOutTime = hoursToTime_t(hour(_fadeOutTime)) +
      minutesToTime_t(minute(_fadeOutTime));
}

/*
 * End of file LedCycle.cpp
 */