#include <IRremote.h>
// uses IR remote and receiver
// receiver is attached to +3.3V, GND, and pin 11

int RECV_PIN = 11;
int time = 12000;
int relay = 13; // relay turns trigger signal - active high;
IRrecv irrecv(RECV_PIN);
decode_results results;


void setup()
{
  Serial.begin(9600);
  irrecv.enableIRIn(); // Start the receiver
  pinMode (relay, OUTPUT); // Define port attribute is output;
}

void loop() {
  if ((irrecv.decode(&results))){
    Serial.println(results.value, DEC);
    irrecv.resume(); // Receive the next value
  }
    
  
  if ((results.value == 16736925) || (results.value == 16745085) || (results.value == 1784778242))
    // 16736925 = Color lights, "DIY 6"
    // 16748655 = Raspberry Pi Remote, (EQ)       
    // 1784778242 = TV remote
    {
      
      Serial.println("SUCCESS");
    
    
      digitalWrite (relay, HIGH); // relay conduction;
      delay (time);
    
      //digitalWrite (relay, LOW); // relay switch is turned off;
      //delay (time / 3);
    
      //digitalWrite (relay, HIGH); // relay conduction;
      //delay (time);
    
      digitalWrite (relay, LOW); // relay switch is turned off;
      delay (time/6);
      
      results.value = 0;
    
      //irrecv.resume(); // Receive the next value
    }


}
