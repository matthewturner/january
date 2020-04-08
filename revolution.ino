const byte interruptPin = 2;
volatile byte state = LOW;
volatile int counter = 0;
int timeSinceStart = 0;
float maxSpeed = 0.0f;
int blockCount = 0;
int blockSize = 5;
int chunkSize = 2;
int lastBlockCount = 0;

void setup() {
  Serial.begin(9600);
  pinMode(interruptPin, INPUT);
  attachInterrupt(digitalPinToInterrupt(interruptPin), onChange, CHANGE);
}

void loop() {
  delay(1000 * chunkSize);
  timeSinceStart += chunkSize;
  blockCount++;

  if (blockCount % blockSize != 0) {
    return;
  }
  
  Serial.print("Current counter: ");
  Serial.println(counter);

  int delta = counter - lastBlockCount;
  float currentSpeed = delta / (blockSize * chunkSize * 1.0f) * 60;
  lastBlockCount = counter;
  Serial.print("Current speed: ");
  Serial.println(currentSpeed);

  if (currentSpeed > maxSpeed) {
    maxSpeed = currentSpeed;
  }
  Serial.print("Max speed:     ");
  Serial.println(maxSpeed);
  
  float avgSpeed = counter / (timeSinceStart * 1.0f) * 60;
  Serial.print("Average speed: ");
  Serial.println(avgSpeed);
}

void onChange() {
  state = digitalRead(interruptPin);
  if (state == HIGH) {
    counter++;
  }
}
