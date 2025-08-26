#include <Servo.h>

const byte ROWS = 4;
const byte COLS = 4;

// Keypad layout
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

byte rowPins[ROWS] = {2, 3, 4, 5};  // R1–R4
byte colPins[COLS] = {6, 7, 8, 9};  // C1–C4

String input = "";
String password = "1234";   // Change your password here
Servo lockServo;

void setup() {
  Serial.begin(9600);

  // Setup rows
  for (byte r = 0; r < ROWS; r++) {
    pinMode(rowPins[r], OUTPUT);
    digitalWrite(rowPins[r], HIGH);
  }

  // Setup columns
  for (byte c = 0; c < COLS; c++) {
    pinMode(colPins[c], INPUT_PULLUP);
  }

 { // Setup servo
  lockServo.attach(10);  // GP10 → Servo signal
  lockServo.write(0);    // Locked position

  Serial.println("Password Lock Ready...");
  Serial.println("Enter password followed by #");
 }
}

char getKey() {
  for (byte r = 0; r < ROWS; r++) {
    digitalWrite(rowPins[r], LOW);
    for (byte c = 0; c < COLS; c++) {
      if (digitalRead(colPins[c]) == LOW) {
        while (digitalRead(colPins[c]) == LOW); // wait for release
        delay(50); // debounce
        digitalWrite(rowPins[r], HIGH);
        return keys[r][c];
      }
    }
    digitalWrite(rowPins[r], HIGH);
  }
  return 0;
}

void loop() {
  char key = getKey();
  if (key) {
    Serial.print("Key: ");
    Serial.println(key);

    if (key == '*') {
      input = "";
      Serial.println("Input cleared");
    }
    else if (key == '#') {
      if (input == password) {
        Serial.println("Access Granted ✅");
        lockServo.write(120);   // Unlock
        delay(5000);           // Keep unlocked for 5s
        lockServo.write(0);    // Lock again
      } else {
        Serial.println("Access Denied ❌");
      }
      input = "";
    }
    else {
      input += key;
      Serial.print("Current Input: ");
      Serial.println(input);
    }
  }
}
