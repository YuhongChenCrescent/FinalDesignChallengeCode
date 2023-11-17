#include "RTClib.h"
#include <LiquidCrystal.h>
#define led 13
#define buzzer 10
#define button 6
#define turnOff 7
RTC_DS1307 rtc;
bool buttonState=0;
bool turnOffState=0;
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

//Start of Alert Function(play noise and flash)
void alert(){
  int notes[10]={261, 294, 330, 349, 392, 392, 349, 330,  294, 261};
  for(int i=0;i<=10;i++){
    digitalWrite(led, HIGH);
    delay(500);
    digitalWrite(led, LOW);
    delay(500);
    tone(10,notes[i]);
  }
  noTone(10);
  lcd.clear();
  lcd.print("Alarm has gone");
  lcd.setCursor(0, 1);
  lcd.print("Off");
  delay(2000);
  lcd.clear();
}

//Start of checking function
  bool check(int yr,int mon,int da,int hr,int min,int sec){
      DateTime current = rtc.now();
      if (current.second()==sec && current.hour()==hr && current.day()==da && current.minute()==min && current.year()==yr && current.month()==mon){
        char arr1[19];
        Serial.print("Alarm has gone off. RTC read time: ");
        sprintf(arr1,"%d-%d-%d-%d:%d:%d",current.year(),current.month(),current.day(),current.hour(),current.minute(),current.second());
        Serial.print(arr1);
        Serial.println();
        alert();
        return true;
    }
    else{
      return false;
    }
}
void end(){
  for(int i=0;i<=3;i++){
    delay(500);
    digitalWrite(13,LOW);
    tone(10,261);
    delay(1000);
    noTone(10);
  }
}

//Setup
void setup () {
  lcd.begin(16,2);
  lcd.clear();
  pinMode(led,OUTPUT);
  pinMode(buzzer,OUTPUT);
  pinMode(button,INPUT);
  pinMode(turnOff,INPUT);
  Serial.begin(57600);
#ifndef ESP8266
  while (!Serial);
#endif
  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    while (1) delay(10);
  }
  if (! rtc.isrunning()) {
    Serial.println("RTC is NOT running, let's set the time!");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
  lcd.clear();
  lcd.print("Program running");
}

//Start of main function
void loop () {
    bool hit=false;
    if (digitalRead(6)==HIGH){
      buttonState=1;
    }
    else if(digitalRead(6)==LOW){
      buttonState=0;
    }
    if(digitalRead(7)==HIGH){
      turnOffState=1;
    }
    //Starting the timer/reseting the timer
    if (buttonState==1){
      digitalWrite(13,HIGH);
        while (true){
          if (digitalRead(6)==HIGH){
            digitalWrite(13,HIGH);
            buttonState=1;
            DateTime Start=rtc.now();
            DateTime newAlarm=(Start+TimeSpan(0,0,0,10));
            const int second=newAlarm.second();
            const int minute=newAlarm.minute();
            const int hour=newAlarm.hour();
            const int day=newAlarm.day();
            const int month=newAlarm.month();
            const int year=newAlarm.year();
            Serial.print("Initialized alarm time");
            char arr[19];
            sprintf(arr,"%d-%d-%d-%d:%d:%d",year,month,day,hour,minute,second);
            Serial.println();
            Serial.print(arr);
            Serial.println();
            lcd.setCursor(0,0);
            lcd.print("Alarm(No year):");
            lcd.setCursor(0,1);
            char t[15];
            sprintf(t,"%d-%d:%d:%d:%d",month,day,hour,minute,second);
            lcd.print(t);
            while(true){
              bool hit=check(year,month,day,hour,minute,second);
              if (hit==1){
                digitalWrite(13,LOW);
                buttonState=0;
                break;
            }
          }
        }
        else if(digitalRead(6)==LOW){
            buttonState=0;
            lcd.clear();
            lcd.print("Alarm has gone");
            lcd.setCursor(0, 1);
            lcd.print("Off");
            digitalWrite(13,HIGH);
            delay(250);
            digitalWrite(13,LOW);
            delay(250);
        }
        if(digitalRead(7)==HIGH){
          turnOffState=1;
        }
        if(turnOffState==1){
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("Code ended");
          end();
          while(true){
          }
        }
      }
    }
}