#include <Encoder.h>

int motorStepPin = 4;
int motorDirPin = 6;
int motorEnablePin = 5;
int stepsperrev = 6400;
long pulsedelay, pulsedelay2;
// length of time to output a pulse and read the encoder in us
// determined by oscilloscope.  And there's a bunch of jitter,
// so this is an eyeball average
int readdur = 88;

float spinrate = 0;

// Encoder pins.  Need to have interrupts on the pins, and on
// this Arduino, that's only 2 and 3
Encoder encoder(2, 3);

void setup()
{
  Serial.begin(115200);
  
  pinMode(motorStepPin, OUTPUT);
  pinMode(motorDirPin, OUTPUT);
  pinMode(motorEnablePin, OUTPUT);
  digitalWrite(motorEnablePin, HIGH);
}

void loop()
{
  long pos1, pos2;
  
  if (Serial.available()) {
    spinrate = Serial.parseFloat();
    pulsedelay = 1000000 / (abs(spinrate) * stepsperrev) - readdur;
    Serial.print("Pulse delay (us): ");
    Serial.println(pulsedelay);
    if (pulsedelay <= 0) {
      Serial.println("Error: movement too fast");
      spinrate = 0;
    }
    
    // set the direction
    if (spinrate > 0) {
      digitalWrite(motorDirPin, HIGH);
    }
    else {
      digitalWrite(motorDirPin, LOW);
    }
  }
  
  if (spinrate != 0) {
    pos1 = encoder.read();
    digitalWrite(motorStepPin, HIGH);
    digitalWrite(motorStepPin, LOW);
    Serial.write(pos1);
    delayMicroseconds(pulsedelay);
  }
}
      
    
  
    
  


