// 4-Channel RC Verification Code for Arduino Mega
#define RC_CH1_PIN 3   // Channel 1
#define RC_CH2_PIN 4   // Channel 2
#define RC_CH3_PIN 5   // Channel 3
#define RC_CH4_PIN 6   // Channel 4

unsigned long pulse_ch1, pulse_ch2, pulse_ch3, pulse_ch4;

void setup() {
  Serial.begin(9600);
  pinMode(RC_CH1_PIN, INPUT);
  pinMode(RC_CH2_PIN, INPUT);
  pinMode(RC_CH3_PIN, INPUT);
  pinMode(RC_CH4_PIN, INPUT);
  Serial.println("RC 4-Channel Verification Started (Mega, pins 3, 4, 5, 6)");
}

void loop() {
  // Read raw PWM pulse widths (microseconds)
  pulse_ch1 = pulseIn(RC_CH1_PIN, HIGH, 30000);
  pulse_ch2 = pulseIn(RC_CH2_PIN, HIGH, 30000);
  pulse_ch3 = pulseIn(RC_CH3_PIN, HIGH, 30000);
  pulse_ch4 = pulseIn(RC_CH4_PIN, HIGH, 30000);

  // Calculate normalized values (-1.0 to 1.0)
  float ch1 = 0.0, ch2 = 0.0, ch3 = 0.0, ch4 = 0.0;
  if (pulse_ch1 >= 1000 && pulse_ch1 <= 2000) ch1 = (pulse_ch1 - 1500) / 500.0;
  if (pulse_ch2 >= 1000 && pulse_ch2 <= 2000) ch2 = (pulse_ch2 - 1500) / 500.0;
  if (pulse_ch3 >= 1000 && pulse_ch3 <= 2000) ch3 = (pulse_ch3 - 1500) / 500.0;
  if (pulse_ch4 >= 1000 && pulse_ch4 <= 2000) ch4 = (pulse_ch4 - 1500) / 500.0;

  // Print formatted output
  Serial.print("CH1: "); Serial.print(pulse_ch1); Serial.print("μs ("); Serial.print(ch1, 2); Serial.print(") | ");
  Serial.print("CH2: "); Serial.print(pulse_ch2); Serial.print("μs ("); Serial.print(ch2, 2); Serial.print(") | ");
  Serial.print("CH3: "); Serial.print(pulse_ch3); Serial.print("μs ("); Serial.print(ch3, 2); Serial.print(") | ");
  Serial.print("CH4: "); Serial.print(pulse_ch4); Serial.print("μs ("); Serial.print(ch4, 2); Serial.println(")");

  // Signal loss check
  if (pulse_ch1 < 900 || pulse_ch2 < 900 || pulse_ch3 < 900 || pulse_ch4 < 900) {
    Serial.println("! WARNING: Signal loss detected !");
  } else if (abs(ch1) < 0.1 && abs(ch2) < 0.1 && abs(ch3) < 0.1 && abs(ch4) < 0.1) {
    Serial.println("All sticks in deadzone: No movement command.");
  } else {
    Serial.println("Joystick(s) active: Ready for command.");
  }

  delay(200); // Reduce serial spam
}
