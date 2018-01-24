#include "Arduino.h"
#include <Streaming.h> // https://github.com/kachok/arduino-libraries/blob/master/Streaming/Streaming.h
#include "Switch.h"
 
const byte toggleSwitchpin = 3; 
const byte buttonGNDpin = 4; 
const byte ButtonVCCpin = 6; 
const byte Button10mspin = 8; 
int i;
 
Switch buttonGND = Switch(buttonGNDpin); // button to GND, use internal 20K pullup resistor
Switch toggleSwitch = Switch(toggleSwitchpin); 
Switch buttonVCC = Switch(ButtonVCCpin, INPUT, HIGH); // button to VCC, 10k pull-down resistor, no internal pull-up resistor, HIGH polarity
Switch button10ms = Switch(Button10mspin, INPUT_PULLUP, LOW, 1); // debounceTime 1ms
 
void setup() 
{ Serial.begin(9600);  
}
 
void loop() 
{ buttonGND.poll();  
  if(buttonGND.switched()) Serial << "switched ";   
  if(buttonGND.pushed()) Serial << "pushed " << ++i << " ";
  if(buttonGND.released()) Serial << "released\n";
  
  if(toggleSwitch.poll()) Serial << toggleSwitch.on() << endl; 
  if(toggleSwitch.longPress()) Serial << "longPress1 ";
  if(toggleSwitch.longPress()) Serial << "longPress2\n";
  if(toggleSwitch.doubleClick()) Serial << "doubleClick1 ";
  if(toggleSwitch.doubleClick()) Serial << "doubleClick2\n";
}
