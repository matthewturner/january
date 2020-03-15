#include <Servo.h>
#include <CheapStepper.h>

CheapStepper stepperExtractor (4, 5, 6, 7);
Servo servoLockWallet;
Servo servoSplitWallet;
Servo servoExtractor;
int triggerPin = 3;

void setup() {
  Serial.begin(9600);
  pinMode(triggerPin, INPUT); 
  servoLockWallet.attach(8);
  servoSplitWallet.attach(9);
  servoExtractor.attach(2);
  // test();
}

void loop() {
  if(triggered()) {
    lockWallet();
    splitEdge();
    extractEdge();
    extractWallet();
    squeezeEdge();
    extractEdge();
    releaseEdge();
    extractWallet();
    unlockWallet();
  }
}

bool triggered() {
  int value = digitalRead(triggerPin);
  Serial.println(value);
  return (value == HIGH);
}

void extractEdge() {
  servoExtractor.write(5);
  delay(500);
  servoExtractor.write(10);
  delay(500);
  servoExtractor.write(15);
  delay(500);
  servoExtractor.write(20);
  delay(500);
  servoExtractor.write(100);
  delay(500);
}

void extractWallet() {
  stepperExtractor.moveDegrees(true, 360 * 3.5);
}

void splitEdge() {
  servoSplitWallet.write(0);
  delay(500);
  servoSplitWallet.write(90);
  delay(500);
}

void squeezeEdge() {
  servoSplitWallet.write(0);
  delay(500);
}

void lockWallet() {
  servoLockWallet.write(0);
  delay(500);
}

void releaseEdge() {
  servoSplitWallet.write(90);
  delay(500);
}

void unlockWallet() {
  servoLockWallet.write(160);
  delay(500);
}

void reset() {
  releaseEdge();
  unlockWallet();
}

void test() {
  lockWallet();
  splitEdge();
  extractEdge();
  reset();
}
