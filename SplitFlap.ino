#include <Stepper.h>

// Hall Effect Sensor
#define MINUTE_SENSOR A1
#define HOUR_SENSOR A2

// Steps it takes to complete a revolution.
const int stepPerRevolution = 2048;
 
// 28BYJ-48 Speed variable.
const int optimumSpeed = 15;

// Flap Rotation variable.
int Once = 204.86;
int Tens = -341.33;
int Hour = -170.66;

// Timer
long lastTime = millis();

// This will help in rotating the Left Flap
bool shouldRotate = false;
bool shouldRotateHour = false;
bool hourRotated = false;
bool minuteRotated = false;

// Connection to the motors
Stepper MinuteOnce(stepPerRevolution, 6, 8, 7, 9);
Stepper MinuteTens(stepPerRevolution, 10, 12, 11, 13);
Stepper MinuteHour(stepPerRevolution, 2, 4, 3, 5);

void setup() {
  // PinModes
  pinMode(MINUTE_SENSOR, INPUT);
  pinMode(HOUR_SENSOR, INPUT);

  // Settings an optimum speed for both Motors.
  MinuteOnce.setSpeed(optimumSpeed);
  MinuteTens.setSpeed(optimumSpeed);
  MinuteHour.setSpeed(optimumSpeed);
  
  // Starting the Serial EVENT
  Serial.begin(9600);
}

void loop() {
  // elapsedTimer variable
  long elapsedTimer = millis() - lastTime;
  lastTime = elapsedTimer + lastTime;

  rotateMinuteFlapRight(elapsedTimer);

  shouldRotateHour = readSensor(2);   
  shouldRotate = readSensor(1);

  // It checks for hall sensor output
  if(shouldRotate && !minuteRotated){
    rotateMinuteFlapLeft();
  }
  if(shouldRotateHour && !hourRotated){
    rotateHour();
  }
  
  if(hourRotated){
    resetHour(elapsedTimer);
  } 
  if(minuteRotated){
    resetMinute(elapsedTimer);
  }
}

// This rotates the Right Flap
void rotateMinuteFlapRight(long elapsedMS){
  static long rotationTime = 0;
  rotationTime = rotationTime + elapsedMS;

  // Rotate the flap after 1 Minute = 60000 Milli Seconds
  if(rotationTime >= 60000){
    rotationTime = rotationTime - 60000;
    MinuteOnce.step(Once);
  }
}

// This rotates the Left Flap
void rotateMinuteFlapLeft(){
  MinuteTens.step(Tens);
  shouldRotate = false;
  minuteRotated = true;
}

// This rotates the Hour Flap
void rotateHour(){
  MinuteHour.step(Hour);
  shouldRotateHour = false;
  hourRotated = true;
}

void resetHour(long elapsedMS){
  static long rotationTime = 0;
  rotationTime = rotationTime + elapsedMS;
  if(rotationTime >= 600000){
    hourRotated = false;    
    rotationTime = rotationTime - 600000;  
  }
}

void resetMinute(long elapsedMS){
  static long rotationTime = 0;
  rotationTime = rotationTime + elapsedMS;
    if(rotationTime >= 540000){
      minuteRotated = false;
      rotationTime = rotationTime - 540000;
    }
}

bool readSensor(int num){
  int output;
  // Reads the sensor
  switch(num){
    case 1:
      output = digitalRead(MINUTE_SENSOR);
      break;
    case 2:
      output = digitalRead(HOUR_SENSOR);
      break;
  }
  

  // Return true/false based on output
  switch (output){
    case 0:
      return true;
      break;
    case 1:
      return false;
      break;
  }
}