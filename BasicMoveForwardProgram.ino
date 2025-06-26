//Basic Moving Forward Program
//I2C通信プロトコルを扱うためのwire.h。これによりSDA(データ線)とSCL（クロック線）の使用が可能になる。
//この場合ではPWMサーボドライバとの通信に必要
#include <Wire.h>
//PWM Servo Driver(PCA9685)を制御するためのライブラリ。通常は60Hz
#include <Adafruit_PWMServoDriver.h>
//pwmというインスタンスを作成して、PCA9685と通信する準備を整える。
//アドレスはデフォルトで0x40となる。
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

void setup() {
  //I2C通信の初期化
  Serial.begin(9600);
  pwm.begin();
  //周波数を指定。
  pwm.setPWMFreq(60);
  delay(10);

  // set1：ニュートラル
  moveHipNeutralL(0); 
  moveHipNeutralR(2);
  //moveHipNeutralL(4); 
  moveHipNeutralL(8);
  moveHipNeutralR(6);
  putDownLegL(1); 
  putDownLegR(3); 
  putDownLegL(5); 
  putDownLegR(7);
  delay(200);
}

// サーボ用のパルス幅を定義（要調整）
//.setPWMは0~4095の範囲。
//ある資料では150~600が0°～180°のマッピングであった
const int HIP1_NEUTRAL = 263; //Left Side
const int HIP1_FORWARD = 150;
const int HIP1_BACKWARD = 375;

const int HIP2_NEUTRAL = 263; //Right Side
const int HIP2_FORWARD = 375;
const int HIP2_BACKWARD = 150;

const int KNEE_UPL = 150;//左足
const int KNEE_DOWNL = 350;
const int KNEE_UPR = 350;//右足
const int KNEE_DOWNR = 150;

const int DEFAULT1 =0;  //基本的に0固定。

const int delay1=300;
const int delay2=600;
//const int DEFAULT2 =600;  


// レッグ番号とサーボ番号
// FL: 0(hip),1(knee) / FR:2,3 / BL:4,5 / BR:6,7

// 左足を持ち上げる
void liftLegL(int kneeChannel) {
  pwm.setPWM(kneeChannel,DEFAULT1, KNEE_UPL);
}

// 左足を下ろす
void putDownLegL(int kneeChannel) {
  pwm.setPWM(kneeChannel, DEFAULT1, KNEE_DOWNL);
}
// 右足を持ち上げる
void liftLegR(int kneeChannel) {
  pwm.setPWM(kneeChannel,DEFAULT1, KNEE_UPR);
}

// 右足を下ろす
void putDownLegR(int kneeChannel) {
  pwm.setPWM(kneeChannel, DEFAULT1, KNEE_DOWNR);
}


// ヒップを前に振る
void moveHipForwardL(int hipChannel) {
  pwm.setPWM(hipChannel, DEFAULT1, HIP1_FORWARD);
}
void moveHipForwardR(int hipChannel) {
  pwm.setPWM(hipChannel, DEFAULT1, HIP2_FORWARD);
}



// ヒップを前からニュートラルへに振る
void moveHipBackwardL1(int hipChannel) {
  pwm.setPWM(hipChannel, DEFAULT1, HIP1_NEUTRAL);
}
// ヒップをニュートラルから後ろに振る
void moveHipBackwardL2(int hipChannel) {
  pwm.setPWM(hipChannel, DEFAULT1, HIP1_BACKWARD);
}

// ヒップを前からニュートラルへに振る
void moveHipBackwardR1(int hipChannel) {
  pwm.setPWM(hipChannel, DEFAULT1, HIP2_NEUTRAL);
}
// ヒップをニュートラルから後ろに振る
void moveHipBackwardR2(int hipChannel) {
  pwm.setPWM(hipChannel, DEFAULT1, HIP2_BACKWARD);
}




// ヒップを中立に戻す
void moveHipNeutralL(int hipChannel) {
  pwm.setPWM(hipChannel, DEFAULT1, HIP1_NEUTRAL);
}

void moveHipNeutralR(int hipChannel) {
  pwm.setPWM(hipChannel, DEFAULT1, HIP2_NEUTRAL);
}

void loop() {
  creepGaitStep();
}

// クリープゲイトの1周期
void creepGaitStep() {

  // step4：右後脚（BR=6,7）を前に
  liftLegR(7);
  moveHipForwardR(6);
  delay(delay1);
  putDownLegR(7);
  delay(delay2);
  // step1：右前脚（FR=2,3）を前に
  liftLegR(3);
  moveHipForwardR(2);
  delay(delay1);
  putDownLegR(3);
  delay(delay2);

  // move1：本体押し出し
  moveHipBackwardL2(0);
  moveHipBackwardR1(6);
  //moveHipBackwardL2(4);
  moveHipBackwardL2(8);
  moveHipBackwardR1(2);


  // step2：左後脚（BL=4,5）を前に
  liftLegL(5);
  //moveHipForwardL(4);
  moveHipForwardL(8);
  delay(delay1);
  putDownLegL(5);
// step3：左前脚（FL=0,1）を前に
  delay(delay2);
  liftLegL(1);
  moveHipForwardL(0);
  delay(delay1);
  putDownLegL(1);
  delay(delay2);

  // move2：本体押し出し
  delay(delay2);
  moveHipBackwardL1(0);
  moveHipBackwardR2(6);
  //moveHipBackwardL1(4);
  moveHipBackwardL1(8);
  moveHipBackwardR2(2);
  delay(delay2);

}

/*
フェーズ	動作
stable 静止状態（特定の脚の開き/閉じ）
step1	右前脚（または右側の脚）を浮かせて前へ伸ばす
move1	全脚を地面に着けたまま後ろへ押し出す（＝体が前へ進む）
set2	左後脚を浮かせて引き寄せる
step2	左前脚（または左側の脚）を浮かせて前へ伸ばす
move2	全脚でまた本体を前に押し出す
step3 右後脚を浮かせて引き寄せる
*/

