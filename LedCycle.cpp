/*
 * LedCycle.cpp
 */

#include "LedCycle.h"
#include "Arduino.h"

#define PWM_MAX_OUTPUT 255

LedCycle::LedCycle (){
  applyDefaultTime();

  // We consider the light is off when we start
  isLedFullOn = false;
  isLedOff = true;
}

LedCycle::LedCycle (time_t _startTime,
                    time_t _fadeInTime, 
                    time_t _stopTime,
                    time_t _fadeOutTime){

  startTime = _startTime;
  fadeInTime = _fadeInTime;
  stopTime = _stopTime;
  fadeOutTime = _fadeOutTime;

  // We consider the light is off when we start
  isLedFullOn = false;
  isLedOff = true;
}

void LedCycle::applyDefaultTime (){
  startTime = hoursToTime_t(16) + minutesToTime_t(0);
  fadeInTime = minutesToTime_t(30);
  stopTime = hoursToTime_t(22) + minutesToTime_t(30);
  fadeOutTime = minutesToTime_t(30);
}

uint8_t LedCycle::getOutputPercent (time_t currentTime){
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