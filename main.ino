#include <Servo.h>
#include <CheapStepper.h>

CheapStepper stepper (4, 5, 6, 7);
Servo servoLockWallet;
Servo servoSplitWallet;
int triggerPin = 3;

void setup() {
  Serial.begin(9600);
  pinMode(triggerPin, INPUT); 
  servoLockWallet.attach(8);
  servoSplitWallet.attach(9);
  test();
}

void loop() {
  if(triggered()) {
    lockWallet();
    splitWallet();
    extractEdge();
    extractWallet();
    reset();
  }
}

bool triggered() {
  int value = digitalRead(triggerPin);
  Serial.println(value);
  return (value == HIGH);
}

void extractEdge() {
  // do something clever
}

void extractWallet() {
  stepper.moveDegrees(true, 360 * 3.5);
}

void splitWallet() {
  servoSplitWallet.write(0);
  delay(500);
  servoSplitWallet.write(45);
  delay(500);
}

void lockWallet() {
  servoLockWallet.write(0);
  delay(500);
}

void reset() {
  servoSplitWallet.write(90);
  servoLockWallet.write(160);
  delay(500);
}

void test() {
  lockWallet();
  splitWallet();
  reset();
  delay(1000);
}
