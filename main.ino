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
Servo servoSlider;
Servo servoFilter;
UltraSonicDistanceSensor sensorExtractor (39, 37);
int pinWalletsAreAvailableInFeeder = 31;
int pinWalletIsAligned = 53;

void setup() {
  Serial.begin(9600);
  pinMode(pinWalletsAreAvailableInFeeder, INPUT);
  pinMode(pinWalletIsAligned, INPUT);
  servoLockWallet.attach(8);
  servoSplitWallet.attach(9);
  servoExtractor.attach(2);
  servoSlider.attach(25);
  servoGripperLeft.attach(46);
  servoGripperRight.attach(47);
  servoFilter.attach(30);
  reset();
}

void loop() {
  // testGrip();
  // testAlignment();
  
  if (walletIsInExtractor()) {
    processWallet();
    return;
  }

  if (walletsAreAvailableInFeeder()) {
    feedWallet();
    return;
  }
  
  Serial.println("Nothing is available.");
  delay(2000);
}

void processWallet() {
  // adjust position of the wallet
  slideGripperIn();
  clearGrip();
  swingGripperIn();
  while (!walletIsAligned()) {
    swingGripperTo(50);
    swingGripperIn();
  }
  lockWallet();
  swingGripperOut();

  // extract paper
  filterWallet();
  squeezeEdge();
  grip();
  slideGripperOut();
  swingGripperIn();
  ungrip();
  separateEdge();
  slideGripperIn();
  grip();
  slideGripperOut();
  catchEdge();
  ungrip();
  swingGripperOut();
  filterPaper();
  extractWallet();

  //extract empty wallet
  filterWallet();
  squeezeEdge();
  swingGripperIn();
  clearGrip();
  slideGripperIn();
  grip();
  releaseEdge();
  unlockWallet();
  slideGripperOut();
  catchEdge();
  ungrip();
  swingGripperOut();
  filterWallet();
  extractWallet();
}

void testGrip() {
  swingGripperIn();
  slideGripperOut();
  squeezeEdge();
  separateEdge();
  grip();
  ungrip();
  delay(2000);
}

void testAlignment() {
  walletIsAligned();
  delay(1000);
}

void feedWallet() {
 Serial.println("Feeding wallet (~5cm)...");
 stepperFeeder.moveDegrees(true, 360);
}

bool walletsAreAvailableInFeeder() {
  Serial.println("Checking if wallets are available in feeder...");
  bool inFeeder = (digitalRead(pinWalletsAreAvailableInFeeder) == HIGH);
  if (inFeeder) {
    Serial.println("  Yes :-)");
  } else {
    Serial.println("  No :-(");
  }
  return inFeeder;
}

bool walletIsInExtractor() {
  Serial.println("Checking if wallet is in extractor...");
  double distance = sensorExtractor.measureDistanceCm();
  Serial.println(distance);
  bool inExtractor = (distance <= 10);
  if (inExtractor) {
    Serial.println("  Yes :-)");
  } else {
    Serial.println("  No :-(");
  }
  return inExtractor;
}

bool walletIsAligned() {
  Serial.println("Checking if wallet is aligned...");
  bool isAligned = (digitalRead(pinWalletIsAligned) == LOW);
  if (isAligned) {
    Serial.println("  Yes :-)");
  } else {
    Serial.println("  No :-(");
  }
  return isAligned;
}

void swingGripperIn() {
  Serial.println("Swinging gripper in...");
  swingGripperTo(0);
}

void swingGripperOut() {
  Serial.println("Swinging gripper out...");
  swingGripperTo(100);
}

void swingGripperTo(int percentageFromIn) {
  Serial.println("Swinging gripper to...");
  int position = (90 / 100.0f * percentageFromIn) + 5;
  servoExtractor.write(position);
  delay(500);
}

void slideGripperOut() {
  Serial.println("Sliding gripper away...");
  servoSlider.write(0);
  delay(500);
}

void slideGripperIn() {
  Serial.println("Sliding gripper back...");
  servoSlider.write(130);
  delay(500);
}

void extractWallet() {
  Serial.println("Extracting wallet...");
  stepperExtractor.moveDegrees(true, 360 * 3.5);
}

void catchEdge() {
  Serial.println("Catching edge...");
  stepperExtractor.moveDegrees(true, 20);
}

void squeezeEdge() {
  Serial.println("Squeezing edge...");
  servoSplitWallet.write(0);
  delay(500);
}

void separateEdge() {
  Serial.println("Separating edge...");
  servoSplitWallet.write(60);
  delay(500);
}

void lockWallet() {
  Serial.println("Locking wallet...");
  servoLockWallet.write(0);
  delay(500);
}

void releaseEdge() {
  Serial.println("Releasing edge...");
  servoSplitWallet.write(90);
  delay(1500);
}

void unlockWallet() {
  Serial.println("Unlocking wallet...");
  servoLockWallet.write(160);
  delay(500);
}

void grip() {
  Serial.println("Gripping...");
  setGrip(0);
}

void ungrip() {
  Serial.println("Ungripping...");
  setGrip(10);
}

void clearGrip() {
  Serial.println("Clearing grip...");
  setGrip(40);
}

void setGrip(int degrees) {
  servoGripperLeft.write(80 - degrees);
  servoGripperRight.write(0 + degrees);
  delay(500);
}

void filterPaper() {
  Serial.println("Filtering paper...");
  servoFilter.write(0);
  delay(500);
}

void filterWallet() {
  Serial.println("Filtering wallet...");
  servoFilter.write(50);
  delay(500);
}

void reset() {
  Serial.println("Resetting...");
  clearGrip();
  releaseEdge();
  unlockWallet();
  slideGripperIn();
  filterWallet();
}
