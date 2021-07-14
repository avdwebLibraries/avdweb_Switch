/*
Switch.cpp
Copyright (C) 2012  Albert van Dalen http://www.avdweb.nl
This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License
as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty
of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License at http://www.gnu.org/licenses .

AUTHOR: Albert van Dalen
WEBSITE: http://www.avdweb.nl/arduino/hardware-interfacing/simple-switch-debouncer.html
*/

#ifndef AVDWEB_SWITCH_H
#define AVDWEB_SWITCH_H

typedef void (*switchCallback_t)(void*); 

class Switch
{
public:	
  Switch(byte _pin, byte PinMode=INPUT_PULLUP, bool polarity=LOW, unsigned long debouncePeriod=50, unsigned long longPressPeriod=300, unsigned long doubleClickPeriod=250, unsigned long deglitchPeriod=10);
  bool poll(); // Returns 1 if switched
  bool switched(); // will be refreshed by poll()
  bool on();
  bool pushed(); // will be refreshed by poll()
  bool released(); // will be refreshed by poll()
  bool longPress(); // will be refreshed by poll()
  bool doubleClick(); // will be refreshed by poll()
  bool singleClick(); // will be refreshed by poll()

  // Set methods for event callbacks
  void setPushedCallback(switchCallback_t cb, void* param = nullptr);
  void setReleasedCallback(switchCallback_t cb, void* param = nullptr);
  void setLongPressCallback(switchCallback_t cb, void* param = nullptr);
  void setDoubleClickCallback(switchCallback_t cb, void* param = nullptr);
  void setSingleClickCallback(switchCallback_t cb, void* param = nullptr);
  void setBeepAllCallback(switchCallback_t cb, void* param = nullptr); 

  unsigned long deglitchPeriod, debouncePeriod, longPressPeriod, doubleClickPeriod;

  protected:
  bool process(); // not inline, used in child class
  void inline deglitch();
  void inline debounce();
  void inline calcLongPress();
  void inline calcDoubleClick();
  void inline calcSingleClick();
  void triggerCallbacks();

  unsigned long deglitchTime, switchedTime, pushedTime, releasedTime, ms;
  const byte pin;
  const bool polarity;
  bool input, lastInput, equal, deglitched, debounced, _switched, _longPress, longPressDisable, _doubleClick, _singleClick, singleClickDisable;

  // Event callbacks
  switchCallback_t _pushedCallback = nullptr;
  switchCallback_t _releasedCallback = nullptr;
  switchCallback_t _longPressCallback = nullptr;
  switchCallback_t _doubleClickCallback = nullptr;
  switchCallback_t _singleClickCallback = nullptr;
  static switchCallback_t _beepAllCallback; // static function pointer, can be used by all objects 
  //static switchCallback_t _beepAllCallback = nullptr; // gives error with SAMD21
  //static void(*_beepAllCallback)(void*) = nullptr; // static function pointer without typedef
  
  void* _pushedCallbackParam = nullptr;
  void* _releasedCallbackParam = nullptr;
  void* _longPressCallbackParam = nullptr;
  void* _doubleClickCallbackParam = nullptr;
  void* _singleClickCallbackParam = nullptr;
  static void* _beepAllCallbackParam; // can be used by all objects
  //static void* _beepAllCallbackParam = nullptr; // gives error with SAMD21
};
#endif
