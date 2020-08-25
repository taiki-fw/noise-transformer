//ピンを宣言します
const int micLeftPin = A0;
const int micRightPin = A1;

const int motorLeftPin = 9;
const int motorRightPin = 10;

const int threshold = 1000;
boolean isProcessing = false;
void setup() {
  //9ピンを出力ピンに設定
  pinMode(motorLeftPin, OUTPUT);
  pinMode(motorRightPin, OUTPUT);
  //シリアル通信を開始します
  Serial.begin(9600);
}

void show(char state, int value) {
  	if(state == 'L') {  
	  	Serial.print("LeftValue:");
    }else {
        Serial.print("RightValue:");
    }
  	Serial.print(abs(value));
  	Serial.println("");
}

void vib(int motorPin) {
	digitalWrite(motorPin, HIGH);
    delay(500);
    digitalWrite(motorPin, LOW);
    delay(500); 
}

void loop() {
  if(isProcessing == true) {
    return;
  }
  
  char state = 'S';
  
  //マイクの音量(電圧)を測り、micValueという場所(変数)に入れておきます
  int micLeftValue = analogRead(micLeftPin);
  int micRightValue = analogRead(micRightPin);
  
  //Left or Right
  if((micLeftValue >= threshold) && (micRightValue >= threshold)) {
    isProcessing = true;
    state = 'B';
    show('L', micLeftValue);
    show('R', micRightValue);
  }else {
    if(micRightValue >= threshold) {
      isProcessing = true;
      state = 'R';
      show('R', micRightValue);
    }
    if(micLeftValue >= threshold) {
      isProcessing = true;
      state = 'L';
      show('L', micLeftValue);
    }
  }
  
  switch (state) {
  case 'B':
    vib(motorRightPin);
    vib(motorLeftPin);
    Serial.println("both ... both");
    break;
  case 'R':
    vib(motorRightPin);
    Serial.println("right ... right");
    break;
  case 'L':
    vib(motorLeftPin); 
    Serial.println("left ... left");
    break;
  default:
    Serial.println("nothing");
  }
  
  if(isProcessing == true) {
    delay(3000);
    isProcessing = false;
  }
  
  Serial.println("");
}