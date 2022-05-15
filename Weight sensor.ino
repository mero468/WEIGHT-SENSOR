#include <LiquidCrystal.h>
#include "HX711.h"

#define LOADCELL_DOUT_PIN 4
#define LOADCELL_SCK_PIN 5
HX711 scale;
float calibration_factor = -7050;

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 10, 9, 8, 7);
bool state = false;
double limit = 40;
double previous = 0;


void setup() {
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  Serial.begin(9600);
  scale.begin( LOADCELL_DOUT_PIN,LOADCELL_SCK_PIN);
  scale.set_scale(calibration_factor);
  scale.tare();
  
  long zero_factor = scale.read_average();
  Serial.print(" zero factor: ");
  Serial.print(zero_factor);
  delay(500);

}

void loop() {
  double KG = scale.get_units();
  Serial.print("Force sensor reading = ");
  Serial.println(KG,3); // print the raw analog reading
  
  delay(500);

  if (KG <= limit && KG>0){
    if(!state){
      lcd.clear();
      state = true;
    }
    if(KG!=previous){
      lcd.clear();
      previous = KG;
    }
    double LBs = KG * 2.205;
    String KGformatted =  String(KG, 3);
    String LBsformatted = String(LBs,3);
    
    //Print KG
    lcd.setCursor(0, 0);
    lcd.print("KG:  ");
    lcd.println(KGformatted);
    //Print LB
    lcd.setCursor(0, 1);
    lcd.print("LB:  ");
    lcd.print(LBsformatted);
  }
  else if(KG>limit){ 
    if(state){
      lcd.clear();
      state = false;
    }
    lcd.setCursor(0, 0);
    lcd.print("KG:  ");
    lcd.print("More than 40KG");
    lcd.setCursor(0, 1);
    lcd.print("LB:  ");
  }
  //KG resets

}
