
/* Test to be Run on clearpathSD motor for moving motor a speed profile
MOTOR 1 : teknic pan
MOTOR 2 :teknic tilt
*/

/*  Motor Speed (d/s) = {(10^6)*360}/{(Pulsewidth*2)*SPR}   
    SPR = 1600
    */

#include <DueTimer.h>
//==============================================
//==========      Motor pins          ==========
//==============================================
int pulse1 = 8;        //Pin number connected to pulse input 
int dir1 = 9;         // Pin number connected to direction input
int enable1 = 10;     //Pin number connected to enable input
int pulse2 = 11;      
int dir2 = 12;
int enable2 = 13;
//===============================================
//====  Motor configurations that are set =======
//===============================================
// If these parameters are changed speed and amplitude changes inversely accordingly
int SPR1 = 1600;  // step per revolutions TEKNIC
int SPR2 = 1600;   // step per revolutions TEKNIC tilt
//===============================================
//======     Random variables in use       ======
//===============================================
long pulseWidth1 =0;  //for driving motor 1
long pulseWidth2 =0;  //for driving motor 2
int val1=0;
int val2=0;
int val3=0;
int val4=0;
int b=0;
int c= 0;

int mode=0;
int motor=0;

float divider[]={0.33,0.5,1,2,3,4,5,6,10};

float delaytable[]={1,2,4,5,10,20,40};

float delaytable2[]={300,500,750,1000,2000,3000,4000,6000};

float veltable[]={500,400,300,200,150,100,50,25};

//Array of cosec values at interval of 1 degree for getting sin velocity profile
float array1[]={100,100,100,100,100,100,101,101,101,101,102,102,102,103,103,104,104,105,105,106,106,107,
108,109,109,110,111,112,113,114,115,117,118,119,121,122,124,125,127,129,131,133,135,137,139,141,144,147,
149,152,156,159,162,166,170,174,179,184,189,194,200,206,213,220,228,237,246,256,267,279,292,307,324,342,
363,386,413,445,481,524,576,639,719,821,957,1147,1434,1434,1434,1434,1434};   

int delayAMP= 0;        //10
int input1=0;
int input2=0;
int input3=0;
int input4=0;

int velinput1=0;
int velinput2=0;
int delayTP=0;
int timeinput1=0;


//==============================================
//======         Initial setup        ==========
//==============================================

void setup(){
  Serial.begin(115200);
  pinMode(pulse1, OUTPUT); 
  pinMode(dir1, OUTPUT); // stepper  dir High=CW low=CCW
  pinMode(enable1, OUTPUT); // stepper 1 Enable High=On Low=Off
  digitalWrite(enable1, LOW); 
  digitalWrite(dir1, val3);
  pinMode(pulse2, OUTPUT); 
  pinMode(dir2, OUTPUT); 
  pinMode(enable2, OUTPUT); 
  digitalWrite(enable2, LOW); 
  digitalWrite(dir2, val4);
//===============================================================================================================================================//
//=============================================================// 
    Serial.println("\n \n Select mode");
    Serial.println(" 1 for Constant Velocity mode"); 
    Serial.println(" 2 for Sine Velocity mode ");          
      while (!Serial.available());{  
            mode =Serial.read()-'1';
      }
     
      if (mode== 0){
          Serial.println("*********************************");
          Serial.println("Constant Velcoity Mode");
          Serial.println("*********************************");

         //Setting Speed of motor                                
          Serial.println("\n choose Maximum speed of motion for Pan Platform(0.01 times of MOTOR1)...");
          Serial.println(" 1 for ~2.25 deg/s");//5600
          Serial.println(" 2 for ~2.81 deg/s");//400
          Serial.println(" 3 for ~3.75 deg/s");//300
          Serial.println(" 4 for ~5.62 deg/s");//200
          Serial.println(" 5 for ~7.5  deg/s");//150
          Serial.println(" 6 for ~11.25deg/s");//100
          Serial.println(" 7 for ~22.5 deg/s ! caution!");//50
          Serial.println(" 8 for ~45   deg/s ! caution!  ");//25

            while (!Serial.available());  
              velinput1 = Serial.read()-'1';
          Serial.print("Selected pan speed = \t");
          Serial.println((velinput1+1));
          pulseWidth1=veltable[velinput1];
          
          Serial.println("\n choose Maximum speed of motion for Tilt Platform(0.01 times of MOTOR2)...");
          Serial.println(" 1 for ~2.25 deg/s");//5600
          Serial.println(" 2 for ~2.81 deg/s");//400
          Serial.println(" 3 for ~3.75 deg/s");//300
          Serial.println(" 4 for ~5.62 deg/s");//200
          Serial.println(" 5 for ~7.5  deg/s");//150
          Serial.println(" 6 for ~11.25deg/s");//100
          Serial.println(" 7 for ~22.5 deg/s ! caution!");//50
          Serial.println(" 8 for ~45   deg/s ! caution!  ");//25

            while (!Serial.available());  
              velinput2 = Serial.read()-'1';
          Serial.print("Selected tilt speed = \t");
          Serial.println((velinput2+1));
          pulseWidth2=veltable[velinput2];
          
          Serial.println(pulseWidth1);
          Serial.println(pulseWidth1);

          
          Serial.println("\n choose Maximum Time Period  motion ");
          Serial.println(" 1 for 0.3  ! caution!s");//5600
          Serial.println(" 2 for 0.5  ! caution!s");//400
          Serial.println(" 3 for 0.75 s");//300
          Serial.println(" 4 for  1   s");//200
          Serial.println(" 5 for  2   s");//150
          Serial.println(" 6 for  3   s");//100
          Serial.println(" 7 for  4   s");//50
          Serial.println(" 8 for  6   s  ");//25

            while (!Serial.available());  
              timeinput1 = Serial.read()-'1';
          Serial.print("Selected time period option = \t");
          Serial.println((timeinput1+1));
          delayTP= delaytable2[timeinput1];
          Serial.print("Selected time delay in millis = \t");
          Serial.println(delayTP);


      }



            
    
      else if(mode==1){  
          Serial.println("*********************************");
          Serial.println("Sine Velcoity Mode");
          Serial.println("*********************************");

                  
        //Setting Speed of motor                                
        // To set different maximum speed command we will be changing the mimumum pulseWidth that we will be giving the following values are for SPR 1600  
          Serial.println("\n choose Maximum speed of motion for Pan Platform(0.01 times of MOTOR1)...");
          Serial.println(" 1 for ~.625 RPM");
          Serial.println(" 2 for ~.937 RPM");
          Serial.println(" 3 for ~1.87 RPM");
          Serial.println(" 4 for ~3.75 RPM");
          Serial.println(" 5 for ~5.62 RPM");
          Serial.println(" 6 for ~7.50 RPM- Caution! ");
          Serial.println(" 7 for ~9.37 RPM- not ready");
          Serial.println(" 8 for ~11.2 RPM- not ready");
          Serial.println(" 9 for ~18.7 RPM- not ready"); 
            while (!Serial.available());  
              input1 = Serial.read()-'1';
          Serial.print("Selected pan speed = \t");
          Serial.println((input1+1));
        
          Serial.println("\n choose Maximum speed of motion for Tilt platform(0.01 times of MOTOR2)...");
          Serial.println(" 1 for ~.625 RPM");
          Serial.println(" 2 for ~.937 RPM");
          Serial.println(" 3 for ~1.87 RPM");
          Serial.println(" 4 for ~3.75 RPM");
          Serial.println(" 5 for ~5.62 RPM");
          Serial.println(" 6 for ~7.50 RPM- Caution! ");
          Serial.println(" 7 for ~9.37 RPM- not ready");
          Serial.println(" 8 for ~11.2 RPM- not ready");
          Serial.println(" 9 for ~18.7 RPM- not ready");  
            while (!Serial.available());
                input3 = Serial.read()-'1';
                
          Serial.print("Selected tilt speed = \t");
          Serial.println(input3+1);
          delay(1000);
        
          
        //  =====================TIME PERIOD===========================
        //To set different Time Period command we will be changing the delayAMP 
          Serial.println("\n Now choose Time Period of motion for Pan platform...");
          Serial.println(" Enter the delay in velocity update(in millis) :");
        
          if(input1 == 0){
          Serial.println(" 2 for 0.36 sec and 1.357 deg travel ");    // delayAMP=2
          Serial.println(" 3 for 0.72 sec and 2.715 deg travel ");    // delayAMP=4
          Serial.println(" 4 for 0.9  sec and 3.393 deg travel");    // delayAMP=5
          Serial.println(" 5 for 1.8 sec and  6.787 deg travel");    // delayAMP=10
          Serial.println(" 6 for 3.6 sec and  13.57 deg travel");    // delayAMP=20
          Serial.println(" 7 for 7.2 sec and  27.15 deg travel");    // delayAMP=40
            
            while (!Serial.available());{  
                  input2 =Serial.read()-'1';
            }
            delayAMP=delaytable[input2];
            Serial.print("Selected option = \t");
            Serial.println(input2+1);
          }
          
          else if(input1 == 1){
          Serial.println(" 2 for 0.36 sec and 2.036 deg travel ");    // delayAMP=2
          Serial.println(" 3 for 0.72 sec and 4.072 deg travel ");    // delayAMP=4
          Serial.println(" 4 for 0.9  sec and 8.145 deg travel");    // delayAMP=5
          Serial.println(" 5 for 1.8 sec  and 16.29 deg travel");    // delayAMP=10
          Serial.println(" 6 for 3.6 sec  and 20.37 deg travel");    // delayAMP=20
          Serial.println(" 7 for 7.2 sec  and 40.72 deg travel");    // delayAMP=40
            
            while (!Serial.available());{  
                  input2 =Serial.read()-'1';
            }
            delayAMP=delaytable[input2];
            Serial.print("Selected option = \t");
            Serial.println(input2+1);
          }
          
          else if(input1 == 2){
          Serial.println(" 2 for 0.36 sec and 4.072 deg travel ");    // delayAMP=2
          Serial.println(" 3 for 0.72 sec and 8.145 deg travel ");    // delayAMP=4
          Serial.println(" 4 for 0.9  sec and 16.29 deg travel");    // delayAMP=5
          Serial.println(" 5 for 1.8 sec and 20.37 deg travel");    // delayAMP=10
          Serial.println(" 6 for 3.6 sec and 40.72 deg travel");    // delayAMP=20
          Serial.println(" 7 for 7.2 sec and 81.45 deg travel");    // delayAMP=40
            
            while (!Serial.available());{  
                  input2 =Serial.read()-'1';
            }
            delayAMP=delaytable[input2];
            Serial.print("Selected option = \t");
            Serial.println(input2+1);
          }
           else if(input1 == 3){
          Serial.println(" 2 for 0.36 sec and 8.145 deg travel ");    // delayAMP=2
          Serial.println(" 3 for 0.72 sec and 16.29 deg travel ");    // delayAMP=4
          Serial.println(" 4 for 0.9  sec and 20.36 deg travel");    // delayAMP=5
          Serial.println(" 5 for 1.8 sec and 40.72 deg travel");    // delayAMP=10
          Serial.println(" 6 for 3.6 sec and 81.45 deg travel");    // delayAMP=20
          Serial.println(" 7 for 7.2 sec and 162.9 deg travel");    // delayAMP=40
            
            while (!Serial.available());{  
                  input2 =Serial.read()-'1';
            }
            delayAMP=delaytable[input2];
            Serial.print("Selected option = \t");
            Serial.println(input2+1);
          }
          else if (input1 == 4){
          Serial.println(" 2 for 0.36 sec and 12.21 deg travel ");    // delayAMP=2
          Serial.println(" 3 for 0.72 sec and 24.43 deg travel ");    // delayAMP=4
          Serial.println(" 4 for 0.9  sec and 30.54 deg travel");    // delayAMP=5
          Serial.println(" 5 for 1.8 sec and 61.08 deg travel");    // delayAMP=10
          Serial.println(" 6 for 3.6 sec and 122.1 deg travel");    // delayAMP=20
          Serial.println(" 7 for 7.2 sec and 244.3 deg travel");    // delayAMP=40
           while (!Serial.available());{  
                  input2 =Serial.read()-'1';
            }
            delayAMP=delaytable[input2];
            Serial.print("Selected option = \t");
            Serial.println(input2+1);
         }
          else if(input1 == 5){
          Serial.println(" 2 for 0.36 seMc and 16.29 deg travel ");    // delayAMP=2
          Serial.println(" 3 for 0.72 sec and 20.36 deg travel ");    // delayAMP=4
          Serial.println(" 4 for 0.9  sec and 40.72 deg travel");    // delayAMP=5
          Serial.println(" 5 for 1.8 sec and 81.45 deg travel");    // delayAMP=10
          Serial.println(" 6 for 3.6 sec and 162.9 deg travel");    // delayAMP=20
              while (!Serial.available());{  
                  input2 =Serial.read()-'1';
            }
            delayAMP=delaytable[input2];
            Serial.print("Selected option = \t");
            Serial.println(input2+1);
          }
        
       Serial.println(" Selected delay");
       Serial.println(delayAMP);
           
   }
   
//    Selecting motors 

   
   Serial.println("\n \n Select motor");
   Serial.println(" 1 for Pan Only"); 
   Serial.println(" 2 for Tilt Only ");
   Serial.println(" 3 for Pan and Tilt  ");             
      while (!Serial.available());{  
            motor =Serial.read()-'1';
      }
   Serial.print("Selected motor mode = \t");
   Serial.println((motor+1));
          
   if(motor==0){
       digitalWrite(enable1,HIGH);
       Serial.println("*********MOTOR 1(PAN) ENABLED*********");
    }
   else if (motor == 1){
       digitalWrite(enable2,HIGH);
       Serial.println("*********MOTOR 2 (TILT) ENABLED*********");
    }
   else if (motor == 2){
       digitalWrite(enable1,HIGH);
       digitalWrite(enable2,HIGH);
    Serial.println("*********MOTOR 1 and 2 ENABLED*********");
    }
   
}
//==============================================
//======         Functions             =========
//==============================================
void step1(){
  val3 = ~val3;
  digitalWrite(pulse1, val3);  
//  Serial.println("a");
}

void step2(){
  val4 = ~val4;
  digitalWrite(pulse2, val4); 
}
//=======================================================
void freq1(){
    if(b>= 0 && b<90){
    pulseWidth1 = array1[b]/divider[input1];
       b++;
  }
  else if(b>90 && b<180){
    pulseWidth1 = array1[180-b]/divider[input1];
      b++;
  } 
  else if(b == 90 ){
    val1 = ~val1;
    digitalWrite(dir1,val1);
    b++;
//    Serial.println("Pan Direction change");
  }

   else if (b == 180){
    b=0;
    }
   
}

//======================================

void freq2(){
    if(c>= 0 && c<90){
    pulseWidth2 = array1[c]/divider[input3];
        c++;
  }
  else if(c>90 && c<180){
    pulseWidth2 = array1[180-c]/divider[input3];
       c++;
  } 
  else if(c == 90 ){
    val2 = ~val2;
    digitalWrite(dir2,val2);
    c++;
//    Serial.println("Tilt Direction change");
  }

   else if (c == 180){
    c=0;
    }
}

//=========================================

void dirchange(){
    val2 = ~val2;
    val1 = ~val1;
//    Timer.detachInterrupt();;
//    Timer1.detachInterrupt();
//    
//     Serial.println("delaystarted");
//     delay(1000);
//     Serial.println("delayended");
    digitalWrite(dir2,val2);
    digitalWrite(dir1,val1);
  
  }
//==================================================
//=====             Main Loop               ========
//==================================================

void loop()
{ 

  switch(mode){
   case 1:
    freq1();
    freq2();
     Timer.setPeriod(pulseWidth1).start().attachInterrupt(step1);
     Timer1.setPeriod(pulseWidth2).start().attachInterrupt(step2);
     delay(delayAMP);
//     Serial.println("SV");
     break;

  case 0:
      dirchange();
      Timer.setPeriod(pulseWidth1).start().attachInterrupt(step1);
      Timer1.setPeriod(pulseWidth2).start().attachInterrupt(step2);
      delay(delayTP);
      Serial.println("CV");
  break;

  }
  
    
}

