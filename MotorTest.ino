#include <Stepper.h>

int stepPerRevolution = 2038;
int sensorPin = A2;

Stepper MinuteOnce(stepPerRevolution, 6, 8, 7, 9);

void setup() {
  pinMode(sensorPin, INPUT);
  MinuteOnce.setSpeed(15);
  Serial.begin(9600);
}

void loop() {
  MinuteOnce.step(-203.8);   
  Serial.print("Should Rotate");
  Serial.println(digitalRead(sensorPin));  
  delay(1000);          
}
