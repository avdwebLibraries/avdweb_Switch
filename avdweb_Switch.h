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

class Switch
{
public:	
  Switch(byte _pin, byte PinMode=INPUT_PULLUP, bool polarity=LOW, int debouncePeriod=50, int longPressPeriod=300, int doubleClickPeriod=250, int deglitchPeriod=10);
  bool poll(); // Returns 1 if switched   
  bool switched(); // will be refreshed by poll()
  bool on(); 
  bool pushed(); // will be refreshed by poll()
  bool released(); // will be refreshed by poll()
  bool longPress(); // will be refreshed by poll()
  bool doubleClick(); // will be refreshed by poll()

  protected: 
  bool process(); // not inline, used in child class
  void inline deglitch();
  void inline debounce();
  void inline calcDoubleClick();
  void inline calcLongPress();

  unsigned long deglitchTime, switchedTime, pushedTime, ms;
  const byte pin; 
  const int deglitchPeriod, debouncePeriod, longPressPeriod, doubleClickPeriod;
  const bool polarity;
  bool input, lastInput, equal, deglitched, debounced, _switched, _longPress, longPressDisable, _doubleClick; 
};

#endif


