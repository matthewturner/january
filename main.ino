#include <Servo.h>
#include <CheapStepper.h>

CheapStepper stepperExtractor (4, 5, 6, 7);
Servo servoLockWallet;
Servo servoSplitWallet;
Servo servoExtractor;
Servo servoGripperLeft;
Servo servoGripperRight;
int triggerPin = 3;

void setup() {
  Serial.begin(9600);
  pinMode(triggerPin, INPUT); 
  servoLockWallet.attach(8);
  servoSplitWallet.attach(9);
  servoExtractor.attach(2);
  servoGripperLeft.attach(13);
  servoGripperRight.attach(10);
  reset();
}

void loop() {  
  if(triggered()) {
    moveToExtractEdge();
    moveToClearEdge();
    lockWallet();
    splitEdge();
    moveToExtractEdge();
    gripEdge();
    extractEdge();
    catchEdge();
    ungripEdge();
    moveToClearEdge();
    extractWallet();
    squeezeEdge();
    moveToExtractEdge();
    gripEdge();
    releaseEdge();
    unlockWallet();
    extractEdge();
    catchEdge();
    ungripEdge();
    moveToClearEdge();
    extractWallet();
  }
}

bool triggered() {
  int value = digitalRead(triggerPin);
  Serial.println(value);
  return (value == HIGH);
}

void moveToExtractEdge() {
  ungripEdge();
  servoExtractor.write(5);
  delay(500);
}

void extractEdge() {
  for (int i = 10; i <= 35; i += 5) {
    servoExtractor.write(i);
    delay(500);
  }
}

void moveToClearEdge() {
  servoExtractor.write(100);
  delay(500);
}

void extractWallet() {
  stepperExtractor.moveDegrees(true, 360 * 3.5);
}

void catchEdge() {
  stepperExtractor.moveDegrees(true, 20);
}

void splitEdge() {
  squeezeEdge();
  separateEdge();
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

void gripEdge() {
  servoGripperLeft.write(88);
  servoGripperRight.write(2);
  delay(500);
}

void ungripEdge() {
  servoGripperLeft.write(80);
  servoGripperRight.write(20);
  delay(500);
}

void reset() {
  releaseEdge();
  unlockWallet();
  ungripEdge();
  separateEdge();
}

void test() {
  lockWallet();
  splitEdge();
  extractEdge();
  reset();
}
