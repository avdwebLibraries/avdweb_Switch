# Switch library
There is another lib distracted, who has time to check the differences and combine both libs to one again?

https://github.com/Martin-Laclaustra/avdweb_Switch

## About
Arduino library for debouncing switches and buttons. Supports detecting longPress and doubleClick. Supports defining callback functions.

Available at: https://github.com/avandalen/avdweb_Switch

#### General features of the Switch library
- Performs not just de-bouncing, but also de-glitching against EMC pulses.
- External pull-up resistors are not required.
- Supports also long press, double click, and single click detection.
- Callback functions.

## Introduction
Switches and push buttons need debouncing. It is straightforward to do this with software, using this library.

For a complete description see: https://www.avdweb.nl/arduino/hardware-interfacing/simple-switch-debouncer

## Using the Switch library

Declare each switch or button, by indicanting its GPIO number:

```
Switch buttonGND = Switch(4); // GPIO 4
```

The library sets GPIO in input mode with an internal pull-up resistor by default. The constructor accepts several arguments including the particular input mode (e.g. without pull-up), whether voltage level high must be interpreted as on or off (argument `polarity` must indicate the pushed voltage level), and the duration of the time periods that define the behaviour.

All switches have to be repeatedly polled individually to update the status. This is usually done in the `loop()` function.

```
buttonGND.poll();
```

After refreshing the status, it can be checked with get functions. For example:

```
if(buttonGND.pushed()) Serial.print("pushed");
```

Any event occurs based on the deglitched and debounced pin signal. See below for details on this function of the library that happens transparently.

Each possible event (each switch-status) has its own get function:

#### switched()

It returns "true" if the pin voltage level changed, no matter the direction (from low to high or from high to low).

#### on()

It returns "true" if the pin voltage agrees with the one defined by `polarity` (which is LOW by default). Use only for toggle switches. It returns "true" as long as the switch is in the "on" position. The polarity of the switch in the "on" position has to be filled in correctly. There is no off() function, this is simply !on().

#### pushed()

It returns "true" if a button was pushed (switched towards the on position). Use only for push buttons. Note that this provides instant response and can be used as "single-click" if "double-click" and "long-press" are not watched events. "Pushed" occurs simultaneously with "double-click" and "long-press".

#### released()

It returns "true" if a push button was released (switched towards the off position), this will however rarely be used.

#### longPress()

It returns "true" if a push button is pressed longer than 300ms (by default). Note that a longPress() always will be preceded by pushed() from the first push.

#### doubleClick()

It returns "true" if a push button is double clicked within 250ms (by default). Note that a doubleClick() always will be preceded by pushed() from the first push.

The anteceding pushed() event can't be avoided, because to restrict the pushed() function to single clicks it would have to wait for a possible second click, which would introduce an annoying delay. So, the action on doubleClick() has to undo the previous action on pushed().

#### singleClick()

It returns "true" if a push button is clicked once and the requirements for doubleClick and longPress are not met. The event thus occur several miliseconds after the actual push, because it depends on the other events not happening after the push. Use this if three different interactions are needed in combination with doubleClick and longPress. Note that a singleClick() always will be preceded by pushed(). Use pushed() instead if immediate response is required and there is no interest in monitoring doubleClick and longPress.


#### Example

See the example in the library for a complete working program.

### Transparent signal filtering
#### Deglitching
Sudden short changes in the microcontroler power supply may create inexistent switched() events if the pin is polled in that instant. Deglitching is achieved because the software will not believe that a voltage change has happened (a swithed event) until the voltage remains in the new level for a minimum period. A voltage change will be ignored if it returns to the original level before that period.

#### Debouncing
Due to the elastic properties of the metals in the contacts they bounce away from each other after the initial contact creating false switched() events after an initial true switch, until the voltage stabilizes in the new level. Debouncing forces a refractary period after an initial switch, during which any voltage level change is ignored.

### Using callback functions to be triggered by button events

Callbacks can be used, instead of needing to check with an `if` statement whether the relevant event has occurred after calling `switch.poll()`.

With a callback registered, Switch will run the defined function when the conditions are met at the time `poll()` is called. Calling `poll()` periodically is the only requirement.

First, define a function with the action you want to perform, with no return type (void) and one argument, a pointer to some reference (or use nullptr if you don't need it.): 

```C++
void foo(void* ref) {
  //your code;
}
``` 
... and then call one of the "set callback" methods in the `setup()` function, like so: 

```C++
void setup() {
  ... // Other setup code 
  ...
  int reference;
  switch.setPushedCallback(&foo, &reference);
  ...
}
``` 

The available callback setting functions are `setPushedCallback()`, `setReleasedCallback()`, `setLongPressCallback()`, `setDoubleClickCallback()`, and `setSingleClickCallback()` which allow defining the functions that will be called on such events. If using a toggle switch and not a push button, the "pushed" event will be of interest when the switch is turned on, and "released" when it is turned off.

If the conditions for more than one event occur simultaneously and there are callback functions registered for them, they will be executed in the order of the functions above.

See the example in the library for a complete working program.

### Notes

The poll instruction must be called at least 50 times per second, i.e. every 20ms; it takes about 20us in Arduino.

The button states are saved till the next poll. Then all previous button states will be cleared, so the button states must be read every poll interval, after calling the poll function.

Reading a button several times within a poll interval gives the same value, so reading doesn't clear the button state.

### SUGESTION: Using an interrupt service routine for polling the buttons

Polling buttons has a high priority and should be done often. Slow functions such as Serial.print() may disrupt the timing.

Polling could be done in a function triggered by a timed interrupt. Remember to keep the subsequent button actions short as you would do with any code in interrupts.

For Arduino see here how to use an ISR for polling the buttons:

```C++
#include <Arduino.h>
#include "avdweb_Switch.h"
#include <FrequencyTimer2.h>

Switch speedUpBtn(1);
Switch speedDownBtn(2);
Switch buttonLeft(3);
Switch buttonRight(4);

void setup(void)
{ Serial.begin(9600);
  FrequencyTimer2::setPeriod(1000);
  FrequencyTimer2::setOnOverflow(timer2ISR);
}

void loop(void)
{ printAll(); // run slow functions in loop()
}

void timer2ISR()
{ pollAll(); // polling buttons has priority
  buttonActions();
}

// you should implement pollAll(), buttonActions(), and printAll()
```

For ESP8266 use the [Ticker](https://arduino-esp8266.readthedocs.io/en/latest/libraries.html#ticker) library.


### How to use successive button events

With the Switch library, you can use all button events at the same time with the same button. For example pushed(), released(), doubleClick() and longPress(). To see an example of what can be achieved with several button events in order to perform different functions, run Windows Explorer: a single click selects a file and a double click opens it.

#### Button pushed event followed by a longPress event

A long-press generates first a pushed event and after 300ms (by default) the longPress event. This is not a shortcoming but a logical consequence. We can, of course, not always wait 300ms to see if a push might be a long push.

#### Button pushed event followed by a doubleClick event

The same happens with doubleClick, which also generates two pushed() events. When doubleClick is used, ignore the second pushed() result or don't call pushed(). When doubleClick is not needed, simply don't call doubleClick().

#### Combining singleClick, doubleClick, and longPress events

If these three events must be used toghether the best strategy is to stick to their get functions and avoid using pushed().

## Hardware considerations

#### Connecting switches to the microcontroler

There is nothing needed beyond the switches, just connect the switches between the ground and a digital pin.

#### Switch between GND and digital pin

This is technically speaking the best solution and it is taken by the library as default. An external pull-up resistor is not needed but allowed.

#### Switch between VCC(3.3V or 5V) and digital pin

For switches connected to the Arduino power supply, the settings are: polarity = HIGH and pinmode = INPUT, which disables the internal pull-up resistor. Note that we need external pull-down resistors of about 10k here.

## Questions

Post questions on the [Arduino board](http://forum.arduino.cc/index.php?topic=379308) and let me know, I will reply there.

Post issues on the github repository.

## Explanation of the inner working

### New: software deglitch

Normally, just debouncing is used. But debouncing doesn't make the software insensitive to noise pulses, which could lead to dangerous situations: if there is a poll() at the moment of a noise pulse, the software can detect an activated switch. To prevent this, a deglitch function is added now: during a "deglitchPeriod", all input values have to be equal.

### Software debounce, how does it work

The software debounce algorithm is based on the following assumptions; the 50ms (by default) is the so-called debounce period:

- The switch bounce time is less than 50ms.
- The time between successive keystrokes is larger than 50ms.
- A reaction time of 50ms is acceptable.

If the above assumptions are met, the software debounce algorithm can be quite simple: a switch event is only accepted when the elapsed time since the last switch event is larger than the debounce period.

A debounce period of 50ms is a safe value; it doesn't hurt the reaction time, and will handle even bad switches.

See the following pseudocode:

```C++
if((current_deglitched != previous_debounced) & ((current_ms - last_switchedTime) >= debouncePeriod)) current_debounced = current_deglitched;
// otherwise current_debounced stays the same as previous_debounced
```

### Overall functioning

Several discrete signals (updated at poll times) are created. The raw signal is deglitched, the later debounced, and that one in turn is processed to generate a signal for each one of the events.

### Timing diagram

```	
..........................................DEGLITCHING..............................

                        ________________   _
               on      |                | | |    _
                       |                | | |   | |
                       |                |_| |___| |__
 analog        off_____|_____________________________|____________________________

                        ________________   _     _
 input            _____|                |_| |___| |_______________________________

 poll            ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^

 equal           0 1 1 0 1 1 1 1 1 1 1 1 0 0 0 1 0 0 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1

 deglitchPeriod          <--------><--   <--     <-  <--------><--------><--------
                                    ___________________________
 deglitched       _________________|                           |__________________

 deglitchTime            ^         ^     ^       ^   ^         ^        ^

 ..........................................DEBOUNCING.............................

 debouncePeriod                    <-------------------------------->
                                    _________________________________
 debounced        _________________|                                 |____________
                                    _                                 _
 _switched        _________________| |_______________________________| |__________

 switchedTime                      ^                                 ^


**********************************************************************************
........................................DOUBLE CLICK..............................

                           __________         ______
 debounced        ________|          |_______|      |_____________________________

 poll            ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^
                           _                  _
 pushed          _________| |________________| |__________________________________

 pushedTime               ^                  ^
                                      _              _
 released        ____________________| |____________| |___________________________

 releasedTime                        ^              ^

 doubleClickPeriod         <------------------------------------->
                                              _
 _doubleClick     ___________________________| |__________________________________


........................................LONG PRESS................................

                           ___________________________
 debounced        ________|                           |___________________________

 poll            ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^

 longPressPeriod            <--------------->
                            _                           _
 _switched        _________| |_________________________| |________________________
                                              __________
 longPressDisable ___________________________|          |_________________________
                                              _
 _longPress       ___________________________| |__________________________________


........................................SINGLE CLICK..............................

                           __________                                 ______
 debounced        ________|          |_______________________________|      |_____

 poll            ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^

 longPressPeriod           <--------------->
 doubleClickPeriod         <------------------------------------->
                            _         _                               _      _
 _switched        _________| |_______| |_____________________________| |____| |___
                                                                  _____
 singleClickDisable______________________________________________|     |__________
                                                                  _
 _singleClick     _______________________________________________| |______________
```

### Additional information
See also: [A Guide To Debouncing](http://www.eng.utah.edu/~cs5780/debouncing.pdf)


### Licenses
#### Library

[![GNU License](https://www.gnu.org/graphics/gplv3-88x31.png)](https://www.gnu.org/licenses/gpl-3.0.en.html)

Copyright (C) 2012  Albert van Dalen

http://www.avdweb.nl

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
#### Documents

[![bt-nc-sa](http://i.creativecommons.org/l/by-nc-sa/3.0/88x31.png)](http://creativecommons.org/licenses/by-nc-sa/3.0/)

This document is licensed under a [Creative Commons Attribution-NonCommercial-ShareAlike 3.0 Unported License](http://creativecommons.org/licenses/by-nc-sa/3.0/).

