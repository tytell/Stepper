#include <TimerOne.h>

int motorStepPin = 3;
int motorDirPin = 4;
int motorDisablePin = 2;

int stepsperrev = 2000;

float spinrate = 0;
float amp = 0;
float freq = 0;


long pulsedelay;
long nsteps = 0;
long revsteps2;
int direc;
int stopping = 0;

long sampledelay = 100;  // msec

void setup()
{
  Serial.begin(115200);
  
  pinMode(motorStepPin, OUTPUT);
  pinMode(motorDirPin, OUTPUT);
  pinMode(motorDisablePin, OUTPUT);
  digitalWrite(motorDisablePin, LOW);

  Timer1.initialize(1000);
  Timer1.attachInterrupt(doStep);
}

void loop()
{
  if (Serial.available()) {
    amp = Serial.parseFloat();
    freq = Serial.parseFloat();
    
    if ((amp > 0) && (freq > 0)) {
      spinrate = 4*amp/360*freq;
      pulsedelay = 1000000 / (abs(spinrate) * stepsperrev);
      revsteps2 = (500000/freq) / pulsedelay / 2;
      
      Serial.print("Amplitude (deg): ");
      Serial.println(amp);
      Serial.print("Frequency (Hz): ");
      Serial.println(freq);
      
      Serial.print("Pulse delay (us): ");
      Serial.println(pulsedelay);
  
      Serial.print("Pulses per reversal: ");
      Serial.println(2*revsteps2);
      digitalWrite(motorDisablePin, LOW);
      Timer1.setPeriod(pulsedelay);

      // set the direction
      digitalWrite(motorDirPin, HIGH);
      direc = 1;
      nsteps = revsteps2;
      
      stopping = 0;
    }
    else {
      Serial.println("Stopping....");
      stopping = 1;
    }      
  }
  
  if ((stopping == 1) && (spinrate == 0)) {
    Serial.println("Stopped!");
    stopping = 0;
  }
  
  delay(sampledelay);  
}

void doStep()
{
  if (spinrate != 0) {
    digitalWrite(motorStepPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(motorStepPin, LOW);
    
    if ((stopping == 1) && (nsteps == revsteps2)) {
      spinrate = 0;
    }
    if (nsteps >= 2*revsteps2) {
      if (direc > 0) {
        direc = -1;
        digitalWrite(motorDirPin, LOW);
      }
      else {
        direc = 1;
        digitalWrite(motorDirPin, HIGH);
      }
      nsteps = 0;
    }      
    nsteps += 1;   
  }
}
    
  
    
  


