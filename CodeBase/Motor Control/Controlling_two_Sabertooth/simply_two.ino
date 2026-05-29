//this code is the simplest to run simultaneously two sabertooth = 4 motors, usingplain text serial (check pg-23 of documentation for dip switch settings)
//this uses different tx for both therefore no address specification is needed.


void setup() {
  Serial2.begin(9600);  // Sabertooth 2
  Serial3.begin(9600);  // Sabertooth 1
  delay(2000);
}

void loop() {
  // Sabertooth 1
  Serial3.print("M1: 1000\r\n");
  Serial3.print("M2: 1000\r\n");
  
  // Sabertooth 2
  Serial2.print("M1: 1000\r\n");
  Serial2.print("M2: 1000\r\n");
  
  delay(3000);
  
  // Stop all
  Serial3.print("M1: 0\r\n");
  Serial3.print("M2: 0\r\n");
  Serial2.print("M1: 0\r\n");
  Serial2.print("M2: 0\r\n");
  
  delay(2000);
}
