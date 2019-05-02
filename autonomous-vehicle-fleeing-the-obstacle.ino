#include <Servo.h>
#define trig 2
#define echo 3
#define mR1 9
#define mR2 8
#define mL1 6
#define mL2 7
#define mRe 10
#define mLe 5

int rightSide = 0;
int next = 90;
int leftSide = 180;
int timee = 0;
int distance = 0;
int oldTime = 0;
Servo sg90;

void setup() {
  // put your setup code here, to run once:
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
  pinMode(mL1, OUTPUT);  //Tüm pinlerden güç çıkışı olacağı için OUTPUT olarak ayarladık.
  pinMode(mL2, OUTPUT);  
  pinMode(mR1, OUTPUT);
  pinMode(mR2, OUTPUT);
  pinMode(mRe, OUTPUT);
  pinMode(mLe, OUTPUT);
  Serial.begin(9600);
  sg90.attach(4);
  sg90.write(90);
}

void calculateDistance(){
  digitalWrite(trig, HIGH);
  delayMicroseconds(500);
  digitalWrite(trig, LOW);
  
  timee = pulseIn(echo, HIGH);
  distance = (timee/2) / 29.1;
}

//VIN (+) GND (-)

void ahead(){
  digitalWrite(mR1, HIGH);
  digitalWrite(mR2, LOW);
  digitalWrite(mRe, 150);
  //********************************
  digitalWrite(mL1, HIGH);
  digitalWrite(mL2, LOW);
  digitalWrite(mLe, 150);
  Serial.println("İleri gidiliyor...");
  oldTime += 1;
  delay(1);
}

void right(){
  digitalWrite(mR1, LOW);
  digitalWrite(mR2, HIGH);
  digitalWrite(mRe, 150);
  //********************************
  digitalWrite(mL1, HIGH);
  digitalWrite(mL2, LOW);
  digitalWrite(mLe, 150);
  Serial.println("Sağa dönülüyor...");
}

void left(){
  digitalWrite(mR1, HIGH);
  digitalWrite(mR2, LOW);
  digitalWrite(mRe, 150);
  //********************************
  digitalWrite(mL1, LOW);
  digitalWrite(mL2, HIGH);
  digitalWrite(mLe, 150);  
  Serial.println("Sola dönülüyor...");
}

void stopp(){
  digitalWrite(mR1, LOW);
  digitalWrite(mR2, LOW);
  digitalWrite(mRe, 150); 
  //********************************
  digitalWrite(mL1, LOW);
  digitalWrite(mL2, LOW); 
  digitalWrite(mLe, 150);
  Serial.println("Durdu...");
}

void back(){
  digitalWrite(mR1, LOW);
  digitalWrite(mR2, HIGH);
  digitalWrite(mRe, 150);
  //********************************
  digitalWrite(mL1, LOW);
  digitalWrite(mL2, HIGH);
  digitalWrite(mLe, 150);
  Serial.println("Geri gidiliyor...");
}

void findDirections(){
  sg90.write(rightSide);
  delay(750);
  calculateDistance();
  if(abs(distance) > 25){
    sg90.write(next);
    delay(150);
    back();
    delay(220);
    right();
    delay(280);
  }
  else if(abs(distance) <= 25){
    sg90.write(leftSide);
    delay(750);
    calculateDistance();
    if(abs(distance) > 25){
      sg90.write(next);
      delay(150);
      back();
      delay(220);
      left();
      delay(280);
    }
    else if(abs(distance) <= 25){
      sg90.write(next);
      delay(150);
      back();
      delay(1500);
      stopp();
      findDirections();
    }
  }
}

void attention(int pain){
  Serial.println("okunan: ");
  Serial.print(pain);
  if(90 > pain > 0){
    sg90.write(135);
    delay(150);
    calculateDistance();
    if(abs(distance) > 25){
      sg90.write(next);
      delay(150);
      back();
      delay(150);
      left();
      delay(120);
    }
    else{
      sg90.write(next);
      delay(150);
      back();
      delay(1500);
      stopp();
      findDirections();
    }
  }
  else if(180 > pain > 90){
    sg90.write(45);
    delay(150);
    calculateDistance();
    if(abs(distance) > 25){
      sg90.write(next);
      delay(150);
      back();
      delay(150);
      right();
      delay(120);
    }
    else{
      sg90.write(next);
      delay(150);
      back();
      delay(1500);
      stopp();
      findDirections();
    }
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  calculateDistance();
  while(abs(distance) > 25){
    ahead();
    calculateDistance();
    for(int i = 30; i <= 150; i+=5){
      sg90.write(i);
      delay(20);
      Serial.println(i);
      calculateDistance();
      if(abs(distance) <= 25){
        attention(i);
        break;
      }
      calculateDistance();
    }
    for(int i = 150; i >= 30; i-=5){
      sg90.write(i);
      delay(20);
      Serial.println(i);
      calculateDistance();
      if(abs(distance) <= 25){
        attention(i);
        break;
      }
      calculateDistance();
    }
    calculateDistance();
  }
  if(abs(distance) <= 25){
    stopp();
    calculateDistance();
    if(abs(distance) <= 25){
      findDirections();
    }
  }
}
