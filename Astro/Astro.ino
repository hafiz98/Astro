

#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 16, 2);

const int diSwHelkBout = 2;
const int aiVrXHelkBout = A0;
const int aiVrYHelkBout = A1;

void setup() {
  pinMode(diSwHelkBout, INPUT);
  digitalWrite(diSwHelkBout, HIGH);
  Serial.begin(9600);

  // initialize the LCD
  lcd.begin();

  // Turn on the blacklight and print a message.
  lcd.backlight();
  lcd.print("Haffoo Productions");
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.print("Switch:  ");
  Serial.print(digitalRead(diSwHelkBout));
  Serial.print("\n");
  Serial.print("X-axis: ");
  Serial.print(analogRead(aiVrXHelkBout));
  Serial.print("\n");
  Serial.print("Y-axis: ");
  Serial.println(analogRead(aiVrYHelkBout));
  Serial.print("\n\n");
  delay(500);

  if (Serial.available()) {
    // Wait a bit for the entire message to arrive
    delay(100);
    // Clear the screen
    lcd.clear();

    // Write all characters received with the serial port to the LCD.
    while (Serial.available() > 0) {
      lcd.write(Serial.read());
    }
  }
}


