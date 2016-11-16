#include <DueTimer.h>
// togp = pulse toggle after pulse width
// togd = direction toggle after half time period
int togp=0;
int togd=0;
double pulseWidth=0;
double vel=0;

//motion variables
double Amp1 = 0.008836*2*180/PI; // converted in degrees           
double Amp2 = 0.008861*2*180/PI;
double Amp3 = 0.009528*2*180/PI;
double Amp4 = 0.008116*2*180/PI;
double Amp5 = 0.01016*2*180/PI;

double f1 = 0.1165*2*PI;
double f2 = 0.2329*2*PI;
double f3 = 0.2912*2*PI;
double f4 = 0.3078*2*PI;
double f5 = 0.1581*2*PI;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(11, OUTPUT); // pulse pin
  pinMode(12, OUTPUT); // direction pin
  pinMode(13, OUTPUT); // enable pin
  
  int command =0;
  
  Serial.println("enter 1 to start");
  
  while (!Serial.available());{
    command = Serial.read();
    if(command = 1)
    {
      digitalWrite(13, LOW); //for active low condition, otherwise HIGH
    }
  }
  digitalWrite(12, LOW);
}

  //----------------------------
  //---pulse toggle function---
  //----------------------------
void changepul() {
  togp = ~togp;
  digitalWrite(11, togp);
}


void loop() {
  //  USING FORMULA
    /*  Motor Speed (d/s) = {(10^6)*360}/{(Pulsewidth*2)*SPR}   
    SPR = 1600
    */  
  vel = 100*(Amp1*f1*cos(f1*millis()/(1000)));
//  Serial.println(vel);
  if (vel>=0){
    digitalWrite(12,HIGH);
    pulseWidth= 1000*1000*1.8/(2*vel);                               
    Timer.setPeriod(pulseWidth).start().attachInterrupt(changepul);
//    Serial.println(pulseWidth);
  }
  if (vel<0){
    digitalWrite(12,LOW);
    pulseWidth= (-1)*1000*1000*1.8/(2*vel);
    Timer.setPeriod(pulseWidth).start().attachInterrupt(changepul);
//    Serial.println(pulseWidth); 
  }
  delay(20);
//  Serial.println("----");
}                

