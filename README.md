# stack-chan-tester-360

本リポジトリは [stack-chan-tester](https://github.com/mongonta0716/stack-chan-tester) を元に、360連続回転サーボの動作確認用に改造したものです。

ｽﾀｯｸﾁｬﾝ-アバターの顔が出る以外は、サーボモーターを動作させる機能しかついていません。

## 使い方

M5Stack (Core2を想定）に、[Stack-chan_Takao_Base](https://www.switch-science.com/products/8905?variant=42653590913222)
や
[ｽﾀｯｸﾁｬﾝ タカオ版 部品キット](https://mongonta.booth.pm/items/3520339)
を接続し、360度回転サーボを付けた状態で起動させてください。

サーボモーターは2個同時に動かせますが、M5Stack Core2単体のバッテリーだけだと正転／負転を切り替えの不可から落ちることが多いです。
そのため外部から給電するか、サーボモーターは1個での使用がよさそうです。

## 想定サーボモーター

- [GeekServo 9G 360° Green](https://www.amazon.co.jp/dp/B0B12P9M4P)
  - 動作確認済み。

- [GeekServo 9G 360° Orange](https://www.switch-science.com/products/6813)
  - 多分Greenと同じ仕様。色が違うだけのはず。
- [FEETECH FS90R](https://www.switch-science.com/products/7113)
  - 未検証だけど仕様的には大差ないはず。

## 実装済み機能

* Aボタン
  * 押すたびに360度回転サーボの正転／負転を切り替えます。
* Bボタン
  * サーボの回転を止めます。
* Cボタン
  * サーボの正転／負転をランダムに切り替えながら動作します。

## 必要なライブラリ（動作確認済バージョン）

※ 最新の情報はplatformio.iniを確認してください。最新で動かない場合はライブラリのバージョンを合わせてみてください。

- [M5Unified](https://github.com/m5stack/M5Unified) v0.1.6で確認
- [M5Stack-Avatar](https://github.com/meganetaaan/m5stack-avatar) v0.8.2で確認<br>v0.7.4以前はM5Unifiedに対応していないのでビルド時にM5の二重定義エラーが出ます。
- [ESP32Servo](https://github.com/madhephaestus/ESP32Servo) v0.13.0で確認

本家のstack-chan-testerでは、サーボモーターの制御に[ServoEasing](https://github.com/ArminJo/ServoEasing)を試用していますが、本プログラムでは360度回転サーボを制御するためのコードには、[ESP32Servo](https://github.com/madhephaestus/ESP32Servo)を使用しています。
