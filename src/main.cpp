 #include <Arduino.h>

#include <SD.h>
#include <Ticker.h>
#include <M5StackUpdater.h>
#include <M5Unified.h>

#if defined(ARDUINO_M5STACK_Core2)
  // M5Stack Core2用のサーボの設定
  // Port.A X:G33, Y:G32
  // Port.C X:G13, Y:G14
  // スタックチャン基板 X:G19, Y:G27
  #define SERVO_PIN_X 33
  #define SERVO_PIN_Y 32
#elif defined( ARDUINO_M5STACK_FIRE )
  // M5Stack Fireの場合はPort.A(X:G22, Y:G21)のみです。
  // I2Cと同時利用は不可
  #define SERVO_PIN_X 22
  #define SERVO_PIN_Y 21
#elif defined( ARDUINO_M5Stack_Core_ESP32 )
  // M5Stack Basic/Gray/Go用の設定
  // Port.A X:G22, Y:G21
  // Port.C X:G16, Y:G17
  // スタックチャン基板 X:G5, Y:G2
  #define SERVO_PIN_X 22
  #define SERVO_PIN_Y 21
#elif defined( ARDUINO_M5STACK_CORES3 )
  // M5Stack CoreS3用の設定 ※暫定的にplatformio.iniにARDUINO_M5STACK_CORES3を定義しています。
  // Port.A X:G1 Y:G2
  // Port.B X:G8 Y:G9
  // Port.C X:18 Y:17
  #define SERVO_PIN_X 18 
  #define SERVO_PIN_Y 17
  #include <gob_unifiedButton.hpp> // 2023/5/12現在 M5UnifiedにBtnA等がないのでGobさんのライブラリを使用
  gob::UnifiedButton unifiedButton;
#endif

#include <Avatar.h> // https://github.com/meganetaaan/m5stack-avatar
#include <ESP32Servo.h>

using namespace m5avatar;
Avatar avatar;

#define TIMER_WIDTH 16

/**
 * GeekServo (https://nagomi0132.hateblo.jp/entry/2023/05/02/213233)
 * 時計回り   : 500 - 1500US : 0度
 * 停止       : 1500US : 90度
 * 反時計周り : 1500 - 2500US : 180度
*/
Servo servo_A;
Servo servo_B;

bool GO_FRONT = true;

// 停止
void moveStop() {
  servo_A.write(90);
  servo_B.write(90);
  GO_FRONT = true;
}

// 前進
void moveFront() {
    moveStop();
    delay(500);
    servo_A.write(0);
    servo_B.write(0);
    GO_FRONT = false;
}

// 後退
void moveBack() {
    moveStop();
    delay(500);
    servo_A.write(180);
    servo_B.write(180);
    GO_FRONT = true;
}

// ランダムモード
void moveRandom() {
  for (;;) {
    moveStop();
    delay(500);
    int rnd_speed = random(0, 6) * 30;
    M5.update();
    if (M5.BtnC.wasPressed()) {
      break;
    }
    servo_A.write(rnd_speed);
    servo_B.write(rnd_speed);
    int delay_time = random(10);
    delay(1000 + 50 * delay_time);
  }
  M5.Speaker.tone(2500, 500);
  GO_FRONT = true;
}

// setup
void setup() {

  auto cfg = M5.config();     // 設定用の情報を抽出
  cfg.output_power = true;    // Groveポートの出力をしない
  M5.begin(cfg);              // M5Stackをcfgの設定で初期化

  M5.Log.setLogLevel(m5::log_target_display, ESP_LOG_NONE);
  M5.Log.setLogLevel(m5::log_target_serial, ESP_LOG_INFO);
  M5.Log.setEnableColor(m5::log_target_serial, false);

  M5_LOGI("Hello World");
  Serial.println("HelloWorldSerial");
  //USBSerial.println("HelloWorldUSBSerial");

#if defined( ARDUINO_M5STACK_FIRE )
  // M5Stack Fireの場合、Port.Aを使う場合は内部I2CをOffにする必要がある。
  M5.In_I2C.release();
#endif
  servo_A.setPeriodHertz(50);
  servo_B.setPeriodHertz(50);
  if (servo_A.attach(SERVO_PIN_X, 500, 2500)) {
    Serial.print("Error attaching servo A");
  }
  if (servo_B.attach(SERVO_PIN_Y, 500, 2500)) {
    Serial.print("Error attaching servo B");
  }

  avatar.init();
  // avatar.setBatteryIcon(true);

}

// loop
void loop() {

  M5.update();

  // 前進/後退
  if (M5.BtnA.wasPressed()) {
    Serial.print("Button A : Go Front /Back");
    M5.Speaker.tone(1000, 100);
    if(GO_FRONT) {
      moveFront();
    } else {
      moveBack();
    }
  }

  // 停止
  if (M5.BtnB.wasPressed()) {
    Serial.print("Button B : Stop");
    M5.Speaker.tone(1500, 100);
    moveStop();
  }

  // ランダム
  if (M5.BtnC.wasPressed()) {
    Serial.print("Button C : Rumdom Move");
    M5.Speaker.tone(2000, 100);
    moveRandom();
  }
}