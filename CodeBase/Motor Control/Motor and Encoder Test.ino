// Motor test with Arduino UNO and Cytron MDD10A
// Motor: RMCS 2083, 18V, 250W, tested at 12V, 4A peak current limitation
// Motor rotates in one direction for a specified time and then in other direction for the same time
// Encoder measures the angle of rotation during these rotations


const int pwmPin = 6;  // PWM control (speed)
const int dirPin = 7;  // Direction control

volatile long encoderCount = 0;

const int encoderPinA = 2; // Interrupt pin (INT0)
const int encoderPinB = 4; // Direction sensing pin (can be digital)

const int CPR = 1196; // Your encoder's Counts Per Revolution

void setup() {
  pinMode(pwmPin, OUTPUT);
  pinMode(dirPin, OUTPUT);

  pinMode(encoderPinA, INPUT);
  pinMode(encoderPinB, INPUT);

  attachInterrupt(digitalPinToInterrupt(encoderPinA), handleEncoder, RISING); // Or CHANGE

  Serial.begin(9600);
}

void loop() {

  static unsigned long lastPrint = 0;
  if (millis() - lastPrint > 50) {
    lastPrint = millis();
    
    digitalWrite(dirPin, HIGH);    // Set direction
    analogWrite(pwmPin, 50);      // Speed (0–255)
    delay(500);                   // Run for 5 seconds

    // Stop motor
    analogWrite(pwmPin, 0);
    delay(1000);

    // Run motor reverse
    digitalWrite(dirPin, LOW);     // Reverse direction
    analogWrite(pwmPin, 50);      // Same speed
    delay(500);

    // Stop moto
    analogWrite(pwmPin, 0);
    delay(1000);

    // Read encoderCount safely
    noInterrupts();
    long count = encoderCount;
    interrupts();

    // Convert count to angle
    float angle = ((float)count / CPR) * 360.0;

    Serial.print("Angle: ");
    Serial.println(angle, 2); // Two decimal places
  }
}

void handleEncoder() {
  // Determine direction based on B signal
  if (digitalRead(encoderPinB) == HIGH) {
    encoderCount--;
  } else {
    encoderCount++;
  }
}
