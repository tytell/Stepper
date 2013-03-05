int motorStepPin = 2;
int motorDirPin = 4;
int motorEnablePin = 3;
int stepsperrev = 6400;
unsigned long outputrate = 100000;
unsigned long sampledelay;

float spinrate = 0;
float samplesperstep = 0;
unsigned int cursamp = 0;

void setup()
{
  Serial.begin(9600);
  
  pinMode(motorStepPin, OUTPUT);
  pinMode(motorDirPin, OUTPUT);
  pinMode(motorEnablePin, OUTPUT);
  digitalWrite(motorEnablePin, HIGH);
  
  sampledelay = long(1000000/outputrate);
}

void loop()
{
  float rate1;
  
  if (Serial.available()) {
    spinrate = Serial.parseFloat();
    samplesperstep = (outputrate / (abs(spinrate) * stepsperrev)) + 0.5;
    Serial.print("Output delay (us): ");
    Serial.println(sampledelay);
    Serial.print("samplesperstep: ");
    Serial.println(samplesperstep);
    if (samplesperstep <= 1) {
      Serial.println("Error: movement too fast");
      samplesperstep = 0;
    }
  }
  
  if (spinrate > 0) {
    cursamp++;
    if (cursamp >= samplesperstep) {
      cursamp = 0;
      digitalWrite(motorStepPin, HIGH);
    }
    else {
      digitalWrite(motorStepPin, LOW);
    }    
    delayMicroseconds(sampledelay);
  }
}
      
    
  
    
  


