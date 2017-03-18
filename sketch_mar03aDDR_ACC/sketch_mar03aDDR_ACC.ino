/*********************************************************************
  下記のサイトに情報が掲載されています。
  http://www.geocities.jp/bokunimowakaru/diy/xbee/xbee-arduino_3.html
  本ソースリストおよびソフトウェアは、ライセンスフリーです。
  利用、編集、再配布等が自由に行えますが、著作権表示の改変は禁止します。
                               Copyright (c) 2014 Wataru KUNINO
                               http://www.geocities.jp/bokunimowakaru/
  本スケッチの作成に下記の資料を参照しました。
  http://playground.arduino.cc/Main/DirectDriveLEDMatrix
  Modified by TadaMatz 2016/3/8
  Modified by TadaMatz 2017/3/3

  inspired by https://www.youtube.com/watch?v=Mf6buDkbyAQ

  Chceck GitHub (https://github.com/matzTada/DDRuino) for further information.
  Modified by TadaMatz 2017/3/19
*********************************************************************/

#define WITH_WIRELESS_ACCELO

#define  DISPLAY_COLS  240   // 表示用のバッファサイズ 最大256-16
#define LED_COMMON    1   // LEDの極性。カソードがROW側(Aタイプ)は0、COL側(Bタイプ)は1

// マトリクスLEDの1番ピンから16番ピンをArduinoのどのDigital入力ピンに接続するかを定義しzます。
// Arduinoの使いやすいピンをLEDに接続することが出来ます。14～19番ピンはAnalog 0～5に相当します。
// 下記のpins[17]の-1よりも後の数字を書き換えてください。

const static
//             /*LED    */  --  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16
byte pins[17] = /*Arduino*/ { -1, 17, 16, 15, 14, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13};

// マトリクスLEDのピン番号と表示位置をLEDの仕様書から書き写してください。
// colsは列、rowsは行です。それぞれpins[番号]のように書きます。
const static
byte cols[8] = {pins[13], pins[ 3], pins[ 4], pins[10], pins[ 6], pins[11], pins[15], pins[16]};
const static
byte rows[8] = {pins[ 9], pins[14], pins[ 8], pins[12], pins[ 1], pins[ 7], pins[ 2], pins[ 5]};

#define BUTTON_PIN 1

#define JOY_STICK_X_PIN A4
#define JOY_STICK_Y_PIN A5

int initialPattern[8][8] = {
  {1, 1, 0, 0, 1, 1, 0, 0},
  {1, 0, 1, 0, 1, 0, 1, 0},
  {1, 0, 1, 0, 1, 0, 1, 0},
  {1, 1, 0, 0, 1, 1, 0, 0},
  {0, 0, 1, 1, 0, 0, 1, 1},
  {0, 0, 1, 0, 1, 0, 1, 1},
  {0, 0, 1, 1, 0, 0, 0, 0},
  {0, 0, 1, 0, 1, 0, 1, 1},
};

int oldButtonState = 0;

#include "header.h"
Shape shapes[4] = {Shape("UP", s1, 0, 1), Shape("RIGHT", s2, 1, 0), Shape("DOWN", s3, 0, -1), Shape("LEFT", s4, -1, 0)};

int successCount = 12;

unsigned long pastMillis = millis();
#define CHANGE_INTERVAL 5000

unsigned long startMillis = millis();
#define GAME_INTERVAL 15000

int currentPattern = random(0, 4);
int currentMode = random(0, 2);

void setup() {
  byte i;
  for (i = 1; i <= 16; i++) pinMode(pins[i], OUTPUT);     // sets the pins as output
  for (i = 1; i <= 8; i++) digitalWrite(cols[i - 1], HIGH);   // set up cols and rows
  for (i = 1; i <= 8; i++) digitalWrite(rows[i - 1], LOW);

  browDisplay(3);
  displayPattern(initialPattern, 3000, 1);
  reset();
  browDisplay(3);
  pastMillis = millis();
  currentPattern = random(0, 4);
  startMillis = millis();

#ifdef WITH_WIRELESS_ACCELO
  Serial.begin(9600);
#endif
}

void loop() {
  float dx = 0;
  float dy = 0;
#ifdef WITH_WIRELESS_ACCELO
  if (Serial.available() > 0) {
    String receiveStr = "";
    receiveStr = Serial.readStringUntil('\n');
    Serial.println("receive");
    Serial.println(receiveStr);
    dx = receiveStr.substring(0, receiveStr.indexOf(',')).toInt();
    dy = receiveStr.substring(receiveStr.indexOf(',') + 1, receiveStr.indexOf('\n')).toInt();
  } else {
    dx = map(analogRead(JOY_STICK_X_PIN), 0, 1023, -10, 10);
    dy = map(analogRead(JOY_STICK_Y_PIN), 0, 1023, 10, -10);
  }
#else
  dx = map(analogRead(JOY_STICK_X_PIN), 0, 1023, -10, 10);
  dy = map(analogRead(JOY_STICK_Y_PIN), 0, 1023, 10, -10);
#endif

  //  Serial.print(dx);
  //  Serial.print(" ");
  //  Serial.print(dy);
  //  Serial.println("");

  displayPattern(shapes[currentPattern].graph, 10, currentMode);

  if (
    ((currentMode == 1)
     && (dx * shapes[currentPattern].dx + dy * shapes[currentPattern].dy)
     / (sqrt(dx * dx + dy * dy)
        * sqrt(shapes[currentPattern].dx * shapes[currentPattern].dx + shapes[currentPattern].dy * shapes[currentPattern].dy))
     > 0.80)
    ||
    (currentMode == 0)
    && (dx * shapes[currentPattern].dx + dy * shapes[currentPattern].dy)
    / (sqrt(dx * dx + dy * dy)
       * sqrt(shapes[currentPattern].dx * shapes[currentPattern].dx + shapes[currentPattern].dy * shapes[currentPattern].dy))
    < -0.80
  ) { //using laws of cosines
    successCount++;
    browDisplay(1);
    pastMillis = millis();
    currentPattern = random(0, 4);
    currentMode = random(0, 2);
  }

#ifdef WITH_WIRELESS_ACCELO
  if (millis() - startMillis > GAME_INTERVAL) {
#else
  if (millis() - startMillis > GAME_INTERVAL || clickDetection() == 1) {
#endif
    displayResult(5000);

    browDisplay(3);
    displayPattern(initialPattern, 3000, 1);
    reset();
    browDisplay(3);
    pastMillis = millis();
    currentPattern = random(0, 4);
    currentMode = random(0, 2);
    startMillis = millis();
  }
}

void displayPattern(int pattern[8][8], int showTime, int mode) {
  unsigned long tmpMillis = millis();
  while (millis() - tmpMillis < showTime) {
    for ( int cat = 0; cat < 8; cat++ )
    {
      digitalWrite( cols[cat], LOW ); // LOWに変更
      // アノードのループ
      for ( int ano = 0; ano < 8; ano++ )
      {
        if (mode == 1) { //normal
          digitalWrite(rows[ano], pattern[ano][cat]);  // HIGH or LOW
        }
        else { //normal
          digitalWrite(rows[ano], 1 - pattern[ano][cat]);  // HIGH or LOW
        }
      }
      delay(1); //点灯時間
      for ( int ano = 0; ano < 8; ano++ )
      {
        digitalWrite( rows[ano], LOW ); // LOWに戻す
      }
      digitalWrite( cols[cat], HIGH );  // HIGHに戻す
    }
  }
}

void displayResult(int showTime) {
  int tempPattern[8][8] = {0};
  int tempCount = 0;
  for (int j = 0; j < 8; j++) {
    for (int i = 0; i < 8; i++) {
      if (tempCount++ < successCount) tempPattern[i][j] = 1;
      else break;
    }
  }
  displayPattern(tempPattern, showTime, 1);
}

void reset() {
  successCount = 0;
}

void browDisplay(int showTime) {
  for ( int i = 0; i < 8; i += 1 ) {
    for ( int j = 0; j < 8; j += 1) {
      cell(i, j, HIGH);
      delay(showTime);
      cell(i, j, LOW);
    }
  }

  for ( int j = 0; j < 8; j += 1) {
    for ( int i = 0; i < 8; i += 1 ) {
      cell(i, j, HIGH);
      delay(showTime);
      cell(i, j, LOW);
    }
  }
}

int clickDetection() { //click detection
  int buttonClicked = 0;
  int newButtonState = digitalRead(BUTTON_PIN);
  if (oldButtonState == HIGH && newButtonState == LOW) buttonClicked = 1; //button pressed. CAUTION pull up
  oldButtonState = newButtonState;
  return buttonClicked;
}
