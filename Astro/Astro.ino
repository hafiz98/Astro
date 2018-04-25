#include <SimpleTimer.h>
#include <LiquidCrystal_I2C.h>
//Gemaakt door Nassim Tijani Hafiz Elkelic
//draden goed fixen
//commentaar wanneer gebruiken?
//rockets
//commentaar code  --------------------------------------------------done
//na schieten geluid
//init functie gebruiken

// Set the LCD address to 0x27 for a 20 chars and 4 line display
LiquidCrystal_I2C lcd(0x27, 20, 4);

const int diSwHelkBout = 4;
const int aiVrXHelkBout = A0;
const int aiVrYHelkBout = A1;
const int doBuzzerHelkBout = 3;



SimpleTimer timerGenerateAstroids;
SimpleTimer timerMoveAstroids;

SimpleTimer timerGenerateSpaceship;
SimpleTimer timerMoveSpaceship;

SimpleTimer timerGenerateRocket;
SimpleTimer timerMoveRocket;

SimpleTimer timerSplashScreen;
SimpleTimer timerGameOverScreen;


typedef struct {
  int xPosition;//x positie int voor de waarde van de x positie
  int yPosition;//y positie int voor de waarde van de y positie
  int randomXPositionHelkBout = random(20, 100);// genereer een randomwaarde tussen de locatie van 20 tot 100 zodat ze van rechts komen.
  int randomYPositionHelkBout = random(0, 2);// de hoogte van de lcd scherm genereerd een getal tussen 0, 2 want begint bij 0.
  int number;
  String symbolUfo;
} ufo;

ufo Astroid[5]; // creeren van 5 astroids
ufo Spaceship[5]; // creeren van 5 spaceships
ufo Rocket[5]; // creeren van 1 raket

int joystickYPositionHelkBout;// y waarde van de joystick
int joystickXPositionHelkBout;// x waarde van de joystick
int scoreAstroidHelkBout = 0; // int voor de score van de astroids
int scoreSpaceshipHelkBout = 10; // int voor de score van de spaceships
int gameStatus = 0; // game status zo kunnen we de splash screen maken en de game over maken.
bool shootRocketPermission = false;
void setup() {
  pinMode(diSwHelkBout, INPUT);

  digitalWrite(diSwHelkBout, HIGH);
  Serial.begin(9600);
  lcd.begin();
  lcd.backlight();


  //timers voor de astroids en voor de spaceships
  timerGenerateAstroids.setTimer(1000, generateAstroids, 1); // dit moet in functie
  timerMoveAstroids.setInterval(1000, moveAstroids);

  timerGenerateSpaceship.setTimer(1000, generateSpaceship, 1);
  timerMoveSpaceship.setInterval(1000, moveSpaceship);

  //timerGenerateRocket.setTimer(1000, generateRocket, 1);
  timerMoveRocket.setInterval(1000, moveRocket);

  timerSplashScreen.setInterval(1000, splashScreen);
  timerGameOverScreen.setInterval(1000, gameOverScreen);
}

void loop() {
  //als game status 0 is laaten we splash screen zien
  if (gameStatus == 0) {
    Serial.print("gamestatus: ");
    Serial.println(gameStatus);
    timerSplashScreen.run();
  }

  //als game status 1 is dan start game weer.
  if (gameStatus == 1) {
    scoreBoard();
    joystickRemote();
    shootRockets();
    timerGenerateAstroids.run();
    timerMoveAstroids.run();

    timerGenerateSpaceship.run();
    timerMoveSpaceship.run();
  }

  if (shootRocketPermission == true) {
    //timerGenerateRocket.run();
    timerMoveRocket.run();
  }

  if (scoreSpaceshipHelkBout == 0) {
    gameOverScreen();
  }

  //het laten zien van lcd naar delay
  if (Serial.available()) {
    delay(100);    // Wait a bit for the entire message to arrive
    lcd.clear();    // Clear the screen
    while (Serial.available() > 0) {
      lcd.write(Serial.read());
    }
  }

}

//leeg maken van remote joystick
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

//het leeg maken van de joystick positie
void clearJoystickPos() {
  lcd.setCursor(0, 0);
  lcd.print(" ");

  lcd.setCursor(0, 1);
  lcd.print(" ");

  lcd.setCursor(0, 2);
  lcd.print(" ");
}

//als hij top is nieuwe locatie
void joystickTop() {
  joystickYPositionHelkBout = 0;
  joystickXPositionHelkBout = 0;
  lcd.setCursor(0, 0);
  lcd.print("(");
}

//als joystick mid is laat locatie zien
void joystickMid() {
  joystickYPositionHelkBout = 1;
  joystickXPositionHelkBout = 0;
  lcd.setCursor(0, 1);
  lcd.print("(");
}

// als joystick naar beneden gaat geef nieuwe locatie aan
void joystickBot() {
  joystickYPositionHelkBout = 2;
  joystickXPositionHelkBout = 0;
  lcd.setCursor(0, 2);
  lcd.print("(");
}

// laten zien van score bord helemaal 4de rij
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

//splash screen met informatie over makers
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

//het game over screen met score
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

//het maken van astroids
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

//het bewegen van astroids naar voren
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

//generen van spaceships
void generateSpaceship() {
  Serial.println("-----HIERONDER GEGENEREEDE SPACESHIP-------");
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

//het naar voren bewegen van spaceships
void moveSpaceship() {
  Serial.println("-----HIERONDER NIEUWE POSITIE VAN EEN SPACESHIP-------");
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
      lcd.setCursor(Spaceship[i].xPosition, Spaceship[i].yPosition);
      lcd.print("<");
    }
  }
}

// het genereren van een raket
void generateRocket() {
  Serial.println("-----HIERONDER GEGENEREEDE ROCKET-------");
  for (int i = 0; i < 1; i++) {
    Rocket[i].xPosition = joystickXPositionHelkBout;
    Rocket[i].yPosition = joystickYPositionHelkBout;

    Serial.print("xpos van Raket: ");
    Serial.println(Rocket[i].xPosition);
    Serial.print("ypos van Raket: ");
    Serial.println(Rocket[i].yPosition);

    lcd.setCursor(Rocket[i].xPosition, Rocket[i].yPosition);
    lcd.print("-");
  }
}

//het naar voren bewegen van spaceships
void moveRocket() {
  Serial.println("-----HIERONDER NIEUWE POSITIE ROCKET-------");
  for (int i = 0; i < 4; i++) {
    lcd.setCursor(Rocket[i].xPosition, Rocket[i].yPosition);
    lcd.print(" ");
    Rocket[i].xPosition = Rocket[i].xPosition + 1;
    Serial.print("xpos van Raket: ");
    Serial.println(Rocket[i].xPosition);
    if (Rocket[i].xPosition == Spaceship[i].xPosition && Rocket[i].yPosition == Spaceship[i].yPosition) {
      Serial.println("ROCKET COLLISION!");
      Spaceship[i].xPosition = 35;
      Rocket[i].xPosition = 35;
      buzzerRocketExplode();
      scoreSpaceshipHelkBout = scoreSpaceshipHelkBout + 1;
      lcd.noBacklight();
      delay(150);
      lcd.backlight();
      lcd.noBacklight();
      delay(150);
      lcd.backlight();
      lcd.noBacklight();
      delay(150);
      lcd.backlight();
    }
    if (Rocket[i].xPosition >= 0 && Rocket[i].xPosition <= 20) {
      lcd.setCursor(Rocket[i].xPosition, Rocket[i].yPosition);
      lcd.print("-");
      buzzerRocket();
    }
  }
}
// het schieten van raketten naar de ruimteschippen
void shootRockets() {
  int btnShootRocketsHelkBout = digitalRead(diSwHelkBout);

  if ( btnShootRocketsHelkBout == LOW) {
    shootRocketPermission = true;
    for (int i = 0; i < 1; i++) {
      if (Rocket[i].xPosition >= 20) { // als de rocket in het LCD is
            generateRocket();
      }
    }
  }
}

//geluiden genereren na het aanraken van een astroid
void buzzerAstroid() {
  tone(doBuzzerHelkBout, 1000, 250);
}

//nieuw geluidje maken na het aanraken van spaceship
void buzzerSpaceship() {
  tone(doBuzzerHelkBout, 1000, 250);
  delay(250);
  tone(doBuzzerHelkBout, 1000, 250);
  delay(250);
  tone(doBuzzerHelkBout, 100, 1000);
}

// geluidje als er een raket geschoten is
void buzzerRocket() {
  tone(doBuzzerHelkBout, 100, 1000);
}

// geluidje als er een raket ontploft is
void buzzerRocketExplode() {
  tone(doBuzzerHelkBout, 5000, 1000);
}
