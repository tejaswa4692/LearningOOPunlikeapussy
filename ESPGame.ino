#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// ------------------ SETTINGS ------------------
#define BALL_COUNT 30
#define BALL_MIN_R 10
#define BALL_MAX_R 15
#define BALL_SPEED 1

// Bottom balls
#define BALL_BOTTOM_Y_MIN 60
#define BALL_BOTTOM_Y_MAX 65

// Top balls
#define BALL_TOP_Y_MIN -5
#define BALL_TOP_Y_MAX 4

// ------------------ SPHERE CLASS ------------------
class Sphere {
  public:
    int x, y, r;

    void init(int startX, int yMin, int yMax) {
      x = startX;
      r = random(BALL_MIN_R, BALL_MAX_R);
      y = random(yMin, yMax);
    }

    void move(int yMin, int yMax) {
      x += BALL_SPEED;
      if (x > SCREEN_WIDTH + r) {
        x = -r;
        r = random(BALL_MIN_R, BALL_MAX_R);
        y = random(yMin, yMax);
      }
    }

    void draw() {
      display.fillCircle(x, y, r, SSD1306_WHITE);
    }
};

// ------------------ BALL ARRAYS ------------------
Sphere bottomBalls[BALL_COUNT];
Sphere topBalls[BALL_COUNT];

// ------------------ UPDATE FUNCTIONS ------------------
void updateBottomBalls() {
  for (int i = 0; i < BALL_COUNT; i++) {
    bottomBalls[i].move(BALL_BOTTOM_Y_MIN, BALL_BOTTOM_Y_MAX);
    bottomBalls[i].draw();
  }
}

void updateTopBalls() {
  for (int i = 0; i < BALL_COUNT; i++) {
    topBalls[i].move(BALL_TOP_Y_MIN, BALL_TOP_Y_MAX);
    topBalls[i].draw();
  }
}

void setup() {
  Wire.begin();

  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    for (;;);
  }

  display.clearDisplay();
  randomSeed(analogRead(A0));

  for (int i = 0; i < BALL_COUNT; i++) {
    bottomBalls[i].init(i * (SCREEN_WIDTH / BALL_COUNT) - 10,
                        BALL_BOTTOM_Y_MIN, BALL_BOTTOM_Y_MAX);

    topBalls[i].init(i * (SCREEN_WIDTH / BALL_COUNT) - 10,
                     BALL_TOP_Y_MIN, BALL_TOP_Y_MAX);
  }
}

void loop() {
  display.clearDisplay();

  updateTopBalls();     // balls at top
  updateBottomBalls();  // balls at bottom

  display.display();
  delay(16);
}
