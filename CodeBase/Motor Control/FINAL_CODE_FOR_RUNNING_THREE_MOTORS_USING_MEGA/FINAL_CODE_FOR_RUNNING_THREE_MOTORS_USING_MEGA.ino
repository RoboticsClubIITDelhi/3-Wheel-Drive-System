// 4Final Code for running using Arduino Mega.
#include <Kangaroo.h>

#define RC_CH2_PIN 4   // Channel 2
#define RC_CH3_PIN 5   // Channel 3
#define RC_CH4_PIN 6   // Channel 4


// Independent mode channels on Kangaroo are, by default, '1' and '2'.

KangarooSerial Kangaroo_2(Serial2); // Serial 2: TX=2, RX=2
KangarooSerial Kangaroo_1(Serial1); // Serial 1: TX=1, RX=1
KangarooChannel wheel3(Kangaroo_2, '1');  // Motor 1 of kangaroo 2 (Sabertooth channel 1)
KangarooChannel wheel1(Kangaroo_1, '1');  // Motor 1 of kangaroo 1 (Sabertooth channel 1)
KangarooChannel wheel2(Kangaroo_1, '2');  // Motor 2 of kangaroo 1 (Sabertooth channel 1)
unsigned long pulse_ch2, pulse_ch3, pulse_ch4;

void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);
  Serial2.begin(9600);
  pinMode(RC_CH4_PIN, INPUT);
  pinMode(RC_CH2_PIN, INPUT);
  pinMode(RC_CH3_PIN, INPUT);
  wheel1.start();
  wheel1.home().wait();
  wheel2.start();
  wheel2.home().wait();
  wheel3.start();
  wheel3.home().wait();
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
  } 
  else if (abs(w) < 0.1 && abs(vy) < 0.1 && abs(vx) < 0.1) {
    
    wheel1.s(0);
    wheel2.s(0);
    wheel3.s(0);
    Serial.println(" | Motors: Stopped (deadzone)");
    delay(20);
  } 
  else if (abs(w) < 0.1 && abs(vy) > 0.1 && abs(vx) < 0.1) {
    
    float w3=(0)*300;
    float w1=(vy)*300;
    float w2=(vy)*300;
    wheel1.s(w1);
    wheel2.s(w2);
    wheel3.s(w3);
    Serial.println(" | Motors: y-direction");
    delay(20);
  } 
  else if (abs(w) > 0.1 && abs(vy) < 0.1 && abs(vx) < 0.1) {
    
    float w3=(w)*300;
    float w1=(-w)*300;
    float w2=(w)*300;
    wheel1.s(w1);
    wheel2.s(w2);
    wheel3.s(w3);
    Serial.println(" | Motors: rotating");
    delay(200);
  } 
  else if (abs(w) < 0.1 && abs(vy) < 0.1 && abs(vx) > 0.1) {
    
    float w3=(2*vx)*300;
    float w1=(vx)*300;
    float w2=(-vx)*300;
    wheel1.s(w1);
    wheel2.s(w2);
    wheel3.s(w3);
    Serial.println(" | Motors: x-direction");
    delay(200);
  } 
  else {
    float w3=(2*vx+w)*300;
    float w1=(vx+vy+w)*300;
    float w2=(vx-vy+w)*300;
    wheel1.s(w1);
    wheel2.s(w2);
    wheel3.s(w3);
    Serial.println(" | Motors: random motion");
    delay(200);
  }

  delay(200); // Reduce serial spam
}
