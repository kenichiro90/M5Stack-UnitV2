/*
  Description: Use GoPLUS Module for four-channel servo test and three-channel ADC test.
*/
#include <M5Stack.h>
#include <Wire.h>
#include "GoPlus2.h"
#include <math.h>

// シリアル通信("M5Stack <-> M5UnitV2")で使用するポート番号の定義
#define rxPin 16
#define txPin 17

// クラスの初期化
GoPlus2 goPlus;


void header(const char *string, uint16_t color)
{
  // LCDにヘッダ文字を表示させる
  M5.Lcd.fillScreen(color);
  M5.Lcd.setTextSize(1);
  M5.Lcd.setTextColor(TFT_WHITE, TFT_BLACK);
  M5.Lcd.fillRect(0, 0, 320, 30, TFT_BLACK);
  M5.Lcd.setTextDatum(TC_DATUM);
  M5.Lcd.drawString(string, 160, 3, 4);
}

void Motor()
{
  // 変数の初期化
  unsigned char buf_speed = 0;
  short spd_mot_0 = 0;
  short spd_mot_1 = 0;

  while (1) {
    // デバイス状態の取得
    M5.update();

    // 受信バッファにデータが溜まっていた場合、1文字ずつ受信して配列に詰める
    if(Serial2.available() > 0) {
      byte data_size = Serial2.available();
      byte buf[data_size];
      for(byte i = 0; i < data_size; i++) {
        buf[i] = Serial2.read();
      }

      // 受信したデータを解析して、各コマンドを実行する
      if(buf[0] == 'R' && buf[1] == 'W' && buf[2] == 'D') {
        // -----
        // 後退コマンド: "RWD[SPD_MOT_0][SPD_MOT_1]"
        // -----
        //   "[SPD_MOT_0], [SPD_MOT_1]"は、1文字のデータ(char)とする。
        //   文字データのエンコード形式は、"ASCII"とする。
        //   "[SPD_MOT_0], [SPD_MOT_1]"で、モーターの回転速度を指定できる。
        //   モーターの回転速度は"0~255"の範囲で指定できるが、上限値("60")を超えた場合、強制的に"60"に設定される。
        // -----

        // 進行方向を表示する(シリアルポート)
        Serial.println("DIR: RWD");

        // モーターの回転速度の読み取り
        spd_mot_0 = buf[3];
        spd_mot_1 = buf[4];

        // 上限値("60")を超えた場合、回転速度を"60"に設定する
        if(spd_mot_0 > 60) spd_mot_0 = 60;
        if(spd_mot_1 > 60) spd_mot_1 = 60;

        // モーターの回転方向を変更する
        //spd_mot_0 = spd_mot_0;
        spd_mot_1 = spd_mot_1 * (-1);
 
        // モーターの回転速度を表示する(シリアルポート)
        Serial.println(spd_mot_0);
        Serial.println(spd_mot_1);

        // 進行方向とモーターの回転速度を表示する(LCD)
        M5.Lcd.fillRect(0, 40, 320, 100, TFT_BLACK);
        M5.Lcd.setCursor(20, 40, 4);
        M5.Lcd.println("Motor Direction: RWD");
        M5.Lcd.setCursor(20, 60, 4);
        M5.Lcd.print("Motor Speed: ");
        M5.Lcd.println(spd_mot_0);

        // モーターの回転を開始する
        goPlus.Motor_write_speed(MOTOR_NUM0, spd_mot_0);
        goPlus.Motor_write_speed(MOTOR_NUM1, spd_mot_1);
      } else if(buf[0] == 'F' && buf[1] == 'W' && buf[2] == 'D') {
        // -----
        // 前進コマンド: "FWD[SPD_MOT_0][SPD_MOT_1]"
        // -----
        //   "[SPD_MOT_0], [SPD_MOT_1]"は、1文字のデータ(char)とする。
        //   文字データのエンコード形式は、"ASCII"とする。
        //   "[SPD_MOT_0], [SPD_MOT_1]"で、モーターの回転速度を指定できる。
        //   モーターの回転速度は"0~255"の範囲で指定できるが、上限値("60")を超えた場合、強制的に"60"に設定される。
        // -----

        // 進行方向を表示する(シリアルポート)
        Serial.println("DIR: FWD");

        // モーターの回転速度の読み取り
        spd_mot_0 = buf[3];
        spd_mot_1 = buf[4];

        // 上限値("60")を超えた場合、回転速度を"60"に設定する
        if(spd_mot_0 > 60) spd_mot_0 = 60;
        if(spd_mot_1 > 60) spd_mot_1 = 60;

        // モーターの回転方向を変更する
        spd_mot_0 = spd_mot_0 * (-1);
        //spd_mot_1 = spd_mot_1;

        // モーターの回転速度を表示する(シリアルポート)
        Serial.println(spd_mot_0);
        Serial.println(spd_mot_1);

        // 進行方向とモーターの回転速度を表示する(LCD)
        M5.Lcd.fillRect(0, 40, 320, 100, TFT_BLACK);
        M5.Lcd.setCursor(20, 40, 4);
        M5.Lcd.println("Motor Direction: FWD");
        M5.Lcd.setCursor(20, 60, 4);
        M5.Lcd.print("Motor Speed: ");
        M5.Lcd.println(spd_mot_1);

        // モーターの回転を開始する
        goPlus.Motor_write_speed(MOTOR_NUM0, spd_mot_0);
        goPlus.Motor_write_speed(MOTOR_NUM1, spd_mot_1);
      } else if(buf[0] == 'S' && buf[1] == 'T' && buf[2] == 'P') {
        // -----
        // 停止コマンド: "STP"
        // -----
        //   文字データのエンコード形式は、"ASCII"とする。
        // -----

        // 進行方向を表示する(シリアルポート)
        Serial.println("DIR: STP");

        // 進行方向とモーターの回転速度を表示する(LCD)
        M5.Lcd.fillRect(0, 40, 320, 100, TFT_BLACK);
        M5.Lcd.setCursor(20, 40, 4);
        M5.Lcd.println("Motor Direction: STP");
        M5.Lcd.setCursor(20, 60, 4);
        M5.Lcd.println("Motor Speed: 0");

        // モーターの回転を停止する
        goPlus.Motor_write_speed(MOTOR_NUM0, 0);
        goPlus.Motor_write_speed(MOTOR_NUM1, 0);
      }
    }

    // 10msec毎に、受信バッファの確認と受信データの解析を実行する
    delay(10);
  }
}

void setup()
{
  // デバイスの初期化(M5Stack)
  M5.begin();

  // シリアル通信の開始
  Serial.begin(115200);                                                 // デバッグ用
  Serial2.begin(115200, SERIAL_8N1, rxPin, txPin);                      // "M5Stack <-> M5UnitV2"用

  // デバイスの初期化(GOPLUS2モジュール)
  goPlus.begin();

  // 少し待つ(100msec)
  delay(100);

  // LCDにヘッダ文字を表示させる
  header("GoPlus 2", TFT_BLACK);
  M5.Lcd.setTextColor(TFT_GREEN, TFT_BLACK);
}

void loop()
{
  // モーター制御の実行
  Motor();
}
