/*
 * LedCycle.cpp
 */

#include "Time.h"

#define DEFAULT_START_TIME 0
#define DEFAULT_FADE_IN_TIME 0
#define DEFAULT_STOP_TIME 0
#define DEFAULT_FADE_OUT_TIME 0

LedCycle::LedCycle (uint8_t _ledPin){
  ledPin = _ledPin;
  applyDefaultTime();
}

LedCycle::LedCycle (uint8_t _ledPin, time_t _startTime,
                    time_t _fadeInTime, time_t _stopTime,
                    time_t fadeOutTime){
  ledPin = _ledPin;
  startTime = _startTime;
  fadeInTime = _fadeInTime;
  stopTime = _stopTime;
  fadeOutTime = _fadeOutTime;
}

void LedCycle::applyDefaultTime (){
  startTime = DEFAULT_START_TIME;
  fadeInTime = DEFAULT_FADE_IN_TIME;
  stopTime = DEFAULT_STOP_TIME;
  fadeOutTime = DEFAULT_FADE_OUT_TIME;
}

void LedCycle::computeCycle (time_t _currentTime){

}

/*
 * End of file LedCycle.cpp
 */