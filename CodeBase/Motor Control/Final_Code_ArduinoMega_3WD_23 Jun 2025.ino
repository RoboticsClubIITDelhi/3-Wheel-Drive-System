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
unsigned long pulse_ch2, pulse_ch3, pulse_ch4, signal_check;

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
  // Read raw PWM pulse widths (microseconds) alongside the checking for loss.
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
  bool constraint_var = false;
  if (abs(vx) < 0.1) {
    vx = 0;
    Serial.println("Movement in x-direction is restricted |");
    constraint_var = true;
  }
  if (abs(vy) < 0.1) {
    vy = 0;
    Serial.println("Movement in y-direction is restricted |");
    constraint_var = true;
  }
  if (abs(w) < 0.1) {
    w = 0;
    Serial.println("Rotation is restricted |");
    constraint_var = true;
  }
  if (constraint_var == true) {
    Serial.println("Robot in DEADLOCK!! |");
    delay(200);
  }
  //Assigning wheels speeds from the input
  float q = 1; // Depends on your pivoting speed(Should try and adjust it)
  float w1 = 125*((0.5*vx) - (0.867*vy) - (q*w));
  float w2 = 125*(((-0.5)*vx) - (0.867*vy) + (q*w));
  float w3 = 125*(vx + (q*w));

  //Controlling the wheels
  wheel1.s(w1);
  wheel2.s(w2);
  wheel3.s(w3);
  if (constraint_var == true) {
    Serial.println("Robot under motion with constraints displayed above");  
  } else {
    Serial.println("Robot under arbitrary motion");
  }
  delay(200);
}