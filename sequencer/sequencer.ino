#include <MIDI.h>
MIDI_CREATE_DEFAULT_INSTANCE();

//number of total leds
const int ledCount = 1;
 
//number of pots controlling note values
const int potCount = 4;
 
//define pot for tempo
const int tempoPin = A5;
 
//define pot for gate
const int gatePin = A4;
 
//number of steps
const int length = 4;
 
//specify which note
const byte notes[32] = {43,45,48,50,51,52,54,55,57,60,62,63,64,66,67,69,72,74,75,76,78,79,81,84,86,87,88,90,91,93,96,98};
//const byte notes[8] = {60,62,64,65,67,69,71,72};
 
//these are the digital pins for the leds, pins are 5 and 10 due to physical constraints
int ledPin = 3;
 
/*these are the analog pins for the pots, not in order because due to physical constraints
it was better to use 1, 2, 0 and 3*/
int potPins [] = {1,2,0,3};
 
void setup () {
  //go over the led pins establishing them as OUTPUT
  //for (int thisLed = 0; thisLed < ledCount; thisLed++) {
   // pinMode(ledPins[thisLed], OUTPUT);
  //}
   pinMode(ledPin, OUTPUT);
   MIDI.begin();
 
  //set gaud rate to use with yoke
  Serial.begin(115200);
}
 
void loop () {
  //sequencer
  for (int counter = 0; counter < length; counter++) {
    //read pots
    int potReading = 1024 - analogRead(potPins[counter]);
    //int tempoReading = analogRead(tempoPin);
    //int gateReading = analogRead(gatePin) * 3;
    
    //int tempoReading = map(analogRead(tempoPin), 0, 1024, 50, 750);
    int gateReading = map(analogRead(gatePin), 0, 1024, 50, 2250);
    
    //turn on leds
    digitalWrite(ledPin, HIGH);
    //digitalWrite(ledPins[1], HIGH);
 
    //convert readings from note pot to a number from 0 to 31 to be used with notes
    int thisNote = map(potReading, 0, 1024, 0, 31);
   
    //send midi message to play note
    MIDI.sendNoteOn(notes[thisNote],127,1);
   
    //delay to turn off note and led (gate)
    delay(gateReading);
   
    //read tempo gate again
    //tempoReading = analogRead(tempoPin);
    int tempoReading = map(analogRead(tempoPin), 0, 1024, 50, 750);
    
    //turnoff led
    
    digitalWrite(ledPin, LOW);
   
    //turnoff midi
    MIDI.sendNoteOff(notes[thisNote],0,1);
   
    //delay to play next note (tempo)
    delay(tempoReading);
  }

}
