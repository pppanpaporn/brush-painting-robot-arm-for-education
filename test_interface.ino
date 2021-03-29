#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
#define BACKLIGHT_PIN 3  
LiquidCrystal_I2C lcd(0x27,2,1,0,4,5,6,7); 

const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
byte rowPins[ROWS] = {30, 32, 34, 36};
//connect to the row pinouts of the keypad
byte colPins[COLS] = {22, 24, 26, 28}; //connect to the column pinouts of the keypad
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );
Keypad keypad2 = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );
/////////////////////////////////////////////////////////////////////////////////////////////////////

//set arrow in LCD
byte arrowright[] = {
  B00000,
  B00100,
  B00110,
  B11111,
  B11111,
  B00110,
  B00100,
  B00000
};

byte arrowleft[] = {
  B00000,
  B00100,
  B01100,
  B11111,
  B11111,
  B01100,
  B00100,
  B00000
};

byte smile[]{
  B00000,
  B00000,
  B01010,
  B00000,
  B10001,
  B01110,
  B00000,
  B00000
};
///////////////////////////////////////////////////////////////////////////////////

void setup() {
 Serial.begin(9600);  
 lcd.begin(20,4);
 lcd.setBacklightPin(BACKLIGHT_PIN,POSITIVE);
 lcd.setBacklight(HIGH);
 lcd.createChar(0, arrowright);
 lcd.createChar(1, arrowleft);
 lcd.createChar(2, smile);
}



void menuStart(){

  lcd.setCursor(2,1);
  lcd.print("[1]Setting");
  lcd.setCursor(2,2);
  lcd.print("[2]Set zero");
  lcd.setCursor(2,3);
  lcd.print("[3]Manualmode");
  lcd.setCursor(17,3);
  lcd.print("A");
  lcd.write(byte(0));

  char key = keypad.getKey();
  while(key == 0){
    key = keypad.getKey();
  }
  Serial.println(key);
  switch (key)
  {
           case 'A': menu2();
                     break; 
//             char key2 = keypad2.getKey();
//             switch (key2)
//            {
//               case '4': test();
//                         break; 
//               case '5': readcard();
//                         break;
//            }
           case '1': setting();
                     break; 
           case '2': setzero();
                     break; 
           case '3': manual();
                     break; 
  }
}

//-------------------------------------------//
void menu2(){

  
  lcd.setCursor(2,1);
  lcd.print("[4]Test sample");
  lcd.setCursor(2,2);
  lcd.print("[5]SD-Card");
  lcd.setCursor(0,3);
  lcd.write(byte(1));
  lcd.print("B");
  char key = keypad.getKey();
  while (key==0) {
    key = keypad.getKey(); 
  }
  if(key!='B'){
  lcd.clear();
  lcd.setCursor(2, 0);
  lcd.write(byte(0));
  lcd.setCursor(4, 0);
  lcd.write(byte(0));
  lcd.print("Select Mode");
  lcd.write(byte(1));
  lcd.print(" ");
  lcd.write(byte(1));
  delay(300);
  
  lcd.setCursor(2,1);
  lcd.print("[4]Test sample");
  lcd.setCursor(2,2);
  lcd.print("[5]SD-Card");
  lcd.setCursor(0,3);
  lcd.write(byte(1));
  lcd.print("B");
    if(key=='B'){
      menuStart();
    }
   key = keypad.getKey();
    while (key!='*') {
      key = keypad.getKey(); 
    }
  }  
}

void setting(){
  
}

void setzero(){
  
}

void manual(){
  
}

void test(){
  
}

void readcard(){
  
}

//-------------------------------------------------------------//
void loop() {
  lcd.clear();
  delay(1000);
  lcd.setCursor(5,0);
  lcd.print("WELCOME TO");
  lcd.setCursor(3,1);
  lcd.print("BRUSHPAINTING");
  lcd.setCursor(5,2);
  lcd.print("ROBOT ARM");
  lcd.setCursor(1,3);
  lcd.write(byte(2));
  lcd.print(" ");
  lcd.write(byte(2));
  lcd.print(" ");
  lcd.write(byte(2));
  lcd.print(" ");
  lcd.write(byte(2));
  lcd.print(" ");
  lcd.write(byte(2));
  lcd.print(" ");
  lcd.write(byte(2));
  lcd.print(" ");
  lcd.write(byte(2));
  lcd.print(" ");
  lcd.write(byte(2));
  lcd.print(" ");
  lcd.write(byte(2));
  lcd.print(" ");
  lcd.write(byte(2));
  lcd.print(" ");
 
  delay(2000);
  menuStart();
  
}
