/*

  Kickstarter Tracker

  by Ettore Chimenti aka Ek5
  for UDOO Team

  @ 04/2016

  The circuit:
 * LCD RS pin to digital pin 12
 * LCD R/W pin to digital pin 11
 * LCD Enable pin to digital pin 10
 * LCD D4 pin to digital pin 9
 * LCD D5 pin to digital pin 8
 * LCD D6 pin to digital pin 7
 * LCD D7 pin to digital pin 6
 * LCD VSS pin to ground
 * LCD VCC pin to 5V
 * 10K pull-down resistor to digital pin 2
 * Button to +3V3 and digital pin 2
 * 10K pot:
 * ends to +5V and ground
 * wiper to LCD VO pin (pin 3)
 * piezo buzzer to PWM 3

*/

#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(6,7,8, 9,10,11,12);

const int buzzPin = 3;
const int buttonPin = 2;

const char project[16] = "UDOO X86";

const int maxIdx = 3;
const int deltaInterval = 30;

int button=0;
int pledged=0;
int pledgedDelta=0;
int timeUpdate = 0;
int backers=0;
int hours=0;
volatile int idx=0;

void setup() {

  //serial init
  Serial.begin(115200);

  pinMode(buttonPin,INPUT);

  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);

  //header
  lcd.print(project);

  pinMode(13,OUTPUT);
  digitalWrite(13,HIGH);
  delay(500);
  digitalWrite(13,LOW);

}

void loop() {

  while(Serial.available()>0){

    int _pledged=Serial.parseInt(); // read money
    int _backers=Serial.parseInt(); // read backers
    int _hours=Serial.parseInt();   // read hours left

    if ( Serial.read() == '\n' ) {

      //values changed?
      if ( ( pledged != _pledged ) || ( backers != _backers ) ){

        //buzzz
        analogWrite(buzzPin,100);
        delay(400);
        analogWrite(buzzPin,0);

        //update
        timeUpdate = millis();
        pledgedDelta = _pledged - pledged;
        idx = 0;

        pledged=_pledged;
        backers=_backers;
      }

      if ( hours != _hours ){
        hours = _hours;
      }

    }else{
      Serial.flush();
      break;
    }
  }

  // button
  int state = LOW;
  while (digitalRead(buttonPin)){
    if (!state){
      state =  HIGH;

      // buzz when screen change
      analogWrite(buzzPin,100);
      delay(100);
      analogWrite(buzzPin,0);

      updateIndex();
    }
  }

  // menu
  switch (idx) {
    case 0:
      // Pledge screen

      // set the cursor to column 0, line 1
      // (note: line 1 is the second row, since counting begins with 0):
      lcd.setCursor(0, 1);

      lcd.print(pledged);
      lcd.print("$");

      // display deltapledge only for a while, when total changes
      if ( (millis() - timeUpdate < deltaInterval * 1000) && pledgedDelta ){

        if (pledgedDelta > 0){
          lcd.print(" +");
        } else {
          lcd.print(" ");
        }

       lcd.print(pledgedDelta);
       lcd.print("$               ");

      } else {
        lcd.print("             ");
      }
      break;

    case 1:
      // Backer screen
      lcd.setCursor(0, 1);
      lcd.print(backers);
      lcd.print(" backers        ");
      break;

    case 2:
      // Time left
      lcd.setCursor(0, 1);

      char _days[3];
      char _hours[3];

      itoa(hours / 24, _days, 10);
      itoa(hours % 24, _hours, 10);
      String str = _days;
      str+= " d & ";
      str+= _hours;
      str+= " H left  ";

      lcd.print(str);
      break;

   }

   delay(100);

}

void updateIndex(){
  idx= ++idx % maxIdx;
}

