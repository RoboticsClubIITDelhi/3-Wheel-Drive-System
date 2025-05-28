// 4-Channel RC Verification Code for Arduino Mega
#include <Kangaroo.h>
#define RC_CH2_PIN 4   // Channel 2
#define RC_CH3_PIN 5   // Channel 3
#define RC_CH4_PIN 6   // Channel 4
// Use Serial1 for Mega (TX1=18, RX1=19)
KangarooSerial K(Serial1);
KangarooChannel K2(K, '1');  // Motor 1 (Sabertooth channel 1)
KangarooChannel K3(K, '2');  // Motor 2 (Sabertooth channel 2)
// Motor speed limits (Kangaroo/Sabertooth: -127 to 127)
const int MOTOR_MIN = -127;
const int MOTOR_MAX = 127;

unsigned long pulse_ch2, pulse_ch3, pulse_ch4;

void setup() {
  Serial.begin(9600);
  Serial1.begin(9600); 
  pinMode(RC_CH4_PIN, INPUT);
  pinMode(RC_CH2_PIN, INPUT);
  pinMode(RC_CH3_PIN, INPUT);
  K2.start();
  K3.start();
  K3.home().wait();
  K2.home().wait();
  Serial.println("RC 3-Channel Verification Started (Mega, pins 4, 5, 6)");
  Serial.println("All good to go");
}

void loop() {
  // Read raw PWM pulse widths (microseconds)
  pulse_ch4 = pulseIn(RC_CH4_PIN, HIGH, 30000);
  pulse_ch2 = pulseIn(RC_CH2_PIN, HIGH, 30000);
  pulse_ch3 = pulseIn(RC_CH3_PIN, HIGH, 30000);

  // Calculate normalized values (-1.0 to 1.0)
  float vx = 0.0, w = 0.0, vy = 0.0;
  if (pulse_ch4 >= 950 && pulse_ch4 <= 2000) vx = (pulse_ch4 - 1500.0)/500.0;
  if (pulse_ch2 >= 950 && pulse_ch2 <= 2000) w = (pulse_ch2 - 1500.0)/500.0;
  if (pulse_ch3 >= 950 && pulse_ch3 <= 2000) vy = (pulse_ch3 - 1500.0)/500.0;

  // Print formatted output
  Serial.print("CH2: "); Serial.print(pulse_ch2); Serial.print("μs ("); Serial.print(w); Serial.print(") | ");
  Serial.print("CH3: "); Serial.print(pulse_ch3); Serial.print("μs ("); Serial.print(vy); Serial.print(") | ");
  Serial.print("CH4: "); Serial.print(pulse_ch4); Serial.print("μs ("); Serial.print(vx); Serial.println(")");

  // Signal loss check
  if (pulse_ch2 < 900 || pulse_ch3 < 900 || pulse_ch4 < 900) {
    Serial.println("! WARNING: Signal loss detected !");
  } else if (abs(w) < 0.1 && abs(vy) < 0.1 && abs(vx) < 0.1) {
    K3.s(0); K2.s(0);
    Serial.println(" | Motors: Stopped (deadzone)");
    delay(20);
  } else {
    //w1=2*vx+w;
    float w2=(vx+vy+w)*200;
    float w3=(vx-vy+w)*200;
    K3.s(w3);K2.s(w2);
    Serial.println("Joystick(s) active: Ready for command.");
    delay(20);
  }

  delay(200); // Reduce serial spam
}