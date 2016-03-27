#include <IRremote.h>

// I/O pins for LED/Arduino connection
int REDPIN = 10;
int GREENPIN = 9;
int BLUEPIN = 6;
int RECV_PIN = 4;
int RELAYPIN = 13;

IRrecv irrecv(RECV_PIN);
decode_results results;
const int TIME = 12000;

// Remote control IR button values
const int POWER = 16712445;
const int RED = 16718565;
const int GREEN = 16751205;
const int BLUE = 16753245;
const int WHITE = 16720605;
const int PINK = 16734375;

const int UPBRIGHT = 16726725;
const int DOWNBRIGHT = 16759365;

const int FADESEVEN = 16769055;
const int FADETHREE = 16736415;
const int JUMPSEVEN = 16752735;
const int JUMPTHREE = 16720095;
const int FASTER = 16771095;
const int SLOWER = 16762935;


const int DIYONE = 16724175;
const int DIYTWO = 16756815;
const int DIYTHREE = 16740495;
const int DIYFOUR = 16716015;
const int DIYFIVE = 16748655;
const int DIYSIX = 16732335;

const int UPRED = 16722135;
const int DOWNRED = 16713975;
const int UPGREEN = 16754775;
const int DOWNGREEN = 16746615;
const int UPBLUE = 16738455;
const int DOWNBLUE = 16730295;

const int DOORONE = 16736925;
const int DOORTWO = 16745085;
const int DOORTHREE = 1784778242;

// LED Fade timer (Make higher to slow down)
int FADESPEED = 5; 

// LED States
boolean powerState = false;
int currentState;
int lastColor = WHITE;
boolean bright = true;
int brightness[10] = {230, 204, 178, 153, 127, 102, 76, 51, 25, 0};
int brightnessCoef = 2;
int red[3] = {255,  0, 0};
int green[3] = {0, 255, 0};
int blue[3] = {0, 0, 255};
int white[3] = {255, 150, 200};
int pink[3] = {255, 0, 212};

int* currentColor = white;
int currentR;
int currentG;
int currentB;
/*boolean fadeSev = false;
boolean fadeThr = false;
boolean jumpSev = false;
boolean jumpThr = false;
*/


void setup() {
  Serial.begin(9600);
  irrecv.enableIRIn(); // Start Reciever
  pinMode(RELAYPIN, OUTPUT);
  pinMode(REDPIN, OUTPUT);
  pinMode(GREENPIN, OUTPUT);
  pinMode(BLUEPIN, OUTPUT);
  pinMode(RECV_PIN, INPUT);
  
/*  for(int x = 0; x < 255; x++) {
  analogWrite(REDPIN, x);
delay(10);  
}
for (int x = 255; x > 0; x--)
{
  analogWrite(REDPIN, x);
delay(10);
}*/
  
  
}

void openDoor(){
  digitalWrite (RELAYPIN, HIGH); // relay conduction;
  delay (TIME);
    
  //digitalWrite (relay, LOW); // relay switch is turned off;
  //delay (time / 3);
    
  //digitalWrite (relay, HIGH); // relay conduction;
  //delay (time);
    
  digitalWrite (RELAYPIN, LOW); // relay switch is turned off;
  delay (TIME/12);
  results.value = 0;
}
//Set the color of the LED's
void setColor(){
  currentR = map(brightness[brightnessCoef], 0, 255, 0, currentColor[0]);
  currentG = map(brightness[brightnessCoef], 0, 255, 0, currentColor[1]);
  currentB = map(brightness[brightnessCoef], 0, 255, 0, currentColor[2]);
  analogWrite(REDPIN, currentColor[0] - currentR);
  analogWrite(GREENPIN, currentColor[1] - currentG);
  analogWrite(BLUEPIN, currentColor[2] - currentB);
}

void loop(){
//Read the IR input and decode it to a result.
  if ((irrecv.decode(&results))){
    Serial.println(results.value, DEC);
    irrecv.resume(); // Receive the next value
  }
  
  //if ((results.value == 16736925) || (results.value == 16745085) || (results.value == 1784778242))
    // 16736925 = Color lights, "DIY 6"
    // 16748655 = Raspberry Pi Remote, (EQ)       
    // 1784778242 = TV remote
    //{
      //openDoor();
    //}
  //else
  //{
//Check the IR input against the power button.
  if(results.value == POWER || currentState == POWER){
    if(powerState == true){
      powerState = false;
      digitalWrite(REDPIN, LOW); 
      digitalWrite(GREENPIN, LOW);
      digitalWrite(BLUEPIN, LOW);
      
    }
    else {
      powerState = true;
      currentState = lastColor;
    }  
  }
  
//if the power is on, allow the lights to be controlled
  if(powerState == true){
    //check results against the brightness up and down buttons
    if(results.value == 16726725 && brightnessCoef < 9){
      brightnessCoef++;
    }
    else if(results.value == 16759365 && brightnessCoef > 0){
      brightnessCoef--;
    }
    // check results against button codes and change the lights accordingly.
    if(results.value == RED || currentState == RED){
        currentColor = red;
        lastColor = RED;
      }
    else if(results.value == GREEN || currentState == GREEN){
        currentColor = green;
        lastColor = GREEN;
    }
    else if(results.value == BLUE || currentState == BLUE){
        currentColor = blue;
        lastColor = BLUE;
    }
    else if(results.value == WHITE || currentState == WHITE){
        currentColor = white;
        lastColor = WHITE; 
    }
    else if(results.value == PINK || currentState == PINK){
      currentColor = pink;
      lastColor = PINK;
    }
    //RED
    /*if(results.value == 16718565){
        currentColor = red;
        lastColor = RED;
      }
      
    //GREEN
    else if(results.value == 16751205){
        currentColor = green;
        lastColor = GREEN;
    }
    
    //BLUE
    else if(results.value == 16753245){
        currentColor = blue;
        lastColor = BLUE;
    }
    
    //WHITE
    else if(results.value == 16720605){
      
      
        currentColor = white;
        lastColor = WHITE; 
    }
    
    //PINK
    else if(results.value == 16734375){
      currentColor = pink;
      lastColor = PINK;
    }*/
    setColor();
  }
  currentState = results.value;
  results.value = 0;
  //}
}
