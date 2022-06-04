int motPower[4];
bool motDirection[4];
void setup() {
  // initialize serial:
  Serial.begin(9600);
  pinMode(4, OUTPUT); // first motor
  pinMode(5, OUTPUT); // first motor
  pinMode(6, OUTPUT); // second motor
  pinMode(7, OUTPUT); // second motor
  pinMode(8, OUTPUT); // third motor
  pinMode(9, OUTPUT); // third motor
  pinMode(10, OUTPUT); // fourth motor
  pinMode(11, OUTPUT); // fourth motor
}

void loop() {
  // set motor power
  for (int i = 0; i < 4; i++) {
    // if power <0; reverse direction...
    digitalWrite((2*i + 4), motDirection[i] ? HIGH : LOW); // lmao ovo nes radi
    analogWrite((2*i + 5), motPower[i]);
  }
}

/*
void setMotorPower(int motorIdx, int power, bool direc) {
  motPower[motorIdx] = motorPower;
  motDirection[motorIdx] = direc;
}*/

/*
  SerialEvent occurs whenever a new data comes in the hardware serial RX. This
  routine is run between each time loop() runs, so using delay inside loop can
  delay response. Multiple bytes of data may be available.
*/
void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char header = (char)Serial.read();

    switch (header) {
      case 'P':
        motPower[0] = Serial.read();
        motPower[1] = Serial.read();
        motPower[2] = Serial.read();
        motPower[3] = Serial.read();
        break;
      case 'O':
        Serial.println("Ping");
        Serial.println("Power " + String(motPower[0]) + " " + String(motPower[1]) + " " + String(motPower[2]) + " " + String(motPower[3]));
        Serial.println("Direction " + String(motDirection[0]) + " " + String(motDirection[1]) + " " + String(motDirection[2]) + " " + String(motDirection[3]));
        break;
      case 'F':
        motPower[0] = 255;
        motPower[1] = 255;
        motPower[2] = 255;
        motPower[3] = 255;
        motDirection[0] = motDirection[1] = motDirection[2] = motDirection[3] = 0;
        Serial.println("Going forward");
        break;
      case 'L':
        motPower[0] = 0;
        motPower[1] = 255;
        motPower[2] = 0;
        motPower[3] = 0;
        motDirection[0] = motDirection[1] = motDirection[2] = motDirection[3] = 0;
        break;
      case 'R':
        motPower[0] = 0;
        motPower[1] = 255;
        motPower[2] = 0;
        motPower[3] = 0;
        motDirection[0] = motDirection[1] = motDirection[2] = motDirection[3] = 0;
        break;
      case 'B':
        motPower[0] = 255;
        motPower[1] = 255;
        motPower[2] = 255;
        motPower[3] = 255;
        motDirection[0] = motDirection[1] = motDirection[2] = motDirection[3] = 1;
        break;
      case 'K':
        motPower[0] = 0;
        motPower[1] = 0;
        motPower[2] = 0;
        motPower[3] = 0;
        motDirection[0] = motDirection[1] = motDirection[2] = motDirection[3] = 0;
        break;
    }
  }
}
