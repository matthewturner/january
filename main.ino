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

void setup() {
  Serial.begin(9600);
  pinMode(pinWalletsAreAvailableInFeeder, INPUT);
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
  slideGripperBack();
  clearGrip();
  swingGripperIn();
  lockWallet();
  swingGripperOut();

  // extract paper
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
  filterPaper();
  extractWallet();

  //extract empty wallet
  squeezeEdge();
  clearGrip();
  swingGripperIn();
  slideGripperBack();
  grip();
  releaseEdge();
  unlockWallet();
  slideGripperAway();
  catchEdge();
  ungrip();
  swingGripperOut();
  filterWallet();
  extractWallet();
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

void swingGripperIn() {
  Serial.println("Swinging gripper in...");
  servoExtractor.write(5);
  delay(500);
}

void slideGripperAway() {
  Serial.println("Sliding gripper away...");
  servoSlider.write(0);
  delay(500); 
}

void slideGripperBack() {
  Serial.println("Sliding gripper back...");
  servoSlider.write(130);
  delay(500);
}

void swingGripperOut() {
  Serial.println("Swinging gripper out...");
  servoExtractor.write(100);
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
  servoSplitWallet.write(90);
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
  delay(500);
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
  setGrip(50);
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
  slideGripperBack();
  filterPaper();
}
