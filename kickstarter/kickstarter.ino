/*
  LiquidCrystal Library - Hello World

 Demonstrates the use a 16x2 LCD display.  The LiquidCrystal
 library works with all LCD displays that are compatible with the
 Hitachi HD44780 driver. There are many of them out there, and you
 can usually tell them by the 16-pin interface.

 This sketch prints "Hello World!" to the LCD
 and shows the time.

  The circuit:
 * LCD RS pin to digital pin 12
 * LCD Enable pin to digital pin 11
 * LCD D4 pin to digital pin 5
 * LCD D5 pin to digital pin 4
 * LCD D6 pin to digital pin 3
 * LCD D7 pin to digital pin 2
 * LCD R/W pin to ground
 * LCD VSS pin to ground
 * LCD VCC pin to 5V
 * 10K resistor:
 * ends to +5V and ground
 * wiper to LCD VO pin (pin 3)

 Library originally added 18 Apr 2008
 by David A. Mellis
 library modified 5 Jul 2009
 by Limor Fried (http://www.ladyada.net)
 example added 9 Jul 2009
 by Tom Igoe
 modified 22 Nov 2010
 by Tom Igoe

 This example code is in the public domain.

 http://www.arduino.cc/en/Tutorial/LiquidCrystal
 */

// include the library code:
#include <LiquidCrystal_neo.h>

LiquidCrystal lcd(6,7,8, 9,10,11,12);

const int buzzPin = 3;
const int buttonPin = 2;

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
  Serial.begin(115200);
  
  pinMode(buttonPin,INPUT);
  pinMode(13,OUTPUT);

  attachInterrupt(buttonPin,updateIndex,RISING);

  // initialize the library with the numbers of the interface pins

  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  delay(100);
  lcd.print("UDOO X86");
  
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

  int state = LOW;
  while (digitalRead(buttonPin)){
    if (!state){
      state =  HIGH;
      updateIndex();
    }
  }
  
 
  switch (idx) {
    case 0:
      // set the cursor to column 0, line 1
      // (note: line 1 is the second row, since counting begins with 0):
      lcd.setCursor(0, 1);
      delay(25);
      lcd.print(pledged);
      delay(25);
      lcd.print("$");

      if (millis() - timeUpdate < deltaInterval * 1000){

        if (pledgedDelta){
          lcd.print(" +");
        } else {
          lcd.print(" ");
        }
 
        lcd.print(pledgedDelta);
        lcd.print("$           ");
      }else{
        lcd.print("         ");
      }
      break;
      
    case 1:
      lcd.setCursor(0, 1);
      delay(25);
      lcd.print(backers);
      delay(25);
      lcd.print(" backers        ");
      break;
    case 2:
      lcd.setCursor(0, 1);

      char _days[3];
      char _hours[3];

      itoa(hours / 24, _days, 10);
      itoa(hours % 24, _hours, 10);
      String str = _days;
      str+= " d & ";
      str+= _hours;
      str+= " H left";
      
      lcd.print(str);
      delay(25);
      break;    
   }
   delay(100);

}

void updateIndex(){
  idx= ++idx % maxIdx;
}

