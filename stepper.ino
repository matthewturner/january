#include <CheapStepper.h>

CheapStepper stepper;

void setup() { 
  Serial.begin(9600);
}

int turns = 0;
void loop() {
  stepper.moveDegrees(true, 360);
  turns++;
  Serial.println(turns);
}
