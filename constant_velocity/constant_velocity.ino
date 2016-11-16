
/*  Motor Speed (d/s) = {(10^6)*360}/{(Pulsewidth*2)*SPR}   
    SPR = 1600
    */
    
#include <DueTimer.h>

////---pan motion variables---
//int pulsep = 8;        //Pin number connected to pulse input 
//int dirp = 9;         // Pin number connected to direction input
//int enablep = 10;     //Pin number connected to enable input

//---tilt motion variables---
int pulset = 11;        //Pin number connected to pulse input 
int dirt = 12;         // Pin number connected to direction input
int enablet = 13;     //Pin number connected to enable input

long pulseWidtht =0;  //for driving motor 1
int delayTP = 1;

int val1=0;
int val3=0;
int angle = 5;

void setup(){
  Serial.begin(115200);
  pinMode(pulset, OUTPUT); 
  pinMode(dirt, OUTPUT); // stepper  dir High=CW low=CCW
  pinMode(enablet, OUTPUT); // stepper 1 Enable High=On Low=Off
  digitalWrite(enablet, LOW); 
  digitalWrite(dirt, val3);

//  int command =0;
//  
//  Serial.println("enter 1 to start");
  
//  while (!Serial.available());{
//    command = Serial.read();
//    if(command = 1)
//    {
      digitalWrite(enablet, LOW);
//    }

  pulseWidtht= 1000*1000/0.45*delayTP/angle;
//  }
}
void step1(){
  val3 =~ val3;
  digitalWrite(pulset,val3); 
  //Serial.println("step"); 
}

void dirchange(){
    val1 =~ val1;
    digitalWrite(dirt,val1);
    //Serial.println("dir");
    }
 
void loop()
{ 
      dirchange();
      Timer.setPeriod(1200).start().attachInterrupt(step1);
      delay(delayTP*15000);
}

