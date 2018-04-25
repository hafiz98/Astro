#include <SimpleTimer.h>
#include <LiquidCrystal_I2C.h>

//draden goed fixen
//commentaar wanneer gebruiken?
//rockets
//commentaar code
//na schieten geluid
//init functie gebruiken

// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 20, 4);

const int diSwHelkBout = 2;
const int aiVrXHelkBout = A0;
const int aiVrYHelkBout = A1;
const int doBuzzerHelkBout = 3;
SimpleTimer timerGenerateAstroids;
SimpleTimer timerMoveAstroids;
SimpleTimer timerGenerateSpaceship;
SimpleTimer timerMoveSpaceship;
SimpleTimer timerSplashScreen;
SimpleTimer timerGameOverScreen;


typedef struct {
  int xPosition;
  int yPosition;
  int randomXPositionHelkBout = random(20, 100);
  int randomYPositionHelkBout = random(0, 2);
  int number;
  String symbolUfo;
} ufo;

ufo Astroid[5];
ufo Spaceship[5];

int joystickYPositionHelkBout;
int joystickXPositionHelkBout;
int scoreAstroidHelkBout = 0;
int scoreSpaceshipHelkBout = 10;
int gameStatus = 0;

void setup() {
  pinMode(diSwHelkBout, INPUT);
  digitalWrite(diSwHelkBout, HIGH);
  Serial.begin(9600);
  lcd.begin();
  lcd.backlight();
  
  timerGenerateAstroids.setTimer(1000, generateAstroids, 1); // dit moet in functie
  timerMoveAstroids.setInterval(1000, moveAstroids);

  timerGenerateSpaceship.setTimer(1000, generateSpaceship, 1);
  timerMoveSpaceship.setInterval(1000, moveSpaceship);

  timerSplashScreen.setInterval(1000, splashScreen);
  timerGameOverScreen.setInterval(1000, gameOverScreen);
}

void loop() {

  if (gameStatus == 0) {
    Serial.print("gamestatus: ");
    Serial.println(gameStatus);
    timerSplashScreen.run();
  }

  if (gameStatus == 1) {
    scoreBoard();
    joystickRemote();
    timerGenerateAstroids.run();
    timerMoveAstroids.run();
    timerGenerateSpaceship.run();
    timerMoveSpaceship.run();
  }
  
//  if (gameStatus == 3) {
//    Serial.print("gamestatus: ");
//    Serial.println(gameStatus);
//    timerSplashScreen.run();
//  }

  if (scoreSpaceshipHelkBout == 0) {
    gameOverScreen();
  }
  //splashScreen();

  //  if (gameStatus == 0) {
  //    Serial.print("gamestatus: ");
  //    Serial.println(gameStatus);
  //    timerSplashScreen.run();
  //  }

  //  if (gameStatus == 1){
  //    Serial.print("gamestatus: ");
  //    Serial.println(gameStatus);
  //    scoreBoard();
  //    joystickRemote();
  //    timerGenerateAstroids.run();
  //    timerMoveAstroids.run();
  //    timerGenerateSpaceship.run();
  //    timerMoveSpaceship.run();
  //  }




  if (Serial.available()) {
    delay(100);    // Wait a bit for the entire message to arrive
    lcd.clear();    // Clear the screen
    while (Serial.available() > 0) {
      lcd.write(Serial.read());
    }
  }

}

void joystickRemote() {

  if (analogRead(aiVrXHelkBout) < 250) {
    clearJoystickPos();
    joystickTop();
  }
  if (analogRead(aiVrXHelkBout) > 750 && analogRead(aiVrXHelkBout) < 1000) {
    clearJoystickPos();
    joystickBot();
  }
  if (analogRead(aiVrXHelkBout) > 400 && analogRead(aiVrXHelkBout) < 600) {
    clearJoystickPos();
    joystickMid();
  }
}
void clearJoystickPos() {
  lcd.setCursor(0, 0);
  lcd.print(" ");

  lcd.setCursor(0, 1);
  lcd.print(" ");

  lcd.setCursor(0, 2);
  lcd.print(" ");
}
void joystickTop() {
  joystickYPositionHelkBout = 0;
  joystickXPositionHelkBout = 0;
  lcd.setCursor(0, 0);
  lcd.print("(");
}
void joystickMid() {
  joystickYPositionHelkBout = 1;
  joystickXPositionHelkBout = 0;
  lcd.setCursor(0, 1);
  lcd.print("(");
}
void joystickBot() {
  joystickYPositionHelkBout = 2;
  joystickXPositionHelkBout = 0;
  lcd.setCursor(0, 2);
  lcd.print("(");
}
void scoreBoard() {
  lcd.setCursor(0, 3);
  lcd.print("S=");

  lcd.setCursor(2, 3);
  lcd.print(scoreSpaceshipHelkBout);

  lcd.setCursor(5, 3);
  lcd.print("A=");

  lcd.setCursor(7, 3);
  lcd.print(scoreAstroidHelkBout);
}
void buzzerAstroid() {
  tone(doBuzzerHelkBout, 1000, 250);
}
void buzzerSpaceship() {
  tone(doBuzzerHelkBout, 1000, 250);
  delay(250);
  tone(doBuzzerHelkBout, 1000, 250);
  delay(250);
  tone(doBuzzerHelkBout, 100, 1000);
}
void splashScreen() {
  Serial.println("ik ben in splash screen!");
  lcd.setCursor(0, 0);
  lcd.print("Astroidinator V1");

  lcd.setCursor(0, 1);
  lcd.print("Gemaakt door:");

  lcd.setCursor(0, 2);
  lcd.print("Hafiz");

  lcd.setCursor(0, 3);
  lcd.print("Nassim");

  gameStatus = 1;
  Serial.print("gamestatus: ");
  Serial.println(gameStatus);

  delay(2500);
  lcd.clear();
}
void gameOverScreen() {
  Serial.println("ik ben in gameover screen!");
  lcd.setCursor(5, 1);
  lcd.print("Game  over");

  lcd.setCursor(6, 2);
  lcd.print("Astroids");

  lcd.setCursor(9, 3);
  lcd.print(scoreAstroidHelkBout);
  gameStatus = 3;

  delay(2500);
  lcd.clear();
}

void generateAstroids() {
  for (int i = 0; i < 5; i++) {
    Astroid[i].xPosition = Astroid[i].randomXPositionHelkBout;
    if (Astroid[i].xPosition % 2) {
      Astroid[i].xPosition + 1;
    }
    Astroid[i].yPosition = Astroid[i].randomYPositionHelkBout;
    lcd.setCursor(Astroid[i].xPosition, Astroid[i].yPosition);
    lcd.print("*");
  }
}
void moveAstroids() {
  for (int i = 0; i < 5; i++) {
    lcd.setCursor(Astroid[i].xPosition, Astroid[i].yPosition);
    lcd.print(" ");
    Astroid[i].xPosition = Astroid[i].xPosition - 1;
    if (joystickYPositionHelkBout == Astroid[i].yPosition && joystickXPositionHelkBout == Astroid[i].xPosition) {
      Astroid[i].xPosition = 35;
      buzzerAstroid();
      scoreAstroidHelkBout = scoreAstroidHelkBout + 1;
    }
    if (Astroid[i].xPosition >= 0 && Astroid[i].xPosition <= 20) {
      lcd.setCursor(Astroid[i].xPosition, Astroid[i].yPosition);
      lcd.print("*");
    }
  }
}

void generateSpaceship() {
  Serial.println("-----HIERONDER GEGENEREEDE Spaceship-------");
  for (int i = 0; i < 4; i++)
  {
    Spaceship[i].xPosition = Spaceship[i].randomXPositionHelkBout;
    if (Spaceship[i].xPosition % 2) 
    {
      Spaceship[i].xPosition + 1;
    }
    Spaceship[i].yPosition = Spaceship[i].randomYPositionHelkBout;
    lcd.setCursor(Spaceship[i].xPosition, Spaceship[i].yPosition);
    lcd.print("<");
  }
}

void moveSpaceship() {
  Serial.println("-----HIERONDER NIEUWE POSITIE-------");
  for (int i = 0; i < 4; i++) {
    lcd.setCursor(Spaceship[i].xPosition, Spaceship[i].yPosition);
    lcd.print(" ");
    Spaceship[i].xPosition = Spaceship[i].xPosition - 1;
    Serial.print("xpos van spaceship: ");
    Serial.println(Spaceship[i].xPosition);
    if (joystickYPositionHelkBout == Spaceship[i].yPosition && joystickXPositionHelkBout == Spaceship[i].xPosition) {
      Spaceship[i].xPosition = 35;
      buzzerSpaceship();
      scoreSpaceshipHelkBout = scoreSpaceshipHelkBout - 1;
      lcd.noBacklight();
      delay(150);
      lcd.backlight();
    }
    if (Spaceship[i].xPosition >= 0 && Spaceship[i].xPosition <= 20) {
      Serial.println(scoreSpaceshipHelkBout);
      lcd.setCursor(Spaceship[i].xPosition, Spaceship[i].yPosition);
      lcd.print("<");
    }
  }
}
