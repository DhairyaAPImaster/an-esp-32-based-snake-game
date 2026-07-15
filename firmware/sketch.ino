#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

#define BTN_UP    5
#define BTN_DOWN  4
#define BTN_LEFT  18
#define BTN_RIGHT 19

#define CELL_SIZE 4

const int COLS = SCREEN_WIDTH / CELL_SIZE;
const int ROWS = SCREEN_HEIGHT / CELL_SIZE;

const unsigned long DEBOUNCE_DELAY = 30;

struct Button {
  uint8_t pin;
  bool stableState;
  bool lastRawState;
  unsigned long lastChangeTime;
  bool ignoreHeld;
};

Button btnUp    = {BTN_UP, HIGH, HIGH, 0, false};
Button btnDown  = {BTN_DOWN, HIGH, HIGH, 0, false};
Button btnLeft  = {BTN_LEFT, HIGH, HIGH, 0, false};
Button btnRight = {BTN_RIGHT, HIGH, HIGH, 0, false};

struct Point {
  int x;
  int y;
};

Point snake[200];
Point food;

int snakeLength = 5;

enum Direction {
  UP,
  DOWN,
  LEFT,
  RIGHT
};

Direction dir = RIGHT;

unsigned long lastMove = 0;
const int moveDelay = 150;

bool gameOver = false;
bool gameStarted = false;

bool startScreenDrawn = false;
bool gameOverDrawn = false;

void resetGame();
void spawnFood();
void updateButton(Button &b);
bool isPressed(Button &b);
bool isPressedForDirection(Button &b);
bool anyButtonPressed();
void armIgnoreForHeldButtons();
void readButtons();
void moveSnake();
void drawGame();
void drawGameOver();
void drawStartScreen();

void setup() {

  Serial.begin(115200);

  pinMode(BTN_UP, INPUT_PULLUP);
  pinMode(BTN_DOWN, INPUT_PULLUP);
  pinMode(BTN_LEFT, INPUT_PULLUP);
  pinMode(BTN_RIGHT, INPUT_PULLUP);

  Wire.begin(21,22);

  // Faster OLED updates
  Wire.setClock(400000);

  if(!display.begin(SSD1306_SWITCHCAPVCC,0x3C)){
    while(true);
  }

  display.clearDisplay();
  display.display();

  randomSeed(millis());

  resetGame();
}

void loop() {

  updateButton(btnUp);
  updateButton(btnDown);
  updateButton(btnLeft);
  updateButton(btnRight);

  // ---------- START SCREEN ----------
  if(!gameStarted){

    if(!startScreenDrawn){
      drawStartScreen();
      startScreenDrawn=true;
    }

    if(anyButtonPressed()){

      gameStarted=true;
      startScreenDrawn=false;

      resetGame();
      armIgnoreForHeldButtons();
    }

    return;
  }

  // ---------- GAME OVER ----------
  if(gameOver){

    if(!gameOverDrawn){
      drawGameOver();
      gameOverDrawn=true;
    }

    if(anyButtonPressed()){

      gameOver=false;
      gameStarted=false;
      gameOverDrawn=false;
    }

    return;
  }

  // Read buttons every loop (VERY FAST)
  readButtons();

  // Move only every 150 ms
  if(millis()-lastMove>=moveDelay){

    lastMove=millis();

    moveSnake();

    drawGame();
  }
}

void drawStartScreen(){

  display.clearDisplay();

  display.setTextSize(2);
  display.setCursor(18,10);
  display.println("SNAKE");

  display.setTextSize(1);
  display.setCursor(6,38);
  display.println("Press Any Button");

  display.setCursor(25,50);
  display.println("to Start");

  display.display();
}

void updateButton(Button &b){

  bool raw=digitalRead(b.pin);

  if(raw!=b.lastRawState){

    b.lastRawState=raw;
    b.lastChangeTime=millis();
  }

  if(millis()-b.lastChangeTime>=DEBOUNCE_DELAY){

    b.stableState=raw;
  }
}

bool isPressed(Button &b){
  return b.stableState==LOW;
}

bool anyButtonPressed(){

  return isPressed(btnUp) ||
         isPressed(btnDown) ||
         isPressed(btnLeft) ||
         isPressed(btnRight);
}

void armIgnoreForHeldButtons(){

  if(isPressed(btnUp)) btnUp.ignoreHeld=true;
  if(isPressed(btnDown)) btnDown.ignoreHeld=true;
  if(isPressed(btnLeft)) btnLeft.ignoreHeld=true;
  if(isPressed(btnRight)) btnRight.ignoreHeld=true;
}

bool isPressedForDirection(Button &b){

  if(b.ignoreHeld){

    if(!isPressed(b))
      b.ignoreHeld=false;

    return false;
  }

  return isPressed(b);
}

void readButtons(){

  if(isPressedForDirection(btnUp) && dir!=DOWN)
    dir=UP;

  else if(isPressedForDirection(btnDown) && dir!=UP)
    dir=DOWN;

  else if(isPressedForDirection(btnLeft) && dir!=RIGHT)
    dir=LEFT;

  else if(isPressedForDirection(btnRight) && dir!=LEFT)
    dir=RIGHT;
}

void moveSnake() {

  Point newHead = snake[0];

  switch (dir) {

    case UP:
      newHead.y--;
      break;

    case DOWN:
      newHead.y++;
      break;

    case LEFT:
      newHead.x--;
      break;

    case RIGHT:
      newHead.x++;
      break;
  }

  // Wall collision
  if (newHead.x < 0 ||
      newHead.x >= COLS ||
      newHead.y < 0 ||
      newHead.y >= ROWS) {

    gameOver = true;
    return;
  }

  // Self collision
  for (int i = 1; i < snakeLength; i++) {

    if (snake[i].x == newHead.x &&
        snake[i].y == newHead.y) {

      gameOver = true;
      return;
    }
  }

  bool ateFood =
      (newHead.x == food.x &&
       newHead.y == food.y);

  // Shift body
  for (int i = snakeLength; i > 0; i--) {
    snake[i] = snake[i - 1];
  }

  // Move head
  snake[0] = newHead;

  // Grow snake
  if (ateFood) {
    snakeLength++;
    spawnFood();
  }
}

void resetGame() {

  snakeLength = 5;

  snake[0] = {10, 8};
  snake[1] = {9, 8};
  snake[2] = {8, 8};
  snake[3] = {7, 8};
  snake[4] = {6, 8};

  dir = RIGHT;

  gameOver = false;
  gameOverDrawn = false;

  spawnFood();

  // Draw the first frame immediately
  drawGame();

  // Prevent an instant move after reset
  lastMove = millis();
}

void spawnFood() {

  bool validPosition = false;

  while (!validPosition) {

    validPosition = true;

    food.x = random(COLS);
    food.y = random(ROWS);

    for (int i = 0; i < snakeLength; i++) {

      if (snake[i].x == food.x &&
          snake[i].y == food.y) {

        validPosition = false;
        break;
      }
    }
  }
}

void drawGame() {

  display.clearDisplay();

  // Draw snake
  for (int i = 0; i < snakeLength; i++) {

    display.fillRect(
      snake[i].x * CELL_SIZE,
      snake[i].y * CELL_SIZE,
      CELL_SIZE,
      CELL_SIZE,
      WHITE
    );
  }

  // Draw food
  display.fillCircle(
    food.x * CELL_SIZE + CELL_SIZE / 2,
    food.y * CELL_SIZE + CELL_SIZE / 2,
    CELL_SIZE / 2,
    WHITE
  );

  // Draw score
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.print("Score: ");
  display.print(snakeLength - 5);

  // Send the framebuffer to the OLED
  display.display();
}

void drawGameOver() {

  display.clearDisplay();

  display.setTextColor(WHITE);

  display.setTextSize(2);
  display.setCursor(18, 10);
  display.println("GAME");

  display.setCursor(18, 30);
  display.println("OVER");

  display.setTextSize(1);
  display.setCursor(20, 52);
  display.print("Score: ");
  display.print(snakeLength - 5);

  display.display();
}
