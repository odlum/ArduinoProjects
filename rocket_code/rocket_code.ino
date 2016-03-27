/*
Code for Orbital Sciences Rocket Competition
Geoffrey Odlum
02 March 2014
--------------SD CARD----------------------------------------------
The circuit:
 * SD card attached to SPI bus as follows:
 ** UNO:  DI - pin 11, DO - pin 12, CLK - pin 13, CS - pin 10 (CS pin can be changed)
  and pin #10 (SS) must be an output
 		Pin 4 used here for consistency with other Arduino examples
--------------PRESSURE SENSOR---------------------------------------
PIN: A0
pressure = (analogRead(0)/1024.0 + 0.095 ) / 0.0009;
--------------THERMISTORS-------------------------------------------
A) INSIDE: A1
B) OUTSIDE: A2

--------------ACCELEROMETER------------------------
x: A3
Y: A4
Z: A5
--------------LEDs--------------------------------------------------
 RED:  	 8
 GREEN:  9
 */
 
#include <avr/pgmspace.h>
#include <SD.h>
#include <math.h>
#include <SPI.h>

int led_state;
unsigned long time;

//Pressure
int pressurePIN = A0;

//Thermistors
int THERM_PIN_IN = A1;  // 10ktherm & 10k resistor as divider.
int THERM_PIN_OUT = A2;

//Accelerometer
const int xpin = A3;                  // x-axis of the accelerometer
const int ypin = A4;                  // y-axis
const int zpin = A5;                  // z-axis (only on 3-axis models)
int sleep = 5;

//LEDs
int RED = 8;
int GREEN = 9;


const int chipSelect = 10;

File dataFile;

double Thermistor(int RawADC) {
 double Temp;
 Temp = log(10000.0*((1024.0/RawADC-1))); 
//         =log(10000.0/(1024.0/RawADC-1)) // for pull-up configuration
 Temp = 1 / (0.001129148 + (0.000234125 + (0.0000000876741 * Temp * Temp ))* Temp );
 Temp = Temp - 273.15;            // Convert Kelvin to Celcius
 //Temp = (Temp * 9.0)/ 5.0 + 32.0; // Convert Celcius to Fahrenheit
 return Temp;
}

void setup()
{
 
 // Open serial communications and wait for port to open:
  Serial.begin(9600);
  
//bit from thermistor code. What does it do? Yo no se  
//pinMode(13, OUTPUT);
  //Serial.begin(19200);
  time=millis();
  //Serial.println("Ready");
     
  Serial.print("Initializing SD card...");
  // make sure that the default chip select pin is set to
  // output, even if you don't use it:
  pinMode(SS, OUTPUT);
  
   //see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    while (1) ;
  }
  Serial.println("card initialized.");
  
  // Open up the file we're going to log to!
  dataFile = SD.open("datalog.txt", FILE_WRITE);
  if (! dataFile) {
    Serial.println("error opening rocket.txt");
    // Wait forever since we cant write data
    while (1) ;    
  }

//Blinking LEDs

pinMode(RED, OUTPUT);
pinMode(GREEN, OUTPUT);

pinMode(sleep, OUTPUT);
}

void loop() 
{
//THERMISTORS
  int therm_in = int(Thermistor(analogRead(THERM_PIN_IN)));
  int therm_out = int(Thermistor(analogRead(THERM_PIN_OUT)));

  if (millis() - time >1000){   
  time=millis();
  digitalWrite(13,led_state);
  led_state=!led_state;
  }
  
//BAROMETER

  int pressure;
  pressure = (analogRead(pressurePIN)/1024.0 + 0.095 ) / 0.0009;
  //Serial.print(pressure);
  
//ACCELERATION
  digitalWrite(sleep,HIGH);
  int xVal;
  int yVal;
  int zVal;
  xVal = analogRead(xpin);
  yVal = analogRead(ypin);
  zVal = analogRead(zpin);

  
//DATA LOGGING
  // make a string for assembling the data to log:
  String dataString = "";
 
  // read three sensors and append to the string:
  //for (int analogPin = 0; analogPin < 6; analogPin++) {
  //int sensor = analogRead(analogPin);
  //  dataString += String(sensor);
  //  if (analogPin < 5) {
  //   dataString += " "; 
  //  }
  //}
  String timeString = "t(s): " + String(time/1000);
  String tempString = "  T(in): " + String(therm_in) + " T(out): " + String(therm_out); 
  String pressureString = "  Pressure: " + String(pressure);
  String accelString = "  (X,Y,Z): (" + String(xVal) + "," + String(yVal) + "," + String(zVal) + ")";
  dataString = timeString + tempString + pressureString + accelString; 
  
  dataFile.println(dataString);

  // print to the serial port too:
  Serial.println(dataString);
  
  // The following line will 'save' the file to the SD card after every
  // line of data - this will use more power and slow down how much data
  // you can read but it's safer! 
  // If you want to speed up the system, remove the call to flush() and it
  // will save the file only every 512 bytes - every time a sector on the 
  // SD card is filled with data.
  dataFile.flush();
  digitalWrite(RED, HIGH);   // turn the LED on (HIGH is the voltage level)
  digitalWrite(GREEN, LOW);
  // Take 1 measurement every 100 milliseconds
  delay(500);
  digitalWrite(RED, LOW);
  digitalWrite(GREEN, HIGH);
  delay(500);
  //blinkLEDs();
}  

//LEDs
//void blinkLEDs(){
  //digitalWrite(RED, HIGH);   // turn the LED on (HIGH is the voltage level)
  //digitalWrite(GREEN, LOW);
  //delay(750);               // wait for a second
  //digitalWrite(RED, LOW);  // turn the LED off by making the voltage LOW
  //digitalWrite(GREEN, HIGH);
  //delay(750);               // wait for a second
//}










