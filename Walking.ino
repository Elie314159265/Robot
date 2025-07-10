#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

// PWM制御用ライブラリ
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

// 超音波センサー用のピン
const int trigPin = 9;
const int echoPin = 10;

// サーボ用パルス幅
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

// 障害物検出の閾値（cm）
const int OBSTACLE_THRESHOLD = 50;

// setup
void setup() {
  Serial.begin(9600);
  pwm.begin();
  pwm.setPWMFreq(60);
  delay(10);

  // set1：ニュートラル
  moveHipNeutralL(0); 
  moveHipNeutralR(2);
  moveHipNeutralL(4); 
  moveHipNeutralR(6);
  putDownLegL(1); 
  putDownLegR(3); 
  putDownLegL(5); 
  putDownLegR(7);
  delay(200);
  
  // 超音波センサーのピン設定
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

// loop
void loop() {
  delay(200);
  long distance = getDistance();  // 障害物までの距離を測定


  Serial.print("Distance: ");
  Serial.print(distance);
  //Serial.println(" cm");

  // 障害物が近い場合、回避動作を行う
  if (distance < OBSTACLE_THRESHOLD) {
    Serial.println("Obstacle detected! Turning...");
    //avoidObstacle();  // 障害物回避
  } else {
    Serial.println("No obstacle. Moving forward...");
    //moveForward();  // 障害物がなければ前進
  }

  delay(500);
}

// 超音波センサーで距離を測定する関数
long getDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  long duration = pulseIn(echoPin, HIGH);
  long distance = duration * 0.034 / 2;
  
  return distance;
}

// 前進動作
void moveForward() {
  // step1：右前脚（FR=2,3）を前に
  delay(delay2);
  liftLegR(3);
  moveHipForwardR(2);
  delay(delay1);
  putDownLegR(3);
  delay(delay2);

  // move1：本体押し出し
  moveHipBackwardL2(0);
  moveHipBackwardR1(6);
  moveHipBackwardL2(4);
  moveHipBackwardR1(2);

  // step2：左後脚（BL=4,5）を前に
  delay(delay2);
  liftLegL(5);
  moveHipForwardL(4);
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
  moveHipBackwardL1(0);
  moveHipBackwardR2(6);
  moveHipBackwardL1(4);
  moveHipBackwardR2(2);

  // step4：右後脚（BR=6,7）を前に
  delay(delay2);
  liftLegR(7);
  moveHipForwardR(6);
  delay(delay1);
  putDownLegR(7);
}

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

// 右旋回（時計回り）
void turnRightStep() {
  //1
  // 右前脚（FR）後ろに
  delay(delay2);
  liftLegR(3);
  moveHipBackwardR2(2);
  delay(delay1);
  putDownLegR(3);
  delay(delay2);

  // 左後脚（BL）前に
  liftLegL(5);
  //moveHipForwardL(4);
  moveHipForwardL(8);
  delay(delay1);
  putDownLegL(5);

// 右後脚（BR）後ろに
  delay(delay2);
  liftLegR(7);
  moveHipBackwardR2(6);
  delay(delay1);
  putDownLegR(7);


  delay(delay2);
  // 左前脚（FL）前に
  liftLegL(1);
  moveHipForwardL(0);
  delay(delay1);
  putDownLegL(1);
  delay(delay2);

  // 押し出し
  moveHipBackwardL2(0);
  //moveHipBackwardL2(4);
  moveHipBackwardL2(8);
  moveHipForwardR(2);
  moveHipForwardR(6);

  //2
  // 右前脚（FR）後ろに
  delay(delay2);
  liftLegR(3);
  moveHipBackwardR2(2);
  delay(delay1);
  putDownLegR(3);
  delay(delay2);

  // 左後脚（BL）前に
  liftLegL(5);
  //moveHipForwardL(4);
  moveHipForwardL(8);
  delay(delay1);
  putDownLegL(5);

// 右後脚（BR）後ろに
  delay(delay2);
  liftLegR(7);
  moveHipBackwardR2(6);
  delay(delay1);
  putDownLegR(7);
  delay(delay2);


  // 左前脚（FL）前に
  liftLegL(1);
  moveHipForwardL(0);
  delay(delay1);
  putDownLegL(1);
  delay(delay2);

  // 押し出し
  moveHipBackwardL2(0);
  //moveHipBackwardL2(4);
  moveHipBackwardL2(8);
  moveHipForwardR(2);
  moveHipForwardR(6);
}

// 左旋回（反時計回り）
void turnLeftStep() {
  //1
  // 右前脚（FR）前に
  liftLegR(3);
  moveHipForwardR(2);
  delay(delay1);
  putDownLegR(3);
  delay(delay2);

  // 左後脚（BL）後ろに
  liftLegL(5);
  //moveHipBackwardL2(4);
  moveHipBackwardL2(8);
  delay(delay1);
  putDownLegL(5);
  delay(delay2);

  // 右後脚（BR）前に
  liftLegR(7);
  moveHipForwardR(6);
  delay(delay1);
  putDownLegR(7);
  delay(delay2);

  // 左前脚（FL）後ろに
  liftLegL(1);
  moveHipBackwardL2(0);
  delay(delay1);
  putDownLegL(1);
  delay(delay2);

  // 押し出し
  moveHipBackwardR2(2);
  moveHipBackwardR2(6);
  moveHipForwardL(0);
  moveHipForwardL(8);
  //moveHipForwardL(4);
  delay(delay2);
  //2
  // 右前脚（FR）前に
  liftLegR(3);
  moveHipForwardR(2);
  delay(delay1);
  putDownLegR(3);
  delay(delay2);

  // 左後脚（BL）後ろに
  liftLegL(5);
  //moveHipBackwardL2(4);
  moveHipBackwardL2(8);
  delay(delay1);
  putDownLegL(5);
  delay(delay2);

  // 右後脚（BR）前に
  liftLegR(7);
  moveHipForwardR(6);
  delay(delay1);
  putDownLegR(7);
  delay(delay2);

  // 左前脚（FL）後ろに
  liftLegL(1);
  moveHipBackwardL2(0);
  delay(delay1);
  putDownLegL(1);
  delay(delay2);

  // 押し出し
  moveHipBackwardR2(2);
  moveHipBackwardR2(6);
  moveHipForwardL(0);
  moveHipForwardL(8);
  //moveHipForwardL(4);
  delay(delay2);
}
/*
getDistance() は超音波センサーを使って前方の距離を測定し、距離を返す。

距離が OBSTACLE_THRESHOLD より小さければ障害物を検知したと判断し、avoidObstacle() を呼び出して回避動作を行う。

avoidObstacle() では、右旋回と左旋回を実行し、その後直進します。これにより、障害物を回避します。

障害物がなければ、moveForward() で前進を続けます。

*/
