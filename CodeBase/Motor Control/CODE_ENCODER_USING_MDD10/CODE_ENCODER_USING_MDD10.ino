
#define dir_1 7        // Motor direction pin
#define pwm_1 6        // Motor PWM pin
#define switch_1 13    // Direction switch

#define encoderA 2     // Encoder Channel A (interrupt pin)
#define encoderB 3     // Encoder Channel B

volatile long encoderCount = 0;
unsigned long lastSpeedCalcTime = 0;
long lastEncoderCount = 0;

const int pulsesPerRevolution = 120;     // Encoder CPR after gearbox
const float supplyVoltage = 12.0;         // Actual supply voltage
const float ratedVoltage = 18.0;          // Motor's rated voltage
const float ratedMaxRPM = 250.0;          // Max RPM at rated voltage
const float maxRPM = ratedMaxRPM * (supplyVoltage / ratedVoltage);  // Adjusted for 12V = ~133.33 RPM

float angle = 0;
float actualRPM = 0;
float estimatedRPM = 0;
float rpmError = 0;

int pwmValue = 0;
String inputString = "";   // Serial input buffer
bool inputComplete = false;

void setup() {
  pinMode(pwm_1, OUTPUT);
  pinMode(dir_1, OUTPUT);
  pinMode(switch_1, INPUT);

  pinMode(encoderA, INPUT_PULLUP);
  pinMode(encoderB, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(encoderA), readEncoder, RISING);

  Serial.begin(9600);
  Serial.println("Enter PWM value (0–255) and press Enter:");
  inputString.reserve(10);
}

void loop() {
  // Handle Serial input
  if (inputComplete) {
    int val = inputString.toInt();
    if (val >= 0 && val <= 255) {
      pwmValue = val;
      Serial.print("PWM set to: ");
      Serial.println(pwmValue);
    } else {
      Serial.println("Invalid input. Please enter a number between 0 and 255.");
    }
    inputString = "";
    inputComplete = false;
  }

  // Set motor direction
  digitalWrite(dir_1, digitalRead(switch_1));

  // Apply PWM signal
  analogWrite(pwm_1, pwmValue);

  // Calculate and print speed and angle every 100 ms
  if (millis() - lastSpeedCalcTime >= 100) {
    long currentCount = encoderCount;
    long deltaCount = currentCount - lastEncoderCount;

    actualRPM = (deltaCount * 600.0) / pulsesPerRevolution;
    angle = (currentCount * 360.0) / pulsesPerRevolution;

    float dutyCycle = pwmValue / 255.0;
    estimatedRPM = dutyCycle * maxRPM;
    rpmError = estimatedRPM - actualRPM;

    lastEncoderCount = currentCount;
    lastSpeedCalcTime = millis();

    Serial.print("PWM: ");
    Serial.print(pwmValue);
    Serial.print(" | Actual RPM: ");
    Serial.print(actualRPM, 2);
    Serial.print(" | Estimated RPM: ");
    Serial.print(estimatedRPM, 2);
    Serial.print(" | Error: ");
    Serial.print(rpmError, 2);
    Serial.print(" | Angle: ");
    Serial.println(angle, 2);
  }

  delay(10);  // Small delay for stability
}

// Interrupt: Count encoder pulses
void readEncoder() {
  int b = digitalRead(encoderB);
  if (b == LOW) {
    encoderCount++;
  } else {
    encoderCount--;
  }
}

// Handle incoming serial data
void serialEvent() {
  while (Serial.available()) {
    char inChar = (char)Serial.read();
    if (inChar == '\n') {
      inputComplete = true;
    } else {
      inputString += inChar;
    }
  }
}