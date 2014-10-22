#include <TimerOne.h>

int motorStepPin = 3;
int motorDirPin = 4;
int motorDisablePin = 2;

int stepsperrev = 6400;

float spinrate = 0;
long pulsedelay;
long sampledelay = 100;  // msec

void setup()
{
  Serial.begin(115200);
  
  pinMode(motorStepPin, OUTPUT);
  pinMode(motorDirPin, OUTPUT);
  pinMode(motorEnablePin, OUTPUT);
  digitalWrite(motorDisablePin, LOW);

  Timer1.initialize(1000);
  Timer1.attachInterrupt(doStep);
}

void loop()
{
  long pos1, pos2;
  
  if (Serial.available()) {
    spinrate = Serial.parseFloat();
    pulsedelay = 1000000 / (abs(spinrate) * stepsperrev);
    Serial.print("Pulse delay (us): ");
    Serial.println(pulsedelay);
    if (pulsedelay <= 0) {
      Serial.println("Error: movement too fast");
      digitalWrite(motorDisablePin, HIGH);
      spinrate = 0;
    }
    else {
      digitalWrite(motorDisablePin, LOW);
      Timer1.setPeriod(pulsedelay);
    }
    
    // set the direction
    if (spinrate > 0) {
      digitalWrite(motorDirPin, HIGH);
    }
    else {
      digitalWrite(motorDirPin, LOW);
    }
  }
  
  delay(sampledelay);  
}

void doStep()
{
  if (spinrate != 0) {
    digitalWrite(motorStepPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(motorStepPin, LOW);
  }
}
    
  
    
  


