#include "BluetoothSerial.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial SerialBT;

float value = 0;
int waktu = 400;
int Y[11] = {4, 13, 16, 17, 18, 19, 26, 21, 25, 23, 22};
char input = ' ';
String output = "";
int rps[] = {5, 2};
int rpm[2];
int rev[2] = {0,0};
long refresh;
long duration[2];
long prevmillis[2] = {0, 0};
boolean prevstate[2] = {LOW, LOW};
boolean currentstate[2];

void isr(){
  rev[0]++;
}

void isr1(){
  rev[1]++;
}

void setup() {
  attachInterrupt(0,isr,RISING);
  attachInterrupt(0,isr1,RISING);
  
  Serial.begin(9600);
  
  if (SerialBT.begin("Vanns")){
    Serial.println("Bluetooth Initialized ...");
  }

  for (int i = 0; i<4; i++){
    pinMode(rps[i], INPUT);
  }
  
  for (int i = 0; i < 11; i++){
    pinMode(Y[i], OUTPUT);
  } 
  
  for (int i = 0; i < 10; i++){
    digitalWrite(Y[i], HIGH);
  } 
  
  digitalWrite(Y[9], LOW);
  delay(waktu);
  digitalWrite(Y[5], LOW);
  digitalWrite(Y[6], LOW);
}

void loop() {
  RPM();
  if (SerialBT.available() > 0) {
      input = char(SerialBT.read());
    if (input != '\n') {
      output += input;
    } else {
      Kerja(output);
      output = "";
    }
  }
}

void Mulai(int A, int B, int C, int D){
  RPM();
  Henti();
  delay(waktu);
  RPM();
  digitalWrite(Y[10], LOW);
  digitalWrite(A, LOW);
  delay(waktu);
  RPM();
  digitalWrite(B, LOW);
  delay(waktu);
  RPM();
  digitalWrite(C, LOW);
  delay(waktu);
  RPM();
  digitalWrite(D, LOW);
  delay(waktu);
  RPM();
  digitalWrite(Y[10], HIGH);
}

void Mulai5(int A, int B, int C, int D, int E){
  RPM();
  Henti();
  delay(waktu);
  RPM();
  digitalWrite(Y[10], LOW);
  delay(waktu);
  RPM();
  digitalWrite(A, LOW);
  delay(waktu);
  RPM();
  digitalWrite(B, LOW);
  delay(waktu);
  RPM();
  digitalWrite(C, LOW);
  delay(waktu);
  RPM();
  digitalWrite(D, LOW);
  delay(waktu);
  RPM();
  digitalWrite(E, LOW);
  delay(waktu);
  RPM();
  digitalWrite(A, HIGH);
  delay(waktu);
  RPM();
  digitalWrite(Y[10], HIGH);
}

void Henti(){
  for (int i = 0; i < 9; i++){
    digitalWrite(Y[i], HIGH);
  }
}

void RL(){
  RPM();
  Henti();
  digitalWrite(Y[2], LOW);
  delay(waktu);
  RPM();
  digitalWrite(Y[3], LOW);
  digitalWrite(Y[4], LOW);
  delay(waktu);
  RPM();
  digitalWrite(Y[6], LOW);
  delay(waktu);
  RPM();
  digitalWrite(Y[7], LOW);
}

void RH(){
  RPM();
  Henti();
  digitalWrite(Y[1], LOW);
  delay(waktu);
  RPM();
  digitalWrite(Y[3], LOW);
  digitalWrite(Y[4], LOW);
  delay(waktu);
  RPM();
  digitalWrite(Y[6], LOW);
  delay(waktu);
  RPM();
  digitalWrite(Y[7], LOW);
}

void RPM(){
  for (int i = 0; i<2; i++){
    currentstate[i] = digitalRead(rps[i]);
    
    if(prevstate[i] != currentstate[i]){
       if(currentstate[i] == HIGH){
           detachInterrupt(0);
           duration[i] = millis() - prevmillis[i];
           rpm[i] = (rev[i]/duration[i])*60000;
           prevmillis[i] = millis();
           rev[i] = 0;

           (i == 0) ? attachInterrupt(0, isr ,RISING):attachInterrupt(0, isr1 ,RISING);
       }
    }
     
    prevstate[i] = currentstate[i];
  }

  //if((millis() - refresh) >= 100){
    String dataRPM = (String) rpm[0] + ',' + (String) rpm[1];
    SerialBT.println(dataRPM);
  //}
}

void Kerja(String output){
  Serial.print("Input = ");
  Serial.print(output);
  Serial.print(" Panjang Input = ");
  Serial.println(output.length());
  if (output.length() >= 2){
    output = String(output[0]) + String(output[1]);
  } else {
    output = output[0];
  }
  Serial.print("Output = ");
  Serial.println(output);
  
  if (output == "0"){
     Serial.println("Program 0");
     Henti();
     delay(waktu);
     digitalWrite(Y[5], LOW);
     digitalWrite(Y[6], LOW);
  } else if (output == "1"){
      Serial.println("Program 1");
     Mulai5(Y[0], Y[2], Y[4], Y[5], Y[7]);
  } else if (output == "2"){
     Serial.println("Program 2");
     Mulai(Y[1], Y[4], Y[5], Y[7]);
  }  else if (output == "3"){
     Serial.println("Program 3");
     Mulai5(Y[0], Y[2], Y[4], Y[6], Y[7]);
  } else if (output == "4"){
     Serial.println("Program 4");
     Mulai(Y[1], Y[4], Y[6], Y[7]); 
  } else if (output == "5"){
     Serial.println("Program 5");
     Mulai5(Y[0], Y[2], Y[5], Y[3], Y[7]);
  } else if (output == "6"){
     Serial.println("Program 6");
     Mulai(Y[1], Y[3], Y[5], Y[7]);
  } else if (output == "7"){
     Serial.println("Program 7");
     Mulai5(Y[0], Y[2], Y[3], Y[6], Y[7]);
  } else if (output == "8"){
     Serial.println("Program 8");
     Mulai(Y[1], Y[3], Y[6], Y[7]);
  } else if (output == "9"){
     Serial.println("Program 9");
     Mulai5(Y[0], Y[2], Y[5], Y[4], Y[8]);
  } else if (output == "10"){
     Serial.println("Program 10");
     Mulai(Y[1], Y[4], Y[5], Y[8]);
  } else if (output == "11"){
     Serial.println("Program 11");
     Mulai5(Y[0], Y[2], Y[4], Y[6], Y[8]);
  } else if (output == "12"){
     Serial.println("Program 12");
     Mulai(Y[1], Y[4], Y[6], Y[8]);
  } else if (output == "13"){
     Serial.println("Program 13");
     Mulai5(Y[0], Y[2], Y[5], Y[3], Y[8]);
  } else if (output == "14"){
     Serial.println("Program 14");
     Mulai(Y[1], Y[3], Y[5], Y[8]);
  } else if (output == "15"){
     Serial.println("Program 15");
     Mulai5(Y[0], Y[2], Y[3], Y[6], Y[8]);
  } else if (output == "16"){
     Serial.println("Program 16");
     Mulai(Y[1], Y[3], Y[6], Y[8]);
  } else if (output == "17"){
     Serial.println("Program RH");
     RH();
  } else if (output == "18"){
     Serial.println("Program RL");
     RL();
  }
}
