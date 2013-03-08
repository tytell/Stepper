#include <avr/pgmspace.h>
#include <TimerOne.h>
#include <Encoder.h>

// table of values of a sine wave
PROGMEM  prog_uchar sine256[]  = {
  127,130,133,136,139,143,146,149,152,155,158,161,164,167,170,173,176,178,181,184,187,190,192,195,198,200,203,205,208,210,212,215,217,219,221,223,225,227,229,231,233,234,236,238,239,240,
  242,243,244,245,247,248,249,249,250,251,252,252,253,253,253,254,254,254,254,254,254,254,253,253,253,252,252,251,250,249,249,248,247,245,244,243,242,240,239,238,236,234,233,231,229,227,225,223,
  221,219,217,215,212,210,208,205,203,200,198,195,192,190,187,184,181,178,176,173,170,167,164,161,158,155,152,149,146,143,139,136,133,130,127,124,121,118,115,111,108,105,102,99,96,93,90,87,84,81,78,
  76,73,70,67,64,62,59,56,54,51,49,46,44,42,39,37,35,33,31,29,27,25,23,21,20,18,16,15,14,12,11,10,9,7,6,5,5,4,3,2,2,1,1,1,0,0,0,0,0,0,0,1,1,1,2,2,3,4,5,5,6,7,9,10,11,12,14,15,16,18,20,21,23,25,27,29,31,
  33,35,37,39,42,44,46,49,51,54,56,59,62,64,67,70,73,76,78,81,84,87,90,93,96,99,102,105,108,111,115,118,121,124,127
};

int motorStepPin = 4;
int motorDirPin = 6;
int motorEnablePin = 5;
int stepsperrev = 6400;

float sinefreq = 0;
float sineamp = 0;
int first = 1;

long stepdelay = 50;
word d;
long cmdpos;
float sinestep;

long encpos;

unsigned int count = 0;

// Encoder pins.  Need to have interrupts on the pins, and on
// this Arduino, that's only 2 and 3
Encoder encoder(2, 3);

void setup()
{
  Serial.begin(57600);
  
  pinMode(motorStepPin, OUTPUT);
  pinMode(motorDirPin, OUTPUT);
  pinMode(motorEnablePin, OUTPUT);
  digitalWrite(motorEnablePin, HIGH);

  Timer1.initialize(1000);
  Timer1.attachInterrupt(doStep);
}

void loop()
{
  float maxspeed;
  long mindelay;
  long pos1;
  char c;
  
  if (Serial.available()) {
    c = Serial.read();
    if (c == 'f') {
      sinefreq = Serial.parseFloat();
      sineamp = Serial.parseFloat();
      
      Serial.print("Freq: ");
      Serial.println(sinefreq);
      
      Serial.print("Amp: ");
      Serial.println(sineamp);
      
      sinestep = sineamp/360 * stepsperrev / 127; //sineamp/360 * stepsperrev / 127;
      d = 256/(4*sinestep);
  
      mindelay = 1000000/sinefreq/(65536/d);
      Serial.print("Min delay (us): ");
      Serial.println(mindelay);
          
      Serial.print("d: ");
      Serial.println(d);
      
      Serial.print("sinestep: ");
      Serial.println(sinestep);
  
      if ((mindelay <= 10) && (4*sinestep > 256)) {
        Serial.println("Error: movement too fast");
        sinefreq = 0;
      }
      else {
        Timer1.setPeriod(mindelay);
      }
    }
    else if (c == 'r') {
      encpos = encoder.read();
      Serial.write((const uint8_t *)&encpos,4);
      Serial.write(42);
    }
    else {
      delay(10);
    }
  }
}

void doStep()
{
  int a, b;
  unsigned char ind;
  int mid;
  int ab;
  long newpos;
  
  if (sinefreq > 0) {
    //ind = count >> 8;
    //mid = count % 256;
    ind = count >> 8;
    mid = count % 256;
    
    ab = pgm_read_word_near(sine256 + ind);    
    a = lowByte(ab);
    b = highByte(ab);
    
    newpos = (a-127)*sinestep + (b-a)*mid*sinestep/256;
    if (newpos > cmdpos) {
      // Serial.println(newpos);
      digitalWrite(motorStepPin, HIGH);
      digitalWrite(motorStepPin, LOW);
      digitalWrite(motorDirPin, HIGH);
    }
    else if (newpos < cmdpos) {
      // Serial.println(newpos);
      digitalWrite(motorStepPin, HIGH);
      digitalWrite(motorStepPin, LOW);
      digitalWrite(motorDirPin, LOW);
    }
    cmdpos = newpos;    
    count += d;
  }
}

