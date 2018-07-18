#include "Arduino.h"
#include "avdweb_Switch.h"

const byte toggleSwitchpin = 13; 
const byte buttonGNDpin = 4; 
const byte ButtonVCCpin = 2; 
const byte Button10mspin = 12; 
 
Switch buttonGND = Switch(buttonGNDpin); // button to GND, use internal 20K pullup resistor
Switch toggleSwitch = Switch(toggleSwitchpin); 
Switch buttonVCC = Switch(ButtonVCCpin, INPUT, HIGH); // button to VCC, 10k pull-down resistor, no internal pull-up resistor, HIGH polarity
Switch button10ms = Switch(Button10mspin, INPUT_PULLUP, LOW, 1); // debounceTime 1ms

void buttonCallbackFunction(void* s) {
  Serial.print("Button: ");
  Serial.println((char*)s);
}

void toggleCallbackFunction(void* s) {
  Serial.print("Toggle: ");
  Serial.println((char*)s);
}

void setup()
{ 
  Serial.begin(9600);
  buttonGND.setPushedCallback(&buttonCallbackFunction, (void*)"pushed");
  buttonGND.setReleasedCallback(&buttonCallbackFunction, (void*)"released");

  toggleSwitch.setLongPressCallback(&toggleCallbackFunction, (void*)"long press");
  toggleSwitch.setDoubleClickCallback(&toggleCallbackFunction, (void*)"double click");
}

void loop()
{ 
  buttonGND.poll();
  toggleSwitch.poll();
}
