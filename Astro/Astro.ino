#include <SimpleTimer.h>
#include <LiquidCrystal_I2C.h>


// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 16, 3);

const int diSwHelkBout = 2;
const int aiVrXHelkBout = A0;
const int aiVrYHelkBout = A1;
SimpleTimer timer;


int timer1;

void setup() {
  pinMode(diSwHelkBout, INPUT);
  digitalWrite(diSwHelkBout, HIGH);
  Serial.begin(9600);

  // initialize the LCD
  lcd.begin();

  // Turn on the blacklight and print a message.
  lcd.backlight(); 

}

void loop() {
splashScreen();



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

void joystickRemote(){
  lcd.clear();
  if(analogRead(aiVrXHelkBout) < 250){
    Serial.println("Ik moet naar boven");
    clearJoystickPos();
    joystickTop();
  }
  if(analogRead(aiVrXHelkBout) > 750 && analogRead(aiVrXHelkBout) < 1000){
    Serial.println("Ik moet naar beneden");
    clearJoystickPos();
    joystickBot();
  }
  if(analogRead(aiVrXHelkBout) > 400 && analogRead(aiVrXHelkBout) < 600){
    Serial.println("Ik moet naar midden");
    clearJoystickPos();
    joystickMid();
  }
}

void clearJoystickPos(){
  lcd.setCursor(0, 0);
  lcd.print(" ");

  lcd.setCursor(0, 1);
  lcd.print(" ");

  lcd.setCursor(0, 2);
  lcd.print(" ");
}

void joystickTop(){
  lcd.setCursor(0, 0);
  lcd.print("(");
}
void joystickMid(){
  lcd.setCursor(0, 1);
  lcd.print("(");
}
void joystickBot(){
  lcd.setCursor(0, 2);
  lcd.print("(");
}



void splashScreen(){
  timer.run();
  timer1 = timer.setTimer(5000, joystickRemote, 1);

  if(timer.isEnabled(timer1)) {
     lcd.clear();
  }

  lcd.setCursor(0, 0);
  lcd.print("Astroidinator V1");
 
  lcd.setCursor(0, 1);
  lcd.print("Gemaakt door:");

  lcd.setCursor(0, 2);
  lcd.print("Hafiz");

  lcd.setCursor(0, 3);
  lcd.print("Bram");
}


