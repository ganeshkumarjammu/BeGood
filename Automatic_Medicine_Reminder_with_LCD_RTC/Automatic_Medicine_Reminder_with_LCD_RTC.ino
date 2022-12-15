/********************* 
Project Title : Automatic Medicine Reminder
Organisation: Soham Academy Of Human Excellence
Website: https://ria.sohamacademy.org/
*********************/

#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);
#include <Wire.h>
#include <RTClib.h>
#include <EEPROM.h>

int totRemin = 0;
int val2;
int addr = 0;

RTC_DS3231 rtc;

#define GetWellSoon 0
#define HelpScreen 1
#define TimeScreen 2

int pushpressed = 0;

const int buzzPin = 11;
const int ledPin = 13; // LED pin
int ledState = LOW;
int Signal = 0;
int btn1state, btn2state, btn3state = 0; 

int oneTimeBtn = 2;
int twoTimeBtn = 3;
int threeTimeBtn = 4;
int stopPin = 9;

int screens = 0; // screen to show

int reminNo = 1;
int maxScreen = 2; // screen count

bool isScreenChanged = true;
long previousMillis = 0;
long interval = 500; // buzzing interval

unsigned long currentMillis;
long previousMillisLCD = 0; // for LCD screen update
long intervalLCD = 2000; // Screen cycling interval

unsigned long currentMillisLCD;

//   Set Reminder Change Time
int remin1HH = 8; //    HH - hours         ##Set these for reminder time in 24hr Format
int remin1MM = 00; //    MM - Minute
int remin1SS = 00; //    SS - Seconds

int remin2HH = 8; //    HH - hours
int remin2MM = 02; //    MM - Minute
int remin2SS = 00; //    SS - Seconds

int remin3HH = 8; //    HH - hours
int remin3MM = 04; //    MM - Minute
int remin3SS = 00; //    SS - Seconds
int nowHr, nowMin, nowSec; // to show current mm,hh,ss

boolean stopAlarm = 0 ;

// All messeges
void gwsMessege()
{ // print get well soon messege
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Stay Healthy :)"); // Give some cheers
  lcd.setCursor(0, 1);
  lcd.print("Get Well Soon :)"); // wish
}

void reminderMsg()
{ 
  //insrtuction Message
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(" Take Dollo 650"); // Give some cheers
  lcd.setCursor(0, 1);
  lcd.print("   with water"); // wish
}

void helpScreen()
{ 
  // function to display 1st screen in LCD
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Press Buttons");
  lcd.setCursor(0, 1);
  lcd.print("for Reminder...!");
}

void timeScreen()
{ 
  // function to display Date and time in LCD screen
  DateTime now = rtc.now(); // take rtc time and print in display
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Time:");
  lcd.setCursor(6, 0);
  lcd.print(nowHr = now.hour(), DEC);
  lcd.print(":");
  lcd.print(nowMin = now.minute(), DEC);
  lcd.print(":");
  lcd.print(nowSec = now.second(), DEC);
  lcd.setCursor(0, 1);
  lcd.print("Date: ");
  lcd.print(now.day(), DEC);
  lcd.print("/");
  lcd.print(now.month(), DEC);
  lcd.print("/");
  lcd.print(now.year(), DEC);
}

void setNxtRem(){
 if(reminNo < totRemin){
    reminNo++ ;
 }
 else if(reminNo >= totRemin){
  reminNo = 1;
 }
}

void setup()
{
  Serial.begin(9600); // start serial debugging

  if (!rtc.begin())
  { 
    // check if rtc is connected
    Serial.println("Couldn't find RTC");
    while (1);
  }

  if (rtc.lostPower())
  {
    Serial.println("RTC lost power, lets set the time!");
  }

  //    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));            // uncomment this to set the current time and then comment in next upload when u set the time

  // manual time set
  rtc.adjust(DateTime(2022,12, 1, 7, 59, 00));  // DateTime(Year,Month,Date,Hour,Minute,Second)
  
  lcd.begin();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("  Welcome To"); // print a messege at startup
  lcd.setCursor(0, 1);
  lcd.print(" GHS, Paripally");
  delay(500);

  pinMode(oneTimeBtn, INPUT_PULLUP); // define push button pins type
  pinMode(twoTimeBtn, INPUT_PULLUP);
  pinMode(threeTimeBtn, INPUT_PULLUP);
  pinMode(stopPin, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);
  pinMode(buzzPin, OUTPUT);
  
  Serial.println(EEPROM.read(addr));
  val2 = EEPROM.read(addr); // read previosuly saved value of push button to start from where it was left previously
  switch (val2)
  {
    case 1:
      Serial.println("Set for 1/day");
      btn1state = 0;
      btn2state = 1;
      btn3state = 1;
      totRemin = 1;
      break;

    case 2:
      Serial.println("Set for 2/day");
      btn1state = 1;
      btn2state = 0;
      btn3state = 1;
      totRemin = 2;
      break;

    case 3:
      Serial.println("Set for 3/day");
      btn1state = 1;
      btn2state = 1;
      btn3state = 0;
      totRemin = 3;
      break;
  }
}

void loop()
{

  is1Time(); // call to set once/day
  is2Times(); // call to set twice/day
  is3Times(); // call to set thrice/day
  checkRemin1();
  checkRemin2();
  checkRemin3();
  currentMillisLCD = millis();        // start millis for LCD screen switching at defined interval of time
  btn1state = digitalRead(oneTimeBtn); // start reading all push button pins
  delay(50);
  btn2state = digitalRead(twoTimeBtn);
  delay(50);
  btn3state = digitalRead(threeTimeBtn);
  delay(50);
  changeScreen(); // screen cycle function
}

// call to set once/day
void is1Time()
{
  // function to set reminder once/day
  if (btn1state == 0)
  {
    btn1state = 1;
    btn2state = 1;
    btn3state = 1;
    //    pushPressed = true;
    EEPROM.write(addr, 1);
    Serial.print("1 time per day written : ");
    Serial.println(EEPROM.read(addr)); // for debugging
    totRemin = 1; // save the state of push button-1
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Reminder set ");
    lcd.setCursor(0, 1);
    lcd.print("for Once/day !");
    delay(1200);
    lcd.clear();
  }
}

// call to set twice/day
void is2Times()
{
  // function to set reminder twice/day
  if (btn2state == 0)
  {
    btn2state = 1;
    btn1state = 1;
    btn3state = 1;
    //    pushPressed = true;
    EEPROM.write(addr, 2);
    Serial.print("2 times per day written : ");
    Serial.println(EEPROM.read(addr));
    totRemin = 2;
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Reminder set ");
    lcd.setCursor(0, 1);
    lcd.print("for Twice/day !");
    delay(1200);
    lcd.clear();
  }
}

// call to set thrice/day
void is3Times()
{
  // function to set reminder thrice/day
  if (btn3state == 0)
  {
    btn3state = 1;
    btn1state = 1;
    btn2state = 1;
    //    pushPressed = true;
    EEPROM.write(addr, 3);
    Serial.print("3 times per day written : ");
    Serial.println(EEPROM.read(addr));
    totRemin = 3;
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Reminder set ");
    lcd.setCursor(0, 1);
    lcd.print("for Thrice/day !");
    delay(1200);
    lcd.clear();
  }
}


void startBuzz()
{
    reminderMsg();
    while (digitalRead(stopPin)) {
        Serial.println("Reminder: Take Medicine");
        unsigned long currentMillis = millis();
        if (currentMillis - previousMillis >= interval)
        {
          previousMillis = currentMillis; // save the last time you blinked the LED
          Serial.println("Start Buzzing");
          if (ledState == LOW)
          { 
           // if the LED is off turn it on and vice-versa:
            ledState = HIGH;
          }
          else
          {
            ledState = LOW;
          }
          digitalWrite(ledPin, ledState);
          digitalWrite(buzzPin, ledState);
        }
  }
        Serial.println("Patient is Medicated"); 
        ledState = LOW;
        digitalWrite(ledPin, ledState);
        digitalWrite(buzzPin, ledState);
        stopAlarm = true;
        setNxtRem();
}

void checkRemin1()
{
  if (reminNo == 1) {
   // 1st Reminder
    DateTime now = rtc.now();
    if (int(now.hour()) >= remin1HH)
    {
      if (int(now.minute()) >= remin1MM)
      {
        if (int(now.second()) >= remin1SS)
        {
          startBuzz();
        }
      }
    }
  }
}

void checkRemin2()
{
  if (reminNo == 2) {
    // 2nd reminder 
    DateTime now = rtc.now();
    if (int(now.hour()) >= remin2HH)
    {
      if (int(now.minute()) >= remin2MM)
      {
        if (int(now.second()) >= remin2SS)
        {
          startBuzz();
        }
      }
    }
  }
}

void checkRemin3()
{
  if (reminNo == 3) {
    // 3rd reminder 
    DateTime now = rtc.now();
    if (int(now.hour()) >= remin3HH)
    {
      if (int(now.minute()) >= remin3MM)
      {
        if (int(now.second()) >= remin3SS)
        {
          startBuzz();
        }
      }
    }
  }
}

// Screen Cycling
void changeScreen()
{
  // function for Screen Cycling
  // Start switching screen every defined intervalLCD
  if (currentMillisLCD - previousMillisLCD > intervalLCD) // save the last time you changed the display
  {
    previousMillisLCD = currentMillisLCD;
    screens++;
    if (screens > maxScreen)
    {
      screens = 0; // all screens over -> start from 1st
    }
    isScreenChanged = true;
  }
  // Start displaying current screen
  if (isScreenChanged) // only update the screen if the screen is changed.
  {
    isScreenChanged = false; // reset for next iteration
    switch (screens) {
      case GetWellSoon:
        gwsMessege(); // get well soon message
        break;

      case HelpScreen:
        helpScreen(); // instructions screen
        break;

      case TimeScreen:
        timeScreen(); // to print date and time
        break;

      default:
        break;
    }
  }
}
