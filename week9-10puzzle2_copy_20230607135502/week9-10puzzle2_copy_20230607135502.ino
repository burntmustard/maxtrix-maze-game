#include <Adafruit_CircuitPlayground.h>

int map1[10][10] = {
  { 2, 0, 0, 0, 0, 0, 1, 1, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 1, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 1, 0, 0, 0, 1, 1, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 1, 1, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 1, 0, 0, 1, 0, 0, 0, 0, 0, 0 }
};

int rb = 5;  //right button
int lb = 4;  //left button
int s = 7;   //switch

volatile int xl = 0;   //x location
volatile int yl = 0;   //y location
volatile int gx = 0;   //gold x location
volatile int gy = 0;   //gold y location
volatile int dir = 0;  //direction 0=horizontal

volatile bool rf = 0;  //right flag
volatile bool lf = 0;  //left flag
volatile bool sf = 0;
volatile bool start = 1;

void rs() {
  rf = 1;
}  //right button state
void ls() {
  lf = 1;
}  //left button state
void sc() {
  sf = 1;
}  //switch state

void Start() { //randomizes the obstacles on the map
  for (int t = 0; t < 14; t++) {
    map1[random(9)][random(9)] = 1;
  }
  gx = random(9);
  gy = random(9);
  map1[gy][gx] = 3;
  start = 0;
}

void update() {
  Serial.println();
  map1[yl][xl] = 2;
  for (int i = 0; i < 10; i++) {
    for (int t = 0; t < 9; t++) {  //prints the first nine lines of array
      if (map1[i][t] == 2) {
        Serial.print("[O]");
      } else if ((map1[i][t] == 3)) {
        Serial.print("[X]");
      } else {
        Serial.print("[ ]");
      }
    }
    if (map1[i][9] == 2) {  //prints the last line of array & starts new line
      Serial.println("[O]");
    } else if ((map1[i][9] == 3)) {
      Serial.println("[X]");
    } else {
      Serial.println("[ ]");
    }
  }
  if (gx == xl && gy == yl) {
    Serial.println("- YOU WIN! -");
    Serial.println("- press any key to begin -");
    start = 1;
  }
  Serial.print(dir);
  Serial.print(yl);
  Serial.print(xl);
  Serial.print(sf);
}

void setup() {
  Serial.begin(9600);
  CircuitPlayground.begin();

  pinMode(rb, INPUT_PULLDOWN);
  pinMode(lb, INPUT_PULLDOWN);
  attachInterrupt(digitalPinToInterrupt(rb), rs, RISING);
  attachInterrupt(digitalPinToInterrupt(lb), ls, RISING);
  attachInterrupt(digitalPinToInterrupt(s), sc, CHANGE);
  Serial.println("- press any key to begin -");
}

void loop() {
  if (lf) {  //backwards movement on the board
    if (start == 1) { Start(); }
    map1[yl][xl] = 0;
    if (dir == 0 && map1[yl][xl - 1] != 1 && xl > 0) { xl--; }
    if (yl > 0) {
      if (dir == 1 && map1[yl - 1][xl] != 1) { yl--; }  //not sure why, but I couldn't combine the ifs with an "&&"
    }
    update();
    delay(100);
    lf = 0;
  }
  if (rf) {  //forward movement on the board
    if (start == 1) { Start(); }
    map1[yl][xl] = 0;
    if (dir == 0 && map1[yl][xl + 1] != 1 && xl < 9) { xl++; }
    if (dir == 1 && map1[yl + 1][xl] != 1 && yl < 9) { yl++; }
    update();
    delay(100);
    rf = 0;
  }
  if (sf) {  //changes axis that the board displays
    if (start == 1) { Start(); }
    dir++;
    if (dir == 2) { dir = 0; }
    update();
    delay(100);
    sf = 0;
  }
  if (dir == 0) {
    for (int t = 0; t < 10; t++) {  //sets the lights on the board according to the matrix value
      if (map1[yl][t] == 0) {
        CircuitPlayground.setPixelColor(t, 0, 0, 0);
      } else if (map1[yl][t] == 1) {
        CircuitPlayground.setPixelColor(t, 250, 20, 0);
      } else if (map1[yl][t] == 2) {
        CircuitPlayground.setPixelColor(t, 255, 255, 255);
      } else if (map1[yl][t] == 3) {
        CircuitPlayground.setPixelColor(t, 0, 250, 0);
      }
    }
  }
  if (dir == 1) {
    for (int t = 0; t < 10; t++) {  //sets the lights on the board according to the matrix value
      if (map1[t][xl] == 0) {
        CircuitPlayground.setPixelColor(t, 0, 0, 0);
      } else if (map1[t][xl] == 1) {
        CircuitPlayground.setPixelColor(t, 250, 20, 0);
      } else if (map1[t][xl] == 2) {
        CircuitPlayground.setPixelColor(t, 255, 255, 255);
      } else if (map1[t][xl] == 3) {
        CircuitPlayground.setPixelColor(t, 0, 250, 0);
      }
    }
  }
}