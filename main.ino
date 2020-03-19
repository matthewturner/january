#include <Servo.h>
#include <CheapStepper.h>

CheapStepper stepperExtractor (4, 5, 6, 7);
CheapStepper stepperFeeder (22, 24, 26, 28);
Servo servoLockWallet;
Servo servoSplitWallet;
Servo servoExtractor;
Servo servoGripperLeft;
Servo servoGripperRight;
int pinWalletIsInExtractor = 3;
int pinWalletsAreAvailableInFeeder = 20;

void setup() {
  Serial.begin(9600);
  pinMode(pinWalletIsInExtractor, INPUT);
  pinMode(pinWalletsAreAvailableInFeeder, INPUT);
  servoLockWallet.attach(8);
  servoSplitWallet.attach(9);
  servoExtractor.attach(2);
  servoGripperLeft.attach(13);
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
  ungrip();
  swingGripperIn();
  swingGripperOut();

  // extract paper
  lockWallet();
  squeezeEdge();
  separateEdge();
  swingGripperIn();
  grip();
  slideGripperAway();
  catchEdge();
  ungrip();
  swingGripperOut();
  extractWallet();

  //extract empty wallet
  slideGripperBack();
  squeezeEdge();
  swingGripperIn();
  grip();
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
  int value = digitalRead(pinWalletIsInExtractor);
  Serial.println(value);
  return (value == HIGH);
}

void swingGripperIn() {
  servoExtractor.write(5);
  delay(500);
}

void slideGripperAway() {
  for (int i = 10; i <= 35; i += 5) {
    servoExtractor.write(i);
    delay(500);
  }
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
  servoGripperLeft.write(88);
  servoGripperRight.write(2);
  delay(500);
}

void ungrip() {
  servoGripperLeft.write(80);
  servoGripperRight.write(20);
  delay(500);
}

void reset() {
  releaseEdge();
  unlockWallet();
  ungrip();
  slideGripperBack();
}
