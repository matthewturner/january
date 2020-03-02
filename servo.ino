#include <Servo.h>

Servo myservo;

int pos = 0;

void setup() {
  myservo.attach(9);
}

void loop() {
  myservo.write(0);
  delay(1000);

  myservo.write(180);
  delay(1000);
}
