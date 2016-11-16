#include <DueTimer.h>
#include <math.h>

// togp = pulse toggle after pulse width
// togd = direction toggle after half time period
int togp_tilt=0;
int togd_tilt=0;
int togp_pan=0;
int togd_pan=0;

int motor = 3;    // setting random value for variable "motor"
int choice = 10;  // setting random value for variable "choice"

//----pulsewidth vairable parameters for diffrenet axis motions---
double pulseWidth_pan=0;
double pulseWidth_tilt=0;

//double time_prev=0;
//double time_now=0;

//software reset variables
int reset = 0;
int resetpin = 2;     // pin 2 of arduino is connected to reset pin through jumper wire

//----------------------------------
//--Enter the parameters of motion--
//----------------------------------
double amp_pan = 5;
double amp_tilt = 5;
double frequency= 0.1;

//---arduino-motor connections---
int enable_tilt = 13;
int dir_tilt = 12;
int pulse_tilt = 11;
int enable_pan = 10;
int dir_pan = 9;
int pulse_pan = 8; 

//--temp variables used in the code
double factor_pan = 0;
double factor_tilt = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  //---Setting the initial conditions--
  pinMode(pulse_tilt, OUTPUT); // pulse high,low
  pinMode(dir_tilt, OUTPUT); //   dir High=CW low=CCW
  pinMode(enable_tilt, OUTPUT); //Enable High=On Low=Off
  
  digitalWrite(enable_tilt, LOW);   // making sure the motor is off
  digitalWrite(dir_tilt, togd_tilt);  // making sure that motor starts in a particular direction
  
  pinMode(pulse_pan, OUTPUT); //  pulse high,low
  pinMode(dir_pan, OUTPUT); //    dir High=CW low=CCW
  pinMode(enable_pan, OUTPUT); // Enable High=On Low=Off
  
  digitalWrite(enable_pan, LOW);
  digitalWrite(dir_pan, togd_pan);

  //---------------------------------------
  //---code to choose the mode of motion---
  //---------------------------------------
   Serial.println("\n \n Select motor");
   Serial.println(" 1 for Pan Only"); 
   Serial.println(" 2 for Tilt Only ");
   Serial.println(" 3 for Pan and Tilt  ");             
      while (!Serial.available());{  
            motor =Serial.read()-'1';
      }
   Serial.print("Selected motor mode = \t");
   Serial.println((motor+1));

  //---------------------------------------------------
  //---choosing among the predefined motion profiles---
  //---------------------------------------------------
  Serial.println("Choose the amplitude among the following:");
  Serial.println("Enter 1 for 15 degrees and 0.1 Hz");
  Serial.println("Enter 2 for 8 degrees and 0.3 Hz");
  Serial.println("Enter 3 for 2.7 degrees and 1 Hz");
  Serial.println("Enter 4 for 0.22 degrees and 5 Hz");
  Serial.println("Enter 5 for 0.15 degrees and 10 Hz");
  while (!Serial.available());{
    choice = Serial.read()-'1';    
  }

  if (choice ==0){
    amp_pan = 15;
    amp_tilt = 15;
    frequency = 0.1 *2*PI;
  }

  if (choice ==1){
    amp_pan = 8;
    amp_tilt = 8;
    frequency = 0.3 *2*PI;
  }

  if (choice ==2){
    amp_pan = 2.7;
    amp_tilt = 2.7;
    frequency = 1 *2*PI;
  }

  if (choice ==3){
    amp_pan = 0.22;
    amp_tilt = 0.22;
    frequency = 5 *2*PI;
  }

  if (choice ==4){
    amp_pan = 0.15;
    amp_tilt = 0.15;
    frequency = 10  *2*PI;
  }

  //---------------------------------------------------
  //---enabling based on the mode of motion selected---
  //---------------------------------------------------          
   if(motor==0){
       digitalWrite(enable_pan,HIGH);
       Serial.println("*********MOTOR 1(PAN) ENABLED*********");
    }
   else if (motor == 1){
       digitalWrite(enable_tilt,HIGH);
       Serial.println("*********MOTOR 2 (TILT) ENABLED*********");
    }
   else if (motor == 2){
       digitalWrite(enable_pan,HIGH);
       digitalWrite(enable_tilt,HIGH);
    Serial.println("*********MOTOR 1(PAN) and 2(TILT) ENABLED*********");
    }                
  delay(1000); 
}

  //----------------------------
  //---pulse toggle functions---
  //----------------------------
void pultilt() {
  togp_tilt = ~togp_tilt;
  digitalWrite(pulse_tilt, togp_tilt);
}

void pulpan() {
  togp_pan = ~togp_pan;
  digitalWrite(pulse_pan, togp_pan);
}

  //---------------------------------------------------
  //---calcuating pulsewidth and direction of motion---
  //---------------------------------------------------

/*  Motor Speed (d/s) = {(10^6)*360}/{(Pulsewidth*2)*SPR}   
    SPR = 1600
    */
void freqtilt(){
  factor_tilt = 100*(amp_tilt*frequency*cos(frequency*millis()/(1000)));    // uses arduino clock to measure the required velocity and converts it into pulsewidth
  if (factor_tilt>=0){
    digitalWrite(dir_tilt,HIGH);    
    pulseWidth_tilt= 1000*1000*1.8/(8*factor_tilt);     
    //Serial.println(pulseWidth_tilt);
  }
  
  if (factor_tilt<0){
    digitalWrite(dir_tilt,LOW);
    pulseWidth_tilt= (-1)*1000*1000*1.8/(8*factor_tilt);
    //Serial.println(pulseWidth_tilt); 
  }
}

void freqpan(){
  factor_pan = 100*(amp_pan*frequency*cos(frequency*millis()/(1000)));
    if (factor_pan>=0){
    digitalWrite(dir_pan,HIGH);    
    pulseWidth_pan= 1000*1000*1.8/(8*factor_pan);     
    //Serial.println(pulseWidth_pan);
  }
  
  if (factor_pan<0){
    digitalWrite(dir_pan,LOW);
    pulseWidth_pan= (-1)*1000*1000*1.8/(8*factor_pan);
    //Serial.println(pulseWidth_pan); 
  }  
}

void loop() { 
    
    //---find the required pulsewidth---
    freqtilt();
    freqpan();

    //---toggle the pulses at the calculated pulsewidth---
    Timer.setPeriod(pulseWidth_tilt).start().attachInterrupt(pultilt);
    Timer1.setPeriod(pulseWidth_pan).start().attachInterrupt(pulpan);
    
    //60 denotes the number of sampling points in one time period
    delay(1000/(frequency/2/PI)/60);      
    
    //software reset  code
    //software resets on pressing "q" or "Q"
    if (Serial.available() >0){
    reset = Serial.read();
    Serial.print("received : ");
    delay(10);
    Serial.println(reset);
    delay(10);
    if (reset ==  113 || reset == 81){
      Serial.println("resetting... ");
      delay(10);
      pinMode(resetpin,LOW);
      delay(10);
      pinMode(resetpin,HIGH);
    }
  }             
}
