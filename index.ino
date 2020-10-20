//ピンを宣言します
const int micLeftPin = A0;
const int micRightPin = A1;

const int motorLeftPin = 8;
const int motorRightPin = 9;

const int threshold = 300;
boolean isProcessing = false;
const int allowableLimitError = 10;

// State
int elem = 5;
int rAvgArr[5] = {0, 0, 0, 0, 0};
int rSum = 0;
int lAvgArr[5] = {0, 0, 0, 0, 0};
int lSum = 0;

void setup() {
  //9ピンを出力ピンに設定
  pinMode(motorLeftPin, OUTPUT);
  pinMode(motorRightPin, OUTPUT);
  //シリアル通信を開始します
  Serial.begin(115200);
}

void swapArr(int newRightValue, int newLeftValue) {
  for(int i=0; i < elem - 1; i++) {
    rAvgArr[i] = rAvgArr[i+1];
    lAvgArr[i] = lAvgArr[i+1];
  }
  rAvgArr[elem - 1] = newRightValue;
  lAvgArr[elem - 1] = newLeftValue;

  return;
}

void vib(int motorPin, int pattern) {
  for(int i = 0; i < pattern; i++){
    Serial.print("start ");
	  digitalWrite(motorPin, HIGH);
    delay(500);
	  Serial.print("stop\n");
    digitalWrite(motorPin, LOW);
    delay(500); 
  }
}

void debugCode(double rAvg, double lAvg) {
  String rightResult = "[";
  for(int i = 0; i < elem; i++) {
    rightResult += rAvgArr[i];
    rightResult += ' ';
  }
  rightResult += "]";
  rightResult += "rAvg ";
  rightResult += (String)rAvg;

  String leftResult = "[";
  for(int i = 0; i < elem; i++) {
    leftResult += lAvgArr[i];
    leftResult += ' ';
  }
  leftResult += "]";
  leftResult += "lAvg ";
  leftResult += (String)lAvg;

  Serial.println(rightResult);
  Serial.println(leftResult);
}

void loop() {
  if(isProcessing == true) {
    return;
  }
  
  char state = 'S';
  
  //マイクの音量(電圧)を測り、micValueという場所(変数)に入れておきます
  int micLeftValue = abs(analogRead(micLeftPin));
  int micRightValue = abs(analogRead(micRightPin));
  
  rSum = rSum + (micRightValue - rAvgArr[0]);
  lSum = lSum + (micLeftValue - lAvgArr[0]);
    
  swapArr(micRightValue, micLeftValue);
  
  double rAvg = (double)rSum / (double)elem;
  double lAvg = (double)lSum / (double)elem;

  debugCode(rAvg, lAvg);
  
  //Left or Right
  if((abs(lAvg - rAvg) <= allowableLimitError) && ((lAvg >= threshold) && (rAvg >= threshold))) {
    isProcessing = true;
    state = 'B';
  }else if(rAvg >= threshold && rAvg > lAvg) {
    isProcessing = true;
    state = 'R';
  }else if(lAvg >= threshold && lAvg > rAvg) {
    isProcessing = true;
    state = 'L';
  }
  
  
  switch (state) {
  case 'B':
    Serial.println("both ... both");
    vib(motorRightPin, 1);
    vib(motorLeftPin, 1);
    break;
  case 'R':
    Serial.println("right ... right");
    vib(motorRightPin, 1);
    break;
  case 'L':
    Serial.println("left ... left");
    vib(motorLeftPin, 1); 
    break;
  default:
    Serial.println("nothing");
  }
  
  if(isProcessing == true) {
    delay(1000);
    isProcessing = false;
  }
  
  Serial.println("");
}