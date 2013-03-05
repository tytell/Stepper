int motorStepPin = 1;
int motorDirPin = 2;
int stepsperrev = 2048;
int outputrate = 10000;
int sampledelay;

float spinrate = 0;
int samplesperstep = 0;
int cursamp = 0;

void setup()
{
  Serial.begin(9600);
  
  pinMode(motorStepPin, OUTPUT);
  pinMode(motorDirPin, OUTPUT);
  
  sampledelay = int(1/outputrate * 1e6);
}

void loop()
{
  float rate1;
  
  if (Serial.available()) {
    rate1 = Serial.parseFloat();
    if (rate1 != 0) {
      spinrate = rate1;
    }
  }
  
  if (spinrate > 0) {
    samplesperstep = int(outputrate / (abs(spinrate) * stepsperrev) + 0.5);
    if (samplesperstep <= 1)
      Serial.println("Error: movement too fast");
      samplesperstep = 0;
    }
  
    cursamp++;
    if (cursamp == samplesperstep) {
      cursamp = 0;
      digitalWrite(motorStepPin, HIGH);
    }
    else {
      digitalWrite(motorStepPin, LOW);
    }
    
    if (spinrate > 0) {
      digitalWrite(motorDirPin, HIGH);
    }
    else {
      digitalWrite(motorDirPin, LOW);
    }
    
    delayMicroseconds(sampledelay);
  }
}
      
    
  
    
  


