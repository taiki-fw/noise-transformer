const int micLeftPin = A0;
const int micRightPin = A1;

const int motorLeftPin = 9;
const int motorRightPin = 10;

void setup() {
  //9ピンを出力ピンに設定
  pinMode(motorLeftPin, OUTPUT);
  pinMode(motorRightPin, OUTPUT);
  //シリアル通信を開始します
  Serial.begin(115200);
}

void loop() {
  Serial.print("start ");
	digitalWrite(motorLeftPin, HIGH);
  delay(500);
  Serial.print("stop\n");
  digitalWrite(motorLeftPin, LOW);
  delay(500); 
} 