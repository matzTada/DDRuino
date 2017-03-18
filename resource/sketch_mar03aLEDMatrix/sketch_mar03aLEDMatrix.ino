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
*********************************************************************/

#define  DISPLAY_COLS  240   // 表示用のバッファサイズ 最大256-16
#define LED_COMMON    1   // LEDの極性。カソードがROW側(Aタイプ)は0、COL側(Bタイプ)は1

// マトリクスLEDの1番ピンから16番ピンをArduinoのどのDigital入力ピンに接続するかを定義します。
// Arduinoの使いやすいピンをLEDに接続することが出来ます。14～19番ピンはAnalog 0～5に相当します。
// 下記のpins[17]の-1よりも後の数字を書き換えてください。

const static
//             /*LED    */  --  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16
byte pins[17] = /*Arduino*/ { -1, 19, 18, 17, 16, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13};

// マトリクスLEDのピン番号と表示位置をLEDの仕様書から書き写してください。
// colsは列、rowsは行です。それぞれpins[番号]のように書きます。
const static
byte cols[8] = {pins[13], pins[ 3], pins[ 4], pins[10], pins[ 6], pins[11], pins[15], pins[16]};
const static
byte rows[8] = {pins[ 9], pins[14], pins[ 8], pins[12], pins[ 1], pins[ 7], pins[ 2], pins[ 5]};

#define BUTTON_PIN 1

int flashMoji[8][8] = {
  {1, 1, 0, 0, 0, 1, 0, 0},
  {1, 0, 1, 0, 1, 0, 1, 0},
  {1, 0, 1, 0, 1, 1, 1, 0},
  {1, 1, 0, 0, 1, 0, 1, 0},
  {0, 1, 1, 1, 0, 0, 1, 0},
  {0, 0, 1, 0, 0, 1, 0, 1},
  {0, 0, 1, 0, 0, 1, 1, 1},
  {0, 0, 1, 0, 0, 1, 0, 1},
};

int oldButtonState = 0;

void setup() {
  byte i;
  for (i = 1; i <= 16; i++) pinMode(pins[i], OUTPUT);     // sets the pins as output
  for (i = 1; i <= 8; i++) digitalWrite(cols[i - 1], HIGH);   // set up cols and rows
  for (i = 1; i <= 8; i++) digitalWrite(rows[i - 1], LOW);

  //  Serial.begin(9600);
  //  Serial.println("LED MATRIX SERIAL COME ON! SERIAL");

  flashLeds();
}

void cell(int x, int y, int state) {
  if (state == HIGH) {
    digitalWrite(rows[y], HIGH);
    digitalWrite(cols[x], LOW);
  } else {
    digitalWrite(rows[y], LOW);
    digitalWrite(cols[x], HIGH);
  }
}

void loop() {
  //  flashLeds();

  int x = (int)map(analogRead(A0), 0, 1023, 0, 7);
  int y = (int)map(analogRead(A1), 0, 1023, 0, 7);
  //  Serial.print(x);
  //  Serial.print(" ");
  //  Serial.print(y);
  //  Serial.println("");

  cell(x, y, HIGH);
  delay(5);
  cell(x, y, LOW);
  delay(5);

  if (clickDetection() == 1) {
    flashLeds();
  }
}

void flashLeds() {
  for ( int i = 0; i < 8; i += 1 ) {
    for ( int j = 0; j < 8; j += 1) {
      cell(i, j, HIGH);
      delay(5);
      cell(i, j, LOW);
    }
  }

  for ( int j = 0; j < 8; j += 1) {
    for ( int i = 0; i < 8; i += 1 ) {
      cell(i, j, HIGH);
      delay(5);
      cell(i, j, LOW);
    }
  }

  //flashMoji loop
  for (int i = 0; i < 10 ; i ++) {
    for ( int cat = 0; cat < 8; cat++ )
    {
      digitalWrite( cols[cat], LOW ); // LOWに変更
      // アノードのループ
      for ( int ano = 0; ano < 8; ano++ )
      {
        digitalWrite( rows[ano], flashMoji[ano][cat] );  // HIGH or LOW
      } 
      delay(10); //点灯時間
      for ( int ano = 0; ano < 8; ano++ )
      {
        digitalWrite( rows[ano], LOW ); // LOWに戻す
      }
      digitalWrite( cols[cat], HIGH );  // HIGHに戻す
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
