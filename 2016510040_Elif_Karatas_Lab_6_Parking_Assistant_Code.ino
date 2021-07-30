#include <LiquidCrystal.h>
// define pins
#define echoPin A1
#define trigPin A0
#define buzzerPin 6

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

int maximumRange = 100; // max distance is 1m
int minimumRange = 0;

int parkingMode = 0; // define int value for different parking modes

String TIME_STRING = "2021.06.13 - 01:35:56"; // DATE AND TIME

// Date and time substring operations for lcd screen and time clock
int year = TIME_STRING.substring(0,4).toInt(); 
int month = TIME_STRING.substring(5,7).toInt();
int day = TIME_STRING.substring(8,10).toInt();
int hour = TIME_STRING.substring(13,15).toInt();
int minute = TIME_STRING.substring(16,18).toInt();
int second = TIME_STRING.substring(19,21).toInt();

// define unsigned int values for calculating seconds
unsigned int lastTime;
unsigned int timeNow;
unsigned int timeNow2;

void setup() {
  Serial.begin(9600);
  lcd.begin(16,2);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(buzzerPin, OUTPUT);
}

void loop() {
  // calculating for second increasing 
  lastTime = millis() - timeNow;
  // parking info
  calculateDistance(maximumRange, minimumRange);
  // warning sounds
  sound();
  delay(1000);
  lcd.clear();
  // date and time info
  if(millis() - timeNow2 >= 1000){
    timeNow2 = millis();
    dateTime();
  }
  // waiting for 2 seconds
  delay(2000);
}

int calculateDistance(int maxrange, int minrange)
{
  long duration, distance;

  digitalWrite(trigPin,LOW);
  delayMicroseconds(2); // 2ms wait for arduino
  digitalWrite(trigPin, HIGH); // send signal 
  delayMicroseconds(10); // 10ms wait for arduino
  digitalWrite(trigPin, LOW); 

  duration = pulseIn(echoPin, HIGH); // time for pin
  distance = duration / 58.2; // time will turn to cantimeters
  delay(50);

  // parking modes and distance information on lcd display
  lcd.clear();
  lcd.setCursor(0,0);
  if(distance > maxrange){
    lcd.print("Distance: ");
    lcd.print(distance);
    lcd.print(" cm.");
    lcd.setCursor(0,1);
    lcd.print("It is too far.");
    parkingMode = 1;
  }
  else if(distance <= 100 && distance >= 51){
    lcd.print("Distance: ");
    lcd.print(distance);
    lcd.print(" cm.");
    lcd.setCursor(0,1);
    lcd.print("It is far.");
    parkingMode = 2;
  }
  else if(distance <= 50 && distance >= 21){
    lcd.print("Distance: ");
    lcd.print(distance);
    lcd.print(" cm.");
    lcd.setCursor(0,1);
    lcd.print("It is close.");
    parkingMode = 3;
  }
  else if(distance <= 20 && distance >= 5){
    lcd.print("Distance: ");
    lcd.print(distance);
    lcd.print(" cm.");
    lcd.setCursor(0,1);
    lcd.print("It is too close.");
    parkingMode = 4;
  }
  else if(distance < 5){
    lcd.print("Distance: ");
    lcd.print(distance);
    lcd.print(" cm.");
    lcd.setCursor(0,1);
    lcd.print("It is crashed!");
    parkingMode = 5;
  }
  return distance;
}

void sound()
{
  // different sounds for different parking modes
  for(int i = 0; i < 5 ; i++){ // 5 times warning sound
    if(parkingMode == 1){
        tone(buzzerPin, 150); 
        delay(100);
        noTone(buzzerPin);
        delay(100);
    }
    else if(parkingMode == 2){
        tone(buzzerPin, 250);
        delay(100);
        noTone(buzzerPin);
        delay(100);
    }
    else if(parkingMode == 3){
        tone(buzzerPin, 350);
        delay(100);
        noTone(buzzerPin);
        delay(100);
    }
    else if(parkingMode == 4){
        tone(buzzerPin, 450);
        delay(100);
        noTone(buzzerPin);
        delay(100);
    }
  }
  if(parkingMode == 5){
    gameOver(); // game over sound for crashed
  }
}

void gameOver(){
// game over sound 
int i = 0;
int floops = 0;
int note = 1000/8; // eight notes
int pause = note * 0.2;

for(floops = 0; floops < 4; floops++){
  for(i = 25; i < 30; i++){
    tone(buzzerPin, 20*i, note);
    delay(pause);
  }
  for(i = 30; i >= 25; i--){
    tone(buzzerPin, 20*i, note);
    delay(pause);
  }
}

delay(500);
tone(buzzerPin, 500, 200);
delay(200);
tone(buzzerPin, 1200, 200);
delay(200);
tone(buzzerPin, 300, 200);
delay(200);
tone(buzzerPin, 1000, 200);
delay(200);
tone(buzzerPin, 400, 200);
delay(200);
tone(buzzerPin, 1100, 200);
delay(200);
}

void dateTime(){
  // calculating date time
    second++;
    lcd.setCursor(0,0);
    lcd.print("Hour: ");
    
    if(hour<10){ // if hour is less than 10, than add 0 to start cursor
      lcd.print("0");
    }
    lcd.print(hour);
    lcd.print(":");
    if(minute<10){ // if minute is less than 10, than add 0 to start cursor
      lcd.print("0");
    }
    lcd.print(minute);
    lcd.print(":");
    if(second<10){ // if hour is less than 10, than add 0 to start cursor
      lcd.print("0");
    }
    lcd.print(second);      
    if(second==59){
      second = 0;
      lcd.setCursor(6,0);
      minute++; // increase minute for every 60 seconds
    }
    if(minute == 60){
      minute = 0;
      lcd.setCursor(3,0);
      hour++; // increase hour for every 60 minutes
    }
    lcd.setCursor(0,1);
    // date and time display
    lcd.print("Date: ");
    lcd.print(day);
    lcd.print(".");
    lcd.print(month);
    lcd.print(".");
    lcd.print(year);
}

// references 
// [1] https://www.youtube.com/watch?v=UKj6qtSBXMQ
// [2] https://github.com/Robotistan/YouTube-DIY-Projeler/tree/master/Arduino%20Dersleri/Ders-10%20HC-SR04%20ile%20Park%20Sensoru%20Yapimi
