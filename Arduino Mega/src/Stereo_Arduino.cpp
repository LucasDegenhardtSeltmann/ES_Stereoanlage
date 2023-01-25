#include <Wire.h>
#include <ArduinoJson.h>
DynamicJsonDocument dataJSON(13);

int potiValues[10];
int oldPotiValues[10];
int butValues[5];
int oldButValues[5];
int sourceValues[6];
int oldSourceValues[6];
int miscValues[2];
int oldMiscValues[2];
int input;

void copy(int* src, int* dst, int len);
void checkChanges();
void sendupdate(int in, int id);
void receiveEvent(int howMany);
void requestEvent();

void setup() {
  //pinMode(22, INPUT);
  Serial.begin(9600);
  Serial.print("\nStartvorgang beendet");
  Wire.begin(8);                /* join i2c bus with address 8 */
  Wire.onReceive(receiveEvent); /* register receive event */
  Wire.onRequest(requestEvent); /* register request event */

}

void loop() {
  //Get Poti Values
  copy(potiValues,oldPotiValues,10);
  potiValues[0] = analogRead(A0);
  potiValues[1] = analogRead(A1);
  potiValues[2] = analogRead(A2);
  potiValues[3] = analogRead(A3);
  potiValues[4] = analogRead(A4);
  potiValues[5] = analogRead(A5);
  potiValues[6] = analogRead(A6);
  potiValues[7] = analogRead(A7);
  potiValues[8] = analogRead(A8);
  potiValues[9] = analogRead(A9);
  //Get Button Values 0 or 1
  copy(butValues,oldButValues,5);
  butValues[0] = digitalRead(22);
  butValues[1] = digitalRead(23);
  butValues[2] = digitalRead(24);
  butValues[3] = digitalRead(25);
  butValues[4] = digitalRead(26);
  //Get Source Value 1/6 at 1
  copy(sourceValues,oldSourceValues,6);
  sourceValues[0] = digitalRead(28);
  sourceValues[1] = digitalRead(29);
  sourceValues[2] = digitalRead(30);
  sourceValues[3] = digitalRead(31);
  sourceValues[4] = digitalRead(32);
  sourceValues[5] = digitalRead(33);
  copy(miscValues,oldMiscValues,2);
  miscValues[0] = digitalRead(52);
  sourceValues[1] = digitalRead(34);
  delay(1000);
}


void copy(int* src, int* dst, int len) {
    memcpy(dst, src, sizeof(src[0])*len);
}

void checkChanges() {
  for(int i=0; i<10;i++){
      int dif = potiValues[i] - oldPotiValues[i];
    if(dif >= 50 || dif <= -50 ){
      sendupdate(potiValues[i],i);
      delay(10);
    }
  }
  for(int i=0; i<5;i++){
    if(butValues[i] != oldButValues[i]){
      if(i>2 || butValues[i] != 0){
        sendupdate(butValues[i],i+10);
        delay(10);
      }
    }
  }
  for(int i=0; i<6;i++){
    if(sourceValues[i] != oldSourceValues[i] && sourceValues[i] != 0){
      sendupdate(sourceValues[i],i+14);
      delay(10);
    }
  }
  for(int i=0; i<2;i++){
    if(miscValues[i] != oldMiscValues[i]){
      sendupdate(miscValues[i],i+19);
      delay(10);
    }
  }
}

void sendupdate(int in, int id){
      uint8_t buffer[4];
      buffer[0] = in >> 8;
      buffer[1] = in & 0xff;
      buffer[2] = id >> 8;
      buffer[3] = id & 0xff;
      Wire.write(buffer, 4);
}
// function that executes whenever data is received from master
void receiveEvent(int howMany) {
 while (0 <Wire.available()) {
    char c = Wire.read();      /* receive byte as a character */
    Serial.print(c);           /* print the character */
  }
 Serial.println();             /* to newline */
}

// function that executes whenever data is requested from master
void requestEvent() {
  checkChanges();
}