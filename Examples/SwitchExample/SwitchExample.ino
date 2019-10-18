#include "Arduino.h"
#include "avdweb_Switch.h"

const byte pushButtonpin = 10; // downButton 
const byte toggleSwitchpin = 9; // upButton
const byte multiresponseButtonpin = 8; // selectButton
const byte alleventsButtonpin = 7; // weldButton
const byte buzzerPin = 3; // for beep when a switch is pressed

int i;
Switch pushButton = Switch(pushButtonpin); // button to GND, use internal 20K pullup resistor
Switch toggleSwitch = Switch(toggleSwitchpin);
Switch multiresponseButton = Switch(multiresponseButtonpin);
Switch alleventsButton = Switch(alleventsButtonpin);
// Other examples of constructors
// Switch pushButtonVCC = Switch(pushButtonpin, INPUT, HIGH); // button to VCC, 10k pull-down resistor, no internal pull-up resistor, HIGH polarity
// Switch pushButton1ms = Switch(pushButtonpin, INPUT_PULLUP, LOW, 1); // debounceTime 1ms

void beepCallbackFunction(void* s) // optional
{ tone(3, 2400, 5); // is non-blocking 
  //Serial.print("BeepCallback: "); Serial.println((char*)s);
}

void setup()
{ Serial.begin(9600);
  toggleSwitch.setBeepStaticCallback(&beepCallbackFunction/*, "Beep done"*/); // needed only for one switch because of static 
}

void loop()
{ // pushButton simple events
  pushButton.poll();
  if(pushButton.switched()) Serial.print("pushButton switched ");
  if(pushButton.pushed()) {Serial.print("pushButton pushed "); Serial.print(++i); Serial.println(" times");}
  if(pushButton.released()) Serial.println("pushButton released");

  // toggleSwitch report status only when changed
  if(toggleSwitch.poll()) {Serial.print("toggleSwitch status changed to "); Serial.println(toggleSwitch.on());}

  // multiresponseButton complex events
  multiresponseButton.poll();
  if(multiresponseButton.longPress()) Serial.println("multiresponseButton longPress");
  if(multiresponseButton.doubleClick()) Serial.println("multiresponseButton doubleClick");
  if(multiresponseButton.singleClick()) Serial.println("multiresponseButton singleClick");

  // alleventsButton complex events
  alleventsButton.poll();
  if(alleventsButton.switched()) {Serial.println("all_e_B switched."); Serial.print(" all_e_B status to "); Serial.print(alleventsButton.on()); Serial.println(".");}
  if(alleventsButton.pushed()) {Serial.println(" all_e_B pushed.");}
  if(alleventsButton.released()) Serial.println(" all_e_B released.");
  if(alleventsButton.longPress()) Serial.println("  ==> all_e_B longPress.");
  if(alleventsButton.doubleClick()) Serial.println("  ==> all_e_B doubleClick.");
  if(alleventsButton.singleClick()) Serial.println("  ==> all_e_B singleClick.");
}
