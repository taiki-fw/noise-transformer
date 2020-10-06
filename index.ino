//ピンを宣言します
const int micLeftPin = A0;
const int micRightPin = A1;

const int motorLeftPin = 9;
const int motorRightPin = 10;

const int threshold = 1000;
boolean isProcessing = false;

// State
int count = 5;
int rAvgArr[100] = {0, 0, 0, 0, 0};
int rSum = 0;
int lAvgArr[100] = {0, 0, 0, 0, 0};
int lSum = 0;
int elem = 5;

void setup() {
  //9ピンを出力ピンに設定
  pinMode(motorLeftPin, OUTPUT);
  pinMode(motorRightPin, OUTPUT);
  //シリアル通信を開始します
  Serial.begin(115200);
}

void resetState() {
  count = 5;
  rAvgArr[100] = {0, 0, 0, 0, 0};
  rSum = 0;
  lAvgArr[100] = {0, 0, 0, 0, 0};
  lSum = 0;
}

void show(char state, int value) {
  	if(state == 'L') {  
	  	Serial.print("LeftValue:");
    }else if(state == 'R') {
        Serial.print("RightValue:");
    }
  	Serial.print(abs(value));
  	Serial.println("");
}

void vib(int motorPin, int pattern) {
  for(int i = 0; i < pattern; i++){
    Serial.print("start");
	digitalWrite(motorPin, HIGH);
    delay(500);
	Serial.print("stop");
    digitalWrite(motorPin, LOW);
    delay(500); 
  }
}

void loop() {
  if(isProcessing == true) {
    return;
  }
  
  char state = 'S';
  
  //マイクの音量(電圧)を測り、micValueという場所(変数)に入れておきます
  int micLeftValue = abs(analogRead(micLeftPin));
  lAvgArr[count] = micLeftValue;
  
  int micRightValue = abs(analogRead(micRightPin));
  rAvgArr[count] = micRightValue;
    
  rSum = rSum + (micRightValue - rAvgArr[count-elem]);
  lSum = lSum + (micLeftValue - lAvgArr[count-elem]);
    
  count++;
  
  double rAvg = rSum / (double)elem;
  double lAvg = lSum / (double)elem;
  
  //Left or Right
  if((lAvg >= threshold) && (rAvg >= threshold)) {
    isProcessing = true;
    resetState();
    state = 'B';
    show('L', lAvg);
    show('R', rAvg);
  }else {
    if(rAvg >= threshold) {
      isProcessing = true;
      resetState();
      state = 'R';
      show('R', rAvg);
    }
    if(lAvg >= threshold) {
      isProcessing = true;
      resetState();
      state = 'L';
      show('L', lAvg);
    }
  }
  
  switch (state) {
  case 'B':
    vib(motorRightPin, 1);
    vib(motorLeftPin, 1);
    Serial.println("both ... both");
    break;
  case 'R':
    vib(motorRightPin, 1);
    Serial.println("right ... right");
    break;
  case 'L':
    vib(motorLeftPin, 1); 
    Serial.println("left ... left");
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