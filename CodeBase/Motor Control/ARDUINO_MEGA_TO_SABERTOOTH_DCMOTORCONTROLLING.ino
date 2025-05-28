#include <Kangaroo.h>

// Use Serial1 for Mega (TX1=18, RX1=19)
KangarooSerial K(Serial1);
KangarooChannel K1(K, '1');  // Motor 1 (Sabertooth channel 1)
KangarooChannel K2(K, '2');  // Motor 2 (Sabertooth channel 2)

// You can change these values to set different speeds
int speed1 = 60;  // Range: -127 (full reverse) to +127 (full forward)
int speed2 = 60;  // Range: -127 (full reverse) to +127 (full forward)
int runTime = 3000; // ms to run in each direction

void setup() {
  Serial.begin(9600);      // For debugging
  Serial1.begin(9600);     // For Kangaroo/Sabertooth

  // Start and home Kangaroo channels
  K1.start();
  K2.start();
  K1.home().wait();
  K2.home().wait();

  Serial.println("Kangaroo/Sabertooth test: Both motors will run forward and backward in a loop.");
}

void loop() {
  // Forward
  Serial.print("Forward: ");
  Serial.print(speed1);
  Serial.print(" ");
  Serial.println(speed2);
  K1.s(speed1);  // Set speed for Motor 1
  K2.s(speed2);  // Set speed for Motor 2
  delay(runTime);

  // Stop
  Serial.println("Stop");
  K1.s(0);
  K2.s(0);
  delay(1000);

  // Reverse
  Serial.print("Reverse: ");
  Serial.print(-speed1);
  Serial.print(" ");
  Serial.println(-speed2);
  K1.s(-speed1); // Reverse Motor 1
  K2.s(-speed2); // Reverse Motor 2
  delay(runTime);

  // Stop
  Serial.println("Stop");
  K1.s(0);
  K2.s(0);
  delay(1000);
}
