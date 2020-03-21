#include <Servo.h>
#include <CheapStepper.h>
#include <HCSR04.h>

CheapStepper stepperExtractor (4, 5, 6, 7);
CheapStepper stepperFeeder (22, 24, 26, 28);
Servo servoLockWallet;
Servo servoSplitWallet;
Servo servoExtractor;
Servo servoGripperLeft;
Servo servoGripperRight;
HCSR04 sensorExtractor (39, 37);
int pinWalletsAreAvailableInFeeder = 20;

void setup() {
  Serial.begin(9600);
  pinMode(pinWalletsAreAvailableInFeeder, INPUT);
  servoLockWallet.attach(8);
  servoSplitWallet.attach(9);
  servoExtractor.attach(2);
  servoGripperLeft.attach(46);
  servoGripperRight.attach(10);
  reset();
}

void loop() { 
  if(!walletsAreAvailableInFeeder() && !walletIsInExtractor()) {
    return;
  }

  while (!walletIsInExtractor()) {
    feedWallet();
  }

  // adjust position of the wallet
  slideGripperBack();
  clearGrip();
  swingGripperIn();
  swingGripperOut();

  // extract paper
  lockWallet();
  squeezeEdge();
  separateEdge();
  slideGripperAway();
  ungrip();
  swingGripperIn();
  slideGripperBack();
  grip();
  slideGripperAway();
  catchEdge();
  ungrip();
  swingGripperOut();
  extractWallet();

  //extract empty wallet
  squeezeEdge();
  swingGripperIn();
  grip();
  slideGripperBack();
  releaseEdge();
  unlockWallet();
  slideGripperAway();
  catchEdge();
  ungrip();
  swingGripperOut();
  extractWallet();
}

void feedWallet() {
 stepperFeeder.moveDegrees(true, 360);
}

bool walletsAreAvailableInFeeder() {
  int value = digitalRead(pinWalletsAreAvailableInFeeder);
  Serial.println(value);
  // return (value == HIGH);
  return true;
}

bool walletIsInExtractor() {
  float value = sensorExtractor.dist();
  Serial.println(value);
  return (value <= 10);
}

void swingGripperIn() {
  servoExtractor.write(5);
  delay(500);
}

void slideGripperAway() {
  // do nothing yet
}

void slideGripperBack() {
  // do nothing yet
}

void swingGripperOut() {
  servoExtractor.write(100);
  delay(500);
}

void extractWallet() {
  stepperExtractor.moveDegrees(true, 360 * 3.5);
}

void catchEdge() {
  stepperExtractor.moveDegrees(true, 20);
}

void squeezeEdge() {
  servoSplitWallet.write(0);
  delay(500);
}

void separateEdge() {
  servoSplitWallet.write(90);
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

void grip() {
  setGrip(0);
}

void ungrip() {
  setGrip(15);
}

void clearGrip() {
  setGrip(50);
}

void setGrip(int degrees) {
  servoGripperLeft.write(80 - degrees);
  servoGripperRight.write(0 + degrees);
  delay(500);
}

void reset() {
  clearGrip();
  releaseEdge();
  unlockWallet();
  slideGripperBack();
}
