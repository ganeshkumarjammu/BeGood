
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);
#include <Servo.h>
Servo myservo;
int insideIR = 4; // IR Sensor 1
int outsideIR = 7; // IR Sensor 2
int maxSlots = 6;      //Enter Total number of parking Slots
boolean insideCar = false;
boolean outsideCar = false;
int slotsLeft = 4; 
void setup()
{
  Serial.begin(9600);
  lcd.begin();
  lcd.backlight();
  pinMode(insideIR, INPUT);
  pinMode(outsideIR, INPUT);
  myservo.attach(9);
  myservo.write(0);
  lcd.setCursor (0, 0);
  lcd.print("    SOHAM  ");
  lcd.setCursor (0, 1);
  lcd.print(" PARKING SYSTEM ");
  delay (2000);
  lcd.clear();
}
/**
 *                   Inside insideIR                                                                                  outsideIR    Outside                                                                                            
 * CAR GOING OUTSIDE:  car -->sensor---> (insideCar==true)  car  ---> Gateopen ---> car  (outsideCar == false) car  --> sensor -->  count=count+1 
 *  count=count-1 <--- car <--sensor<--- (insideCar==false) car <--- Gateopen <--- car  (outsideCar == true)   car  <-- sensor <--   car          < --- CAR COMING INSIDE
 *  
 **/
void loop() {
  if(!digitalRead(insideIR)){
    if( !(insideCar && outsideCar) ){   //if insideCar == false && outsideCar ==false
      if(slotsLeft < maxSlots){
      insideCar = true ;  //car is going outside
      myservo.write(120);
      delay(50);
      }
    }
    else if( (!insideCar) && outsideCar ){ //if insideCar == false && outsideCar == true
      if(slotsLeft > 0 ){
        outsideCar = false ; //car is coming inside
        myservo.write(0);
        delay(50);
        slotsLeft-- ;
        displayMsg();
      }
      else{
        displayMsg();
      }
    }
    else if( insideCar && (!outsideCar) ){ //if  insideCar == true && outsideCar == false 
      myservo.write(0);            
      delay(50);
      insideCar = false ;
    }
  }
  
  if(!digitalRead(outsideIR)){
    if( !(insideCar && outsideCar) ){   //if insideCar == false && outsideCar ==false
      if(slotsLeft > 0){
      outsideCar = true ;  //car is coming inside
      myservo.write(120);
      delay(50);
      }
      else{
        
      }
    }
    else if( insideCar && (!outsideCar) ){ //if insideCar == true && outsideCar == false
      if(slotsLeft < maxSlots){
        insideCar = false ; //car is going outside
        myservo.write(0);
        delay(50);
        slotsLeft++ ;
        displayMsg();
      }
      else{
        displayMsg();
      }
    }
    else if( insideCar && (!outsideCar) ){ //if  insideCar == false && outsideCar == true
      myservo.write(0);            
      delay(50);
      outsideCar = false ;
    }
  }
}
void displayMsg(){
  lcd.setCursor (0, 0);
  lcd.print("  WELCOME!  ");
  lcd.setCursor (0, 1);
  lcd.print("Slots Left: ");
  lcd.print(slotsLeft);
  delay(1000);
}
