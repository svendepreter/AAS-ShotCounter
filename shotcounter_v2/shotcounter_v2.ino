#include <SoftPWM.h>
#include <SoftPWM_timer.h>

#define DIP_1 2
#define DIP_2 3
#define DIP_3 4
#define DIP_4 5
#define DIP_5 6
#define DIP_6 7

#define LED_1 8
#define LED_2 9
#define LED_3 10
#define LED_4 11
#define LED_5 12

#define SIG_IN  A1
#define SIG_OUT A0
#define RESET_BTN A2

#define SIG_DEBOUNCE 1000
#define SIG_HIGH 1000

#define LED_BRIGHTNESS 16
bool dip_HIGH(unsigned char dipswitch){
  switch(dipswitch){
    case 1 : return digitalRead(DIP_1)==HIGH;
    case 2 : return digitalRead(DIP_2)==HIGH;
    case 3 : return digitalRead(DIP_3)==HIGH;
    case 4 : return digitalRead(DIP_4)==HIGH;
    case 5 : return digitalRead(DIP_5)==HIGH;
    case 6 : return digitalRead(DIP_6)==HIGH;
    default:
    return false;
  }
}

void toggleLed(unsigned char ledId,bool off)
{
  unsigned char ID=0;
  switch(ledId){
    case 1: ID=LED_1; break;
    case 2: ID=LED_2; break;
    case 3: ID=LED_3;break;
    case 4: ID=LED_4; break;
    case 5: ID=LED_5;break;
    default:
      ID=0;
  }
  if (ID!=0)
  {
    SoftPWMSet(ID,off?0:LED_BRIGHTNESS);
  }
 }


void setup() {
  // put your setup code here, to run once:
 SoftPWMBegin();
 pinMode(DIP_1,INPUT);
 pinMode(DIP_2,INPUT);
 pinMode(DIP_3,INPUT);
 pinMode(DIP_4,INPUT);
 pinMode(DIP_5,INPUT);
 pinMode(DIP_6,INPUT);
 pinMode(LED_1,OUTPUT);
 pinMode(LED_2,OUTPUT);
 pinMode(LED_3,OUTPUT);
 pinMode(LED_4,OUTPUT);
 pinMode(LED_5,OUTPUT);
 pinMode(SIG_IN,INPUT);
 pinMode(SIG_OUT,OUTPUT);
 Serial.begin(9600);

}

void signalOutput(){
      unsigned long startTime = millis();
      Serial.print("SIGNAL OUT:Triggering output for ");
      Serial.print(SIG_HIGH,DEC);
      Serial.println(" msec");
      while (millis()-startTime < SIG_HIGH)
      {
        SoftPWMSet(SIG_OUT,0);
      }
      Serial.println("SIGNAL OUT: Resuming normal operation");
      SoftPWMSet(SIG_OUT,255);
      while (digitalRead(RESET_BTN)==HIGH)
      {
        delay(10);     
      }
}


void loop() {
  // put your main code here, to run repeatedly:
  int ledsHit=0;
  for (int i=1;i<6;i++) {
    toggleLed(i,true);
  }
  unsigned long lastHit=0;
  bool current_State=LOW;

  while(true){
    // input needs to be pulled low
     bool readSensor=digitalRead(SIG_IN)==HIGH;
    SoftPWMSet(SIG_OUT,255);
    // Input debouncing
    if (millis()-lastHit >= SIG_DEBOUNCE && readSensor){
    
      if(readSensor){
        lastHit=millis();
        ledsHit++;
        toggleLed(ledsHit,false);
        Serial.print("LedsHit=");
        Serial.println(ledsHit,DEC);
      
        if (dip_HIGH(ledsHit))    {  
          signalOutput();
        }
        // reset
        if (ledsHit > 5 || (dip_HIGH(6) && dip_HIGH(ledsHit))){
          ledsHit=0;
          for (int i=1;i<6;i++){
          toggleLed(i,true);
        }
        lastHit=millis();
      }
      
     }
    }
    
  }
}
