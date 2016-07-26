// MorseButton
// Chiyoung Kim

// Output pin from button to D2
// Ground to ground (duh)
// Power to 5V

#include <HID.h>
#include <Keyboard.h>

const int buttonPin = 2;

int pressLength = 0;

const int shortSig = 100;
const int longSig = 500;
const int pauseSig = 500;

int stream[4] = {-1,-1,-1,-1};

const char alphabet[26] = {'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z'};

const int combos[26][4] = {{1,2,0,0},{2,1,1,1},{2,1,2,1},{2,1,1,0},{1,0,0,0},{1,1,2,1},{2,2,1,0},{1,1,1,1},{1,1,0,0},{1,2,2,2},{2,1,2,0},{1,2,1,1},{2,2,0,0},{2,1,0,0},{2,2,2,0},{1,2,2,1},{2,2,1,2},{1,2,1,0},{1,1,1,0},{2,0,0,0},{1,1,2,0},{1,1,1,2},{1,2,2,0},{2,1,1,2},{2,1,2,2},{2,2,1,1}};

int start = 0;

int inSig;

void setup() {
  // put your setup code here, to run once:
  pinMode(buttonPin,INPUT);
  Keyboard.begin();
  Serial.begin(9600);
}

void loop() {
  while(start == 0){
    if (digitalRead(buttonPin) == HIGH){
      start = 1;
    }
  }
  inSig = 0;
  // put your main code here, to run repeatedly:
  if (digitalRead(buttonPin) == HIGH) {
    while (digitalRead(buttonPin) == HIGH){
      delay(50);
      inSig += 50;
    }
    if (inSig >= 1000) {
      Keyboard.write(KEY_RETURN);
      inSig = 0;
      streamreset();
    }
    else if (inSig >= 200){
      streampush(2);
    }
    else if (inSig >= 50) {
      streampush(1);
    }
  }
  else if (digitalRead(buttonPin) == LOW) {
    while (digitalRead(buttonPin) == LOW) {
      delay(100);
      inSig -= 100;
      if (inSig == -500) {
        while (!streamcheck()) {
          streampush(0);
        }
        break;
      }
    }
  }
  if (streamcheck()){
    streaminterp(); 
    streamreset();
  }
}

void streampush(int input){
  stream[3] = stream[2];
  stream[2] = stream[1];
  stream[1] = stream[0];
  stream[0] = input;
}

bool streamcheck(){
  for (int i = 0; i < 4; ++i){
    if (stream[i] == -1) {
      return false;
    }
  }
  return true;
}

void streaminterp(){
  for (int i = 0; i < 26; ++i) {
    for (int j = 0; j < 4; ++j) {
      if (stream[3-j] != combos[i][j]){
        break;
      }
      if (j == 3) {
        Keyboard.write(alphabet[i]);
        Serial.println(alphabet[i]);
        return;
      }
    }
  }
}

void streamreset(){
  for (int i = 0; i < 4; ++i){
   stream[i] = -1; 
  }
  start = 0;
}

