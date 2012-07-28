/*
 * LedCycle.h
 */

#ifndef LED_CYCLE_H
#define LED_CYCLE_H

#include "Time.h"

class LedCycle {
public:
  LedCycle (uint8_t _ledPin);

  LedCycle (uint8_t _ledPin, 
            time_t startTime,
            time_t fadeInTime,
            time_t stopTime,
            time_t fadeOutTime);

  void computeCycle (time_t _currentTime);

  void setStartTime (time_t _startTime);
  time_t getStartTime() const {return startTime;}

  void setFadeInTime (time_t _fadeInTime);
  time_t getFadeInTime() const {return fadeInTime;}
  
  void setStopTime (time_t _stopTime);
  time_t getStopTime() const {return stopTime;}

  void setFadeOutTime (time_t _fadeOutTime);
  time_t getFadeOutTime() const {return fadeOutTime;}

  void applyDefaultTime ();

protected:
  uint8_t ledPin;

  time_t startTime, fadeInTime, stopTime, fadeOutTime;

  bool isLedFullOn, isLedOff;

  uint8_t getBrightnessPercentage (time_t currentTime);
  bool isLightOn (time_t currentTimeInDay);
};

#endif // LED_CYCLE_H

/*
 * End of file LedCycle.h
 */