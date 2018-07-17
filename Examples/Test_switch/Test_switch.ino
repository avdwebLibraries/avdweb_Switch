#include "Arduino.h"
#include <Streaming.h> // https://github.com/kachok/arduino-libraries/blob/master/Streaming/Streaming.h
#include "avdweb_Switch.h"
 
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
  if(buttonGND.switched()) Serial.print("switched ");
  if(buttonGND.pushed()) {Serial.print("pushed "); Serial.print(++i); Serial.print(" ");}
  if(buttonGND.released()) Serial.println("released");

  if(toggleSwitch.poll()) Serial.println(toggleSwitch.on());
  if(toggleSwitch.longPress()) Serial.print("longPress1 ");
  if(toggleSwitch.longPress()) Serial.println("longPress2");
  if(toggleSwitch.doubleClick()) Serial.print("doubleClick1 ");
  if(toggleSwitch.doubleClick()) Serial.println("doubleClick2");
}
