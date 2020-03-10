#include <Servo.h>
#include <CheapStepper.h>

CheapStepper stepper (4, 5, 6, 7);
Servo servoFixWallet;
Servo servoSplitWallet;

void setup() {
  Serial.begin(9600);
  servoFixWallet.attach(9);
  servoSplitWallet.attach(12);
}

void loop() {
  stepper.moveDegrees(true, 360);
  
  servoFixWallet.write(0);
  delay(1000);

  servoFixWallet.write(180);
  delay(1000);
}
