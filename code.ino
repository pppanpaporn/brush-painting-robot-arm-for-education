#include <SD.h>
#include <SPI.h> 
#include <dk_Stepper6.h>
#include <String.h>
#include <math.h>
#include <Servo.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
#define BACKLIGHT_PIN 3 
#define x_max 30000 
#define y_max 11000
#define x_drive 11
#define x_revert 10
#define y_drive 9
#define y_revert 8

LiquidCrystal_I2C lcd(0x27,2,1,0,4,5,6,7);

const byte NONE = 0;
const byte LEFT = 1;
const byte UP = 2;
const byte DOWN = 3;
const byte RIGHT = 4;
const byte SELECT = 5;


const byte buttonPin = A2;
byte keyButton = 0; 
const int chipSelect = 53;
  int theta = 45;
  int falpha = 50; //ระดับยกปลายพู่กัน เป็น % (0-100);
  int balpha = 30;
  int resolution = 2;  //ความละเอียดของ Servo ;
  int fspeed = 90;
  int bspeed = 20;
  int delaytime = 20;
  float brushlength = 65; //ความยาวพู่กันจากจุดหมุนถึงปลายพู่กัน 

/***********************************************************************/
int dx=0, dy = 0, jx_value, jy_value, temp;
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
/***********************************************************************/
dk_Stepper6 PBPR;
float dk_distM0mm=0 ,dk_distM1mm=0;
float dk_M0distcurrent=0, dk_M1distcurrent=0;
const byte jx = A1;    //joystick
const byte jy = A0;
const int limit_x = 5;
const int limit_y = 4;
const int buttonPin_red = 38;
const int buttonPin_blue = 40;
int buttonState_blue = 0;
int buttonState_red = 0;
//int delaytime = 15;
Servo myservo1,myservo2,myservo3;

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

void setup() {
  // put your setup code here, to run once:
  
  Serial.begin(9600);
  pinMode(buttonPin,INPUT_PULLUP);
  PBPR.dk_configStepmot(0,11,10);
  PBPR.dk_configStepmot(1,9,8);
  PBPR.dk_configMech(0,400,6);
  PBPR.dk_configMech(1,400,12);
 
 
  myservo1.attach(14);//pitch
  myservo2.attach(16);//roll
  myservo3.attach(18);//z  
  myservo1.write(77);
  myservo2.write(90);
  //myservo3.write(160);
 // myservo3.write(160);
  //draw_cat();
  //draw_fish();
  //draw_famingo();
  //draw_butterfly();
  //draw_dear(); 
  lcd.begin(20,4);
  lcd.setBacklightPin(BACKLIGHT_PIN,POSITIVE);
  lcd.setBacklight(HIGH);
  lcd.createChar(0, arrowright);
  lcd.createChar(1, arrowleft);
  lcd.createChar(2, smile);
  
  //dk_waitsecond(1);
  
  //PBPR_initial();
  lcd.clear();
  //setup_arm();
  //bamboo();
 //read_sd();
  //moveto_default();
  setmenu();
  
}

void PBPR_initial() {
  for(int ii=0; ii<7; ii++)       // reset all carries to zero 
    PBPR.dk_carry[ii]=0;
    PBPR.dk_pitch[0] = 6;
    PBPR.dk_pitch[1] = 12;
    PBPR.dk_SPmm[0]= PBPR.dk_SPR[0]/PBPR.dk_pitch[0];    // step per mm
    PBPR.dk_SPmm[1]= PBPR.dk_SPR[1]/PBPR.dk_pitch[1];    // step per mm
    Serial.print("Linear : X Pitch (mm)= ");Serial.println(PBPR.dk_pitch[0],2);
    Serial.print("Linear : X Steps per Rev (steps)= ");Serial.println(PBPR.dk_SPR[0],2);
    Serial.print("Linear : X Resolution (S/mm)= ");Serial.println(PBPR.dk_SPmm[0],2);
    Serial.print("Linear : Y Pitch (mm)= ");Serial.println(PBPR.dk_pitch[1],2);
    Serial.print("Linear : Y Steps per Rev (steps)= ");Serial.println(PBPR.dk_SPR[1],2);
    Serial.print("Linear : Y Resolution (S/mm)= ");Serial.println(PBPR.dk_SPmm[1],2);
  delay(2000);
}

void setup_arm(){
  //z_controlup();
  moveto_default();
  //z_controldown();
  //delay(500);
  //z_controlup();
   
}
/****************************************************/
//void z_controlup(){
//  //Serial.println("Brush Up ");
//  myservo3.write(100); 
//  //myservo3.write(ongsa); 
//  
//  delay(1000);// ขึ้นสุด 
//}
//void z_controldown(){
// // Serial.println("Brush Down ");
//  myservo3.write(155);
//   //myservo3.write(ongsa);
//  //myservo3.write(165);//ใช้กับต้นไผ่ 
//  delay(1000);// ลงสุด 
//}

/**********************************************************/
byte getButton()
{
  int val = 0;
  byte button = 0;
  val = analogRead(buttonPin);
  button = NONE;
  if(val <= 20) {button = LEFT;}
  else if ( (val >= 30) && (val <= 60) )  {button = UP;}
  else if ( (val >= 200) && (val <= 220) )  {button = RIGHT;}
  else if ( (val >= 110) && (val <= 150) )  {button = DOWN;}
  else if ( (val >= 310) && (val <= 420) )  {button = SELECT;}
  return button;
}

void dk_clearpositionXY() {   // clear position data of X and Y to Zero
    PBPR.dk_posXmm=0;
    PBPR.dk_posYmm=0;
}

void dk_ClearCarry_Stepvalues() {
 for (int ii=0; ii<(PBPR.dk_MaximumMotor-1); ii++) {  // clear all data of step drive
    PBPR.dk_stepcarry[ii]=0.0;
    PBPR.dk_stepvalue[ii]=0.0; 
 }
    dk_M0distcurrent=0.0;
    dk_M1distcurrent=0.0;
    
}

void dk_Clearalldata() {
  dk_ClearCarry_Stepvalues();
  dk_clearpositionXY();
}


void dk_waitsecond(int _second) {
  int _delay=_second*1000/5;
  Serial.print(" start : wait for "); Serial.print(_second); Serial.println(" sec");  delay(_delay);
     Serial.println(" . . . . 3 ");    delay(_delay);
     Serial.println(" . . . . 2 ");    delay(_delay);
     Serial.println(" . . . . 1 ");    delay(_delay);
     Serial.println(" . . . . . ");    delay(_delay);
}

void LCD_display(){
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
}
void LCD_display2(){
  lcd.clear();
  lcd.setCursor(2, 0);
  lcd.write(byte(0));
  lcd.setCursor(4, 0);
  lcd.write(byte(0));
  lcd.print("Test example");
  lcd.write(byte(1));
  lcd.print(" ");
  lcd.write(byte(1));
  delay(300);
}

void LCD_display3(){
  lcd.clear();
  lcd.setCursor(2, 0);
  lcd.write(byte(0));
  lcd.setCursor(4, 0);
  lcd.write(byte(0));
  lcd.print("Read SD-caed");
  lcd.write(byte(1));
  lcd.print(" ");
  lcd.write(byte(1));
  delay(300);
}
void setmenu(){
  lcd.clear();
  //delay(1000);
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
 
  delay(2000);
  menuStart();
}
/////////////////////////////////////////////////////////////////////



void menuStart(){
  LCD_display();
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
  switch (key){
           case 'A': menu2();
                     break; 
           case '1': setting();
                     break; 
           case '2': setzero();
                     break; 
           case '3': Manual();
                     break; 
  }
  return;
}

void menu2(){
  LCD_display();
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
  switch (key){
   
           case '4': test_1();
                     break; 
           case '5': readcard();
                     break;  
  }
  if(key=='B'){
     menuStart();
  }  
}

void setting(){
  showresMxMy();
  char key = keypad.getKey();
  while (key==0) {
    key = keypad.getKey(); 
  }
   if(key=='A'){
      showresXY();
   } 
}

void showresMxMy(){
  lcd.clear();
  lcd.setCursor(2,0);
  lcd.print("Res Mx=");    lcd.print(PBPR.dk_SPR[0],0); lcd.print("S/R");
  lcd.setCursor(7,1);
  lcd.print("My=");    lcd.print(PBPR.dk_SPR[1],0); lcd.print("S/R");
  lcd.setCursor(7,2);
  lcd.print("Z = 2.5 d/mm.");
  lcd.setCursor(18,3);
  lcd.print("A");
  lcd.write(byte(0));
  char key = keypad.getKey();
  while (key==0) {
    key = keypad.getKey(); 
  }
   if(key=='A'){
      showresXY();
   } 
}
void showresXY(){
    PBPR.dk_pitch[0] = 6;
    PBPR.dk_pitch[1] = 12;
    lcd.clear();
    lcd.setCursor(2, 0);
    lcd.print("Xres="); lcd.print(PBPR.dk_SPmm[0],2);    lcd.print(" S/mm");
    lcd.setCursor(2, 1);
    lcd.print("Pitch X="); lcd.print(PBPR.dk_pitch[0],0);   lcd.print(" mm");
    lcd.setCursor(2, 2);
    lcd.print("Yres="); lcd.print(PBPR.dk_SPmm[1],2);    lcd.print(" S/mm");
    lcd.setCursor(2, 3);
    lcd.print(" Pitch Y=");  lcd.print(PBPR.dk_pitch[1],0);   lcd.print(" mm");
    lcd.setCursor(0, 3);
    lcd.write(byte(1));lcd.print("B");
    lcd.setCursor(18, 3);
    lcd.print("A");lcd.write(byte(0));
    char key = keypad.getKey();
    while (key==0) {
      key = keypad.getKey(); 
    }
      if(key=='B'){
        showresMxMy();
       } 
       else if(key=='A'){
          menuStart();
       }
}

void setzero(){
  int value_x, value_y;
  while(1){
    value_x = digitalRead(limit_x);
    value_y = digitalRead(limit_y);
    if(value_x==0&&value_y==0){
      // print home
      lcd.clear();
      lcd.setCursor(3, 0);
      lcd.print("SET ZERO MODE");
      lcd.setCursor(0, 1);
      lcd.print("====================");
      lcd.setCursor(1, 2);
      lcd.print("Current positions");
      lcd.setCursor(3, 3);
      lcd.print("are at origin.");
      delay(3000);
      break;
    }else{
      //print check
      lcd.clear();
      lcd.setCursor(3, 0);
      lcd.print("SET ZERO MODE");
      lcd.setCursor(0, 1);
      lcd.print("====================");
      lcd.setCursor(1, 2);
      lcd.print("Check limit switch");
      lcd.setCursor(6, 3);
      lcd.print("X-Y Axis");
      delay(3000);
      lcd.clear();
      while(value_x&&value_y){
           if(value_x){
           PBPR.dk_drivemotor(0,1,50);
           //ซ้ายx_backward();
           delayMicroseconds(200);
           }
            if(value_y){
             PBPR.dk_drivemotor(1,-1,50);//ลงy_backward();
             delayMicroseconds(200);
            }
           value_x = digitalRead(limit_x);
           value_y = digitalRead(limit_y); 

           if(!value_x) {
                lcd.clear();
                lcd.setCursor(0, 1);
                delay(1000);
                lcd.print("Limit sw X is OK!!");
                delay(2000);
                break;}
           if(!value_y) {
            lcd.clear();
            lcd.setCursor(0, 2);
            delay(1000);
            lcd.print("Limit sw Y is OK!!");
            delay(2000);
            break;}
           
        }
      while(value_y){
       if(value_y){
       PBPR.dk_drivemotor(1,-1,50);//y_backward();
        delayMicroseconds(200);
        }
          value_y = digitalRead(limit_y);
       }
         while(value_x){
        if(value_x){
            PBPR.dk_drivemotor(0,1,50);//x_backward();
            delayMicroseconds(200);
        }
          value_x = digitalRead(limit_x);
       }
    }

  
  menuStart();
  return;
  }
}
void Manual(){ 
  lcd.clear();
  lcd.setCursor(3, 0);
  lcd.print("MANUAL MODE");
  lcd.setCursor(0, 1);
  lcd.print("===================");
  lcd.setCursor(2, 2);
  lcd.print("OK = up - down");
  lcd.setCursor(2, 3);
  lcd.print("A = Twist");
  delay(1000);
  lcd.clear();
 
  while(1){
    buttonState_red = digitalRead(buttonPin_red);
    buttonState_blue = digitalRead(buttonPin_blue);
    int ongsa1 = 100;
    int ongsa2 = 155;
    keyButton = getButton();
    joystickRead();
     temp = digitalRead(limit_x);
     if(!temp) dx=0;
     temp = digitalRead(limit_y);
     if(!temp) dy=0;
     
     // joystick control
     if(jx_value<312||jx_value>712) drive_first();
      else drive_second(); 
//    char key = keypad.getKey();
//      
//    while (key==0) {
//      key = keypad.getKey(); 
//    }
    if(keyButton == SELECT){
    lcd.setCursor(2, 1);
    lcd.print(" Position OK!!");
    delay(3000);
    lcd.clear(); 
    while(1){
      buttonState_red = digitalRead(buttonPin_red);
      buttonState_blue = digitalRead(buttonPin_blue);
      keyButton = getButton();
      if(buttonState_red==LOW){
        lcd.clear();
        lcd.print("BRUSH DOWN");
        z_controldown(ongsa2);
        delay(1000);
      }else if(buttonState_blue==LOW){
        lcd.clear();
        lcd.print("BRUSH UP");
        z_controlup(ongsa1);
        delay(1000);
      }else if(keyButton == SELECT){
        lcd.clear();
        lcd.print("BACK MAIN MENU");
        delay(1000);
        menuStart();
      }
    }
   }
   else if(keyButton == LEFT){
    keyButton = getButton();
    buttonState_red = digitalRead(buttonPin_red);
    lcd.clear();
        lcd.print("Brush Twist");
        delay(2000);
        lcd.clear();
        lcd.print("Select ongsa");
        
        delay(2000);
         lcd.clear();
    float brushlength = 65; 
      char key = keypad.getKey();
      
    while (key==0) {
      key = keypad.getKey(); 
    }
       switch (key){
   
           case '1': twist(0, 50, 27, 95, 50, brushlength);
           
                     break; 
           case '2': twist(45, 50, 27, 95, 50, brushlength);
                     break;  
           case '3':twist(90, 50, 27, 95, 50, brushlength);
                     break;  
           case '4': twist(135, 50, 27, 95, 50, brushlength);
                     break;   
           case '5': twist(180, 50, 27, 95, 50, brushlength);
                     break;  
           case '6': twist(225, 50, 27, 95, 50, brushlength);
                     break;  
           case '7': twist(270, 50, 27, 95, 50, brushlength);
                     break;  
           case '8': twist(315, 50, 27, 95, 50, brushlength);
                     break;  
           case '9': twist(360, 50, 27, 95, 50, brushlength);
                     break;                                    
      }
        
//       else if(key1=='B'){
//        lcd.clear();
//        lcd.print("BACK MAIN MENU");
//        delay(1000);
//        menuStart();
//       }
   }
  }
   return;  
} 
void drive(int port, int delaytime){
  digitalWrite(port,HIGH);
  delayMicroseconds(delaytime);
  digitalWrite(port,LOW);
  delayMicroseconds(delaytime);
}
void x_forward(){
  digitalWrite(x_revert,HIGH); 
  drive(x_drive, 200); 
  dx++;
}
void x_backward(){
  digitalWrite(x_revert,LOW); 
  drive(x_drive, 200); 
  dx--;
}
void y_forward(){
  digitalWrite(y_revert,LOW); 
  drive(y_drive, 200); 
  dy++;
}
void y_backward(){
  digitalWrite(y_revert,HIGH); 
  drive(y_drive, 200); 
  dy--; 
}
///////////////////////////////////////////
void joystickRead(){
  jx_value = analogRead(jx);
  jy_value = analogRead(jy);  
}
void drive_first(){
  if(jx_value>712&&dx<x_max){
     x_forward();
     drive_second();
     delayMicroseconds(200);
  }else if(jx_value<312&&dx>0){
     x_backward();
     drive_second();
     delayMicroseconds(200);
  }else{
    drive_second();
    delayMicroseconds(200);
  }
}
void drive_second(){
  if(jy_value>712&&dy<y_max) y_forward();
  else if(jy_value<312&&dy>0) y_backward();
return;
}


////////////////////////////////////////////////////
void test_1(){
  lcd.clear();
  LCD_display2();
  lcd.setCursor(2,1);
  lcd.print("[1]Cat");
  lcd.setCursor(2,2);
  lcd.print("[2]Fish");
  lcd.setCursor(2,3);
  lcd.print("[3]Butterfly");
  lcd.setCursor(17,3);
  lcd.print("A");
  lcd.write(byte(0));
  
int  key = keypad.getKey();
  while(key == 0){
    key = keypad.getKey();
  }  
    
  if(key != '*'){
    switch(key){
     case 'A' :  test_2();
                  break;
      case '1' :  draw_cat();
                  break;
      case '2' :  draw_fish();
                  break;
      case '3' :  draw_butterfly();
                  break;          
   } 
     return;
  }
}
void test_2(){
  lcd.clear();
  LCD_display2();
  lcd.setCursor(2,1);
  lcd.print("[4]Famingo");
  lcd.setCursor(2,2);
  lcd.print("[5]Dear");
  lcd.setCursor(2,3);
  lcd.print("[6]Bamboo");
  lcd.setCursor(0,3);
  lcd.write(byte(1));
  lcd.print("B");
  lcd.setCursor(17,3);
  lcd.print("A");
  lcd.write(byte(0));
  char key = keypad.getKey();
  while (key==0) {
    key = keypad.getKey(); 
  }
  switch (key) {
           case '4': draw_famingo();
                     break; 
           case '5': draw_dear();
                     break;  
           case '6': bamboo2();
                     break;  
  }
  if(key=='B'){
    test_1();
  }else if(key=='A'){
    menuStart();
  }
}
void draw_cat(){
  delay(3000);
  int d_delay = 300;
   int ongsa1 = 100;
   int ongsa2 = 155;
  z_controldown(ongsa2);
  
  z_controlup(ongsa1); 
  PBPR.dk_drive2motmmA(0,1,-30,20,d_delay);//+x ไปซ้าย +y ขึ้นบน
  z_controldown(ongsa2);
  
  PBPR.dk_drive2motmmA(0,1,-20,30,d_delay);
  PBPR.dk_drive2motmmA(0,1,-20,50,d_delay);
  PBPR.dk_drive2motmmA(0,1,-30,60,d_delay);
  PBPR.dk_drive2motmmA(0,1,-35,70,d_delay);
  PBPR.dk_drive2motmmA(0,1,-40,60,d_delay);
  PBPR.dk_drive2motmmA(0,1,-60,60,d_delay);
  PBPR.dk_drive2motmmA(0,1,-65,70,d_delay);
  PBPR.dk_drive2motmmA(0,1,-70,60,d_delay);
  PBPR.dk_drive2motmmA(0,1,-80,50,d_delay);
  PBPR.dk_drive2motmmA(0,1,-80,30,d_delay);
  PBPR.dk_drive2motmmA(0,1,-70,20,d_delay);
  PBPR.dk_drive2motmmA(0,1,-28,20,d_delay);
  
  z_controlup(ongsa1);
  PBPR.dk_drive2motmmA(0,1,-40,25,d_delay);
  z_controldown(ongsa2);
  PBPR.dk_drive2motmmA(0,1,-50,35,d_delay);
  PBPR.dk_drive2motmmA(0,1,-40,35,d_delay);
  PBPR.dk_drive2motmmA(0,1,-48,23,d_delay);
  z_controlup(ongsa1);
  PBPR.dk_drive2motmmA(0,1,-30,40,d_delay);
  z_controldown(ongsa2);
  PBPR.dk_drive2motmmA(0,1,-35,50,d_delay);
  PBPR.dk_drive2motmmA(0,1,-40,40,d_delay);
  z_controlup(ongsa1);
  
  PBPR.dk_drive2motmmA(0,1,-55,40,d_delay);
  z_controldown(ongsa2);
  PBPR.dk_drive2motmmA(0,1,-60,50,d_delay);
  PBPR.dk_drive2motmmA(0,1,-65,40,d_delay);
  z_controlup(ongsa1);
  PBPR.dk_drive2motmmA(0,1,-25,40,d_delay);
  z_controldown(ongsa2);
  PBPR.dk_drive2motmmA(0,1,-15,45,d_delay);
  z_controlup(ongsa1);
  PBPR.dk_drive2motmmA(0,1,-25,35,d_delay);
  z_controldown(ongsa2);
  PBPR.dk_drive2motmmA(0,1,-15,35,d_delay);
   z_controlup(ongsa1);
  PBPR.dk_drive2motmmA(0,1,-25,30,d_delay);
  z_controldown(ongsa2);
  PBPR.dk_drive2motmmA(0,1,-15,25,d_delay);
  z_controlup(ongsa1);
  PBPR.dk_drive2motmmA(0,1,-75,40,d_delay);
  z_controldown(ongsa2);
  PBPR.dk_drive2motmmA(0,1,-85,45,300);
  z_controlup(ongsa1);
  PBPR.dk_drive2motmmA(0,1,-75,35,300);
  z_controldown(ongsa2);
  PBPR.dk_drive2motmmA(0,1,-85,35,300);
   z_controlup(ongsa1);
  PBPR.dk_drive2motmmA(0,1,-75,30,300);
  z_controldown(ongsa2);
  PBPR.dk_drive2motmmA(0,1,-85,25,300);
   z_controlup(ongsa1);

  moveto_default();
  delay(1000);
  menuStart();

}
void draw_fish(){
  int d_delay = 400;
  int ongsa1 = 100;
   int ongsa2 = 155;
    z_controldown(ongsa2);
  z_controlup(ongsa1); 
  PBPR.dk_drive2motmmA(0,1,-10,80,d_delay);//+x ไปซ้าย +y ขึ้นบน
  z_controldown(ongsa2);
  
  PBPR.dk_drive2motmmA(0,1,-15,90,d_delay);
  PBPR.dk_drive2motmmA(0,1,-20,100,d_delay);
  PBPR.dk_drive2motmmA(0,1,-30,110,d_delay);
  PBPR.dk_drive2motmmA(0,1,-40,115,d_delay);
  PBPR.dk_drive2motmmA(0,1,-80,110,d_delay);
  PBPR.dk_drive2motmmA(0,1,-90,105,d_delay);
  PBPR.dk_drive2motmmA(0,1,-95,100,d_delay);
  PBPR.dk_drive2motmmA(0,1,-100,95,d_delay);
  PBPR.dk_drive2motmmA(0,1,-105,90,d_delay);
  PBPR.dk_drive2motmmA(0,1,-120,105,d_delay);
  PBPR.dk_drive2motmmA(0,1,-120,55,d_delay);
  PBPR.dk_drive2motmmA(0,1,-105,70,d_delay);
  PBPR.dk_drive2motmmA(0,1,-100,65,d_delay);
  PBPR.dk_drive2motmmA(0,1,-95,60,d_delay);
  PBPR.dk_drive2motmmA(0,1,-90,55,d_delay);
  PBPR.dk_drive2motmmA(0,1,-80,50,d_delay);
  PBPR.dk_drive2motmmA(0,1,-40,45,d_delay);
  PBPR.dk_drive2motmmA(0,1,-30,50,d_delay);
  PBPR.dk_drive2motmmA(0,1,-20,60,d_delay);
  PBPR.dk_drive2motmmA(0,1,-15,70,d_delay);
  PBPR.dk_drive2motmmA(0,1,-10,80,d_delay);

  z_controlup(ongsa1);
  PBPR.dk_drive2motmmA(0,1,-10,80,d_delay);
  z_controldown(ongsa2);
  PBPR.dk_drive2motmmA(0,1,-20,75,d_delay);
  PBPR.dk_drive2motmmA(0,1,-25,80,d_delay);

  z_controlup(ongsa1);
  PBPR.dk_drive2motmmA(0,1,-25,90,d_delay);
  z_controldown(ongsa2);
  PBPR.dk_drive2motmmA(0,1,-30,100,d_delay);
  PBPR.dk_drive2motmmA(0,1,-35,90,d_delay);

  z_controlup(ongsa1);
  PBPR.dk_drive2motmmA(0,1,-40,115,d_delay);
  z_controldown(ongsa2);
  PBPR.dk_drive2motmmA(0,1,-40,60,d_delay);

  z_controlup(ongsa1);
  PBPR.dk_drive2motmmA(0,1,-40,90,d_delay);
  z_controldown(ongsa2);
  PBPR.dk_drive2motmmA(0,1,-70,90,d_delay);
  PBPR.dk_drive2motmmA(0,1,-60,80,d_delay);
   PBPR.dk_drive2motmmA(0,1,-70,70,d_delay);
  PBPR.dk_drive2motmmA(0,1,-40,70,d_delay);

  z_controlup(ongsa1);
  PBPR.dk_drive2motmmA(0,1,-60,113,d_delay);
  z_controldown(ongsa2);
  PBPR.dk_drive2motmmA(0,1,-80,135,d_delay);
  PBPR.dk_drive2motmmA(0,1,-80,110,d_delay);

  z_controlup(ongsa1);
  PBPR.dk_drive2motmmA(0,1,-60,47,d_delay);
  z_controldown(ongsa2);
  PBPR.dk_drive2motmmA(0,1,-80,25,d_delay);
  PBPR.dk_drive2motmmA(0,1,-80,50,d_delay);
  z_controlup(ongsa1);

  moveto_default();
  delay(1000);
   menuStart();
}
void draw_butterfly(){
  int d_delay = 400;
   int ongsa1 = 100;
   int ongsa2 = 155;
    z_controldown(ongsa2);
  z_controlup(ongsa1); 
  PBPR.dk_drive2motmmA(0,1,-75,25,d_delay);//+x ไปซ้าย +y ขึ้นบน
  z_controldown(ongsa2);
  
  PBPR.dk_drive2motmmA(0,1,-80,30,d_delay);
  PBPR.dk_drive2motmmA(0,1,-85,90,d_delay);
  PBPR.dk_drive2motmmA(0,1,-80,105,d_delay);
  PBPR.dk_drive2motmmA(0,1,-85,115,d_delay);
  PBPR.dk_drive2motmmA(0,1,-80,120,d_delay);
  PBPR.dk_drive2motmmA(0,1,-90,130,d_delay);
  PBPR.dk_drive2motmmA(0,1,-90,140,d_delay);
  PBPR.dk_drive2motmmA(0,1,-80,116,d_delay);
  PBPR.dk_drive2motmmA(0,1,-70,116,d_delay);
  PBPR.dk_drive2motmmA(0,1,-60,136,d_delay);
  PBPR.dk_drive2motmmA(0,1,-60,126,d_delay);
  PBPR.dk_drive2motmmA(0,1,-70,116,d_delay);
  PBPR.dk_drive2motmmA(0,1,-65,107,d_delay);
  PBPR.dk_drive2motmmA(0,1,-70,97,d_delay);
  PBPR.dk_drive2motmmA(0,1,-65,86,d_delay);
  PBPR.dk_drive2motmmA(0,1,-70,28,d_delay);
  PBPR.dk_drive2motmmA(0,1,-75,23,d_delay);
 
  z_controlup(ongsa1);
  PBPR.dk_drive2motmmA(0,1,-67,70,d_delay);
  z_controldown(ongsa2);
  PBPR.dk_drive2motmmA(0,1,-40,85,d_delay);
  PBPR.dk_drive2motmmA(0,1,-25,80,d_delay);
  PBPR.dk_drive2motmmA(0,1,-20,100,d_delay);
   PBPR.dk_drive2motmmA(0,1,-10,120,d_delay);
  PBPR.dk_drive2motmmA(0,1,-20,140,d_delay);
  PBPR.dk_drive2motmmA(0,1,-40,130,d_delay);
  PBPR.dk_drive2motmmA(0,1,-70,100,d_delay);
  PBPR.dk_drive2motmmA(0,1,-80,100,d_delay);
  PBPR.dk_drive2motmmA(0,1,-110,130,d_delay);
  PBPR.dk_drive2motmmA(0,1,-130,140,d_delay);
  PBPR.dk_drive2motmmA(0,1,-140,120,d_delay);
  PBPR.dk_drive2motmmA(0,1,-130,100,d_delay);
  PBPR.dk_drive2motmmA(0,1,-125,80,d_delay);
  PBPR.dk_drive2motmmA(0,1,-110,85,d_delay);
  PBPR.dk_drive2motmmA(0,1,-78,65,d_delay);
 
  z_controlup(ongsa1);
  PBPR.dk_drive2motmmA(0,1,-40,83,d_delay);
  z_controldown(ongsa2);
  PBPR.dk_drive2motmmA(0,1,-30,60,d_delay);
  PBPR.dk_drive2motmmA(0,1,-40,40,d_delay);
  PBPR.dk_drive2motmmA(0,1,-60,35,d_delay);
  PBPR.dk_drive2motmmA(0,1,-70,42,d_delay);
  PBPR.dk_drive2motmmA(0,1,-75,35,d_delay);
  PBPR.dk_drive2motmmA(0,1,-82,40,d_delay);
  PBPR.dk_drive2motmmA(0,1,-90,35,d_delay);
  PBPR.dk_drive2motmmA(0,1,-110,40,d_delay);
  PBPR.dk_drive2motmmA(0,1,-120,60,d_delay);
  PBPR.dk_drive2motmmA(0,1,-110,85,d_delay); 
  z_controlup(ongsa1);

  moveto_default();
  delay(1000);
  menuStart();
}

void draw_famingo(){
  int d_delay = 400;
  int ongsa1 = 100;
  int ongsa2 = 155;
   z_controldown(ongsa2);
  z_controlup(ongsa1); 
  PBPR.dk_drive2motmmA(0,1,-20,180,d_delay);//+x ไปซ้าย +y ขึ้นบน
  z_controldown(ongsa2);
  PBPR.dk_drive2motmmA(0,1,-20,155,d_delay);
  PBPR.dk_drive2motmmA(0,1,-30,170,d_delay);
  PBPR.dk_drive2motmmA(0,1,-20,180,d_delay);
  PBPR.dk_drive2motmmA(0,1,-30,190,d_delay);
  PBPR.dk_drive2motmmA(0,1,-30,170,d_delay);
  PBPR.dk_drive2motmmA(0,1,-40,180,d_delay);
  PBPR.dk_drive2motmmA(0,1,-30,190,d_delay);
  PBPR.dk_drive2motmmA(0,1,-40,210,d_delay);
  PBPR.dk_drive2motmmA(0,1,-50,218,d_delay);
  PBPR.dk_drive2motmmA(0,1,-60,220,d_delay);
  PBPR.dk_drive2motmmA(0,1,-80,212,d_delay);
  PBPR.dk_drive2motmmA(0,1,-85,205,d_delay);
  PBPR.dk_drive2motmmA(0,1,-95,185,d_delay);
  PBPR.dk_drive2motmmA(0,1,-90,160,d_delay);
  PBPR.dk_drive2motmmA(0,1,-70,130,d_delay);
  PBPR.dk_drive2motmmA(0,1,-50,100,d_delay);
  PBPR.dk_drive2motmmA(0,1,-45,90,d_delay);
  PBPR.dk_drive2motmmA(0,1,-50,85,d_delay);
  PBPR.dk_drive2motmmA(0,1,-80,120,d_delay);
  
  PBPR.dk_drive2motmmA(0,1,-90,125,d_delay);
  PBPR.dk_drive2motmmA(0,1,-130,130,d_delay);
  PBPR.dk_drive2motmmA(0,1,-160,120,d_delay);
  PBPR.dk_drive2motmmA(0,1,-130,115,d_delay);
  PBPR.dk_drive2motmmA(0,1,-100,100,d_delay);
  PBPR.dk_drive2motmmA(0,1,-80,40,d_delay);
  PBPR.dk_drive2motmmA(0,1,-45,50,d_delay);
  PBPR.dk_drive2motmmA(0,1,-35,60,d_delay);
  PBPR.dk_drive2motmmA(0,1,-25,85,d_delay);
  PBPR.dk_drive2motmmA(0,1,-35,105,d_delay);
  PBPR.dk_drive2motmmA(0,1,-75,165,d_delay);
  PBPR.dk_drive2motmmA(0,1,-75,185,d_delay);
  PBPR.dk_drive2motmmA(0,1,-70,190,d_delay);
  PBPR.dk_drive2motmmA(0,1,-45,182,d_delay);
  PBPR.dk_drive2motmmA(0,1,-40,180,d_delay);

  z_controlup(ongsa1); 
  PBPR.dk_drive2motmmA(0,1,-80,40,d_delay);//+x ไปซ้าย +y ขึ้นบน
  z_controldown(ongsa2);
  PBPR.dk_drive2motmmA(0,1,-150,40,d_delay);
  PBPR.dk_drive2motmmA(0,1,-160,45,d_delay);
  PBPR.dk_drive2motmmA(0,1,-200,65,d_delay);
  PBPR.dk_drive2motmmA(0,1,-185,100,d_delay);
  PBPR.dk_drive2motmmA(0,1,-130,115,d_delay);
  z_controlup(ongsa1); 

  moveto_default();
  delay(1000);
   menuStart();
}
void draw_dear(){
  int d_delay = 400;
   int ongsa1 = 100;
   int ongsa2 = 155;
  z_controldown(ongsa2); 
  z_controlup(ongsa1); 
  PBPR.dk_drive2motmmA(0,1,-80,100,d_delay);//+x ไปซ้าย +y ขึ้นบน
  z_controldown(ongsa2);
  
  PBPR.dk_drive2motmmA(0,1,-100,140,d_delay);
  PBPR.dk_drive2motmmA(0,1,-105,160,d_delay);
  PBPR.dk_drive2motmmA(0,1,-105,170,d_delay);
  PBPR.dk_drive2motmmA(0,1,-100,185,d_delay);
  PBPR.dk_drive2motmmA(0,1,-90,195,d_delay);
  PBPR.dk_drive2motmmA(0,1,-80,200,d_delay); 
  PBPR.dk_drive2motmmA(0,1,-60,200,d_delay);
  PBPR.dk_drive2motmmA(0,1,-50,195,d_delay);
  PBPR.dk_drive2motmmA(0,1,-40,185,d_delay);
  PBPR.dk_drive2motmmA(0,1,-35,170,d_delay);
  PBPR.dk_drive2motmmA(0,1,-35,160,d_delay);
  PBPR.dk_drive2motmmA(0,1,-40,140,d_delay);
  PBPR.dk_drive2motmmA(0,1,-60,100,d_delay);
  PBPR.dk_drive2motmmA(0,1,-80,100,d_delay);
  PBPR.dk_drive2motmmA(0,1,-60,120,d_delay);
  PBPR.dk_drive2motmmA(0,1,-80,120,d_delay);
  PBPR.dk_drive2motmmA(0,1,-60,100,d_delay);
  
  z_controlup(ongsa1);
  PBPR.dk_drive2motmmA(0,1,-37,177,d_delay);
  z_controldown(ongsa2);
  PBPR.dk_drive2motmmA(0,1,-20,190,d_delay);
  PBPR.dk_drive2motmmA(0,1,10,200,d_delay);
  PBPR.dk_drive2motmmA(0,1,10,190,d_delay);
  PBPR.dk_drive2motmmA(0,1,0,170,d_delay);
  PBPR.dk_drive2motmmA(0,1,-20,160,d_delay);
  PBPR.dk_drive2motmmA(0,1,-35,160,d_delay);
  PBPR.dk_drive2motmmA(0,1,-10,185,d_delay);
  PBPR.dk_drive2motmmA(0,1,10,200,d_delay);
  
  z_controlup(ongsa1);
  PBPR.dk_drive2motmmA(0,1,-103,177,d_delay);
  z_controldown(ongsa2);
  PBPR.dk_drive2motmmA(0,1,-120,190,d_delay);
  PBPR.dk_drive2motmmA(0,1,-150,200,d_delay);
  PBPR.dk_drive2motmmA(0,1,-150,190,d_delay);
  PBPR.dk_drive2motmmA(0,1,-140,170,d_delay);
  PBPR.dk_drive2motmmA(0,1,-120,160,d_delay);
  PBPR.dk_drive2motmmA(0,1,-105,160,d_delay);
  PBPR.dk_drive2motmmA(0,1,-130,185,d_delay);
  PBPR.dk_drive2motmmA(0,1,-150,200,d_delay);
  
  z_controlup(ongsa1);
   PBPR.dk_drive2motmmA(0,1,-40,170,d_delay);
  z_controldown(ongsa2);
  PBPR.dk_drive2motmmA(0,1,-45,175,d_delay);
  PBPR.dk_drive2motmmA(0,1,-55,175,d_delay);
  PBPR.dk_drive2motmmA(0,1,-65,165,d_delay);
  PBPR.dk_drive2motmmA(0,1,-50,165,d_delay);
  PBPR.dk_drive2motmmA(0,1,-45,170,d_delay);
  PBPR.dk_drive2motmmA(0,1,-45,175,d_delay);
  
  z_controlup(ongsa1);
  PBPR.dk_drive2motmmA(0,1,-100,170,d_delay);
  z_controldown(ongsa2);
  PBPR.dk_drive2motmmA(0,1,-95,175,d_delay);
  PBPR.dk_drive2motmmA(0,1,-85,175,d_delay);
  PBPR.dk_drive2motmmA(0,1,-75,165,d_delay);
  PBPR.dk_drive2motmmA(0,1,-90,165,d_delay);
  PBPR.dk_drive2motmmA(0,1,-95,170,d_delay);
  PBPR.dk_drive2motmmA(0,1,-95,175,d_delay);
  
  z_controlup(ongsa1);
  PBPR.dk_drive2motmmA(0,1,-40,185,d_delay);
  z_controldown(ongsa2);
  PBPR.dk_drive2motmmA(0,1,-20,210,d_delay);
  PBPR.dk_drive2motmmA(0,1,-10,210,d_delay);
  PBPR.dk_drive2motmmA(0,1,0,220,d_delay);
  PBPR.dk_drive2motmmA(0,1,20,270,d_delay);
  PBPR.dk_drive2motmmA(0,1,10,300,d_delay);
  PBPR.dk_drive2motmmA(0,1,-10,310,d_delay); 
  PBPR.dk_drive2motmmA(0,1,0,300,d_delay);
  PBPR.dk_drive2motmmA(0,1,10,280,d_delay);
  PBPR.dk_drive2motmmA(0,1,0,270,d_delay);
  PBPR.dk_drive2motmmA(0,1,-10,270,d_delay);
  PBPR.dk_drive2motmmA(0,1,-30,290,d_delay);
  PBPR.dk_drive2motmmA(0,1,-20,260,d_delay);
  PBPR.dk_drive2motmmA(0,1,-5,250,d_delay);
  PBPR.dk_drive2motmmA(0,1,-10,240,d_delay);
  PBPR.dk_drive2motmmA(0,1,-30,230,d_delay);
  PBPR.dk_drive2motmmA(0,1,-40,210,d_delay);
  PBPR.dk_drive2motmmA(0,1,-60,200,d_delay);
  PBPR.dk_drive2motmmA(0,1,-50,230,d_delay);
  PBPR.dk_drive2motmmA(0,1,-50,260,d_delay);
  PBPR.dk_drive2motmmA(0,1,-40,240,d_delay);
  PBPR.dk_drive2motmmA(0,1,-45,207,d_delay);
  
   z_controlup(ongsa1);
  PBPR.dk_drive2motmmA(0,1,-100,185,d_delay);
  z_controldown(ongsa2);
  PBPR.dk_drive2motmmA(0,1,-120,210,d_delay);
  PBPR.dk_drive2motmmA(0,1,-155,240,d_delay);
  PBPR.dk_drive2motmmA(0,1,-150,290,d_delay);
  PBPR.dk_drive2motmmA(0,1,-135,310,d_delay);
  PBPR.dk_drive2motmmA(0,1,-135,265,d_delay);
  PBPR.dk_drive2motmmA(0,1,-120,280,d_delay); 
  PBPR.dk_drive2motmmA(0,1,-100,280,d_delay); 
  PBPR.dk_drive2motmmA(0,1,-120,270,d_delay);
  PBPR.dk_drive2motmmA(0,1,-135,250,d_delay);
  PBPR.dk_drive2motmmA(0,1,-130,240,d_delay);
  PBPR.dk_drive2motmmA(0,1,-110,230,d_delay);
  PBPR.dk_drive2motmmA(0,1,-100,210,d_delay);
  PBPR.dk_drive2motmmA(0,1,-80,200,d_delay);
  PBPR.dk_drive2motmmA(0,1,-95,235,d_delay);
  PBPR.dk_drive2motmmA(0,1,-90,260,d_delay);
  PBPR.dk_drive2motmmA(0,1,-105,235,d_delay);
  PBPR.dk_drive2motmmA(0,1,-95,207,d_delay);

  z_controlup(ongsa1);
  PBPR.dk_drive2motmmA(0,1,-45,130,d_delay);
  z_controldown(ongsa2);
  PBPR.dk_drive2motmmA(0,1,-10,90,d_delay);
  PBPR.dk_drive2motmmA(0,1,-20,60,d_delay);
  PBPR.dk_drive2motmmA(0,1,-30,40,d_delay);
  PBPR.dk_drive2motmmA(0,1,-70,-10,d_delay);
  PBPR.dk_drive2motmmA(0,1,-110,40,d_delay);
  PBPR.dk_drive2motmmA(0,1,-120,60,d_delay);
  PBPR.dk_drive2motmmA(0,1,-130,90,d_delay);
  PBPR.dk_drive2motmmA(0,1,-95,130,d_delay);
  z_controlup(ongsa1);

  moveto_default();
  delay(1000);
   menuStart();
}

void bamboo2(){
  int speed1 = 500;
   BrushLinearXYZ(15,43,170,speed1);
   end_BrushLinearXYZ(20,107,speed1); 
   BrushLinearXYZ(20,107,170,speed1);
   end_BrushLinearXYZ(42,164,speed1);  
   BrushLinearXYZ(42,164,170,speed1);
   end_BrushLinearXYZ(80,209,speed1);
   BrushLinearXYZ(80,209,175,speed1);
   end_BrushLinearXYZ(129,236,speed1);
   BrushLinearXYZ(129,236,175,speed1);
   end_BrushLinearXYZ(173,245,speed1);
   twist(135, 50, 27, 95, 50, brushlength);
   twist(180, 50, 27, 95, 50, brushlength);
   twist(225, 50, 27, 95, 50, brushlength);
   
   BrushLinearXYZ(80,209,160,speed1);
   end_BrushLinearXYZ(89,250,speed1);
   twist(45, 50, 27, 95, 50, brushlength);
   twist(90, 50, 27, 95, 50, brushlength);

   BrushLinearXYZ(42,164,160,speed1);
   end_BrushLinearXYZ(100,175,speed1);
   BrushLinearXYZ(100,175,160,speed1);
   end_BrushLinearXYZ(140,164,speed1);
   twist(270, 50, 27, 95, 50, brushlength);
   twist(225, 50, 27, 95, 50, brushlength);

   BrushLinearXYZ(20,107,160,speed1);
   end_BrushLinearXYZ(69,134,speed1);
   twist(250, 50, 27, 95, 50, brushlength);
   //twist(180, 50, 27, 95, 50, brushlength);
 
    moveto_default();
}

void readcard(){
  lcd.clear();
  LCD_display3();
  lcd.setCursor(2,1);
  lcd.print("[1]CoE");
  lcd.setCursor(2,2);
  lcd.print("[2]Eng_PSU");
  lcd.setCursor(2,3);
  lcd.print("[3]Playboy");
  lcd.setCursor(17,3);
  lcd.print("A");
  lcd.write(byte(0));
  int  key = keypad.getKey();
  while(key == 0){
    key = keypad.getKey();
  }  
    
  if(key != '*'){
    switch(key){
      case 'A' :  readcard_2();
                  break;
      case '1' :  read_sd("coe_psu.txt");
                  break;
      case '2' :  read_sd("eng_psu");
                  break;
      case '3' :  read_sd("playboy.txt");
                  break;          
   }
 }
}
void readcard_2(){
  lcd.clear();
  LCD_display3();
  lcd.setCursor(2,1);
  lcd.print("[4]Gear");
  lcd.setCursor(2,2);
  lcd.print("[5]Apple");
  lcd.setCursor(2,3);
  lcd.print("[6]Heart");
  lcd.setCursor(0,3);
  lcd.write(byte(1));
  lcd.print("B");
  lcd.setCursor(17,3);
  lcd.print("A");
  lcd.write(byte(0));
  char key = keypad.getKey();
  while (key==0) {
    key = keypad.getKey(); 
  }
  switch (key){
           case '4': read_sd("gear_n.txt");
                     break; 
           case '5': read_sd("apple.txt");
                     break;  
           case '6': read_sd("heart_n.txt");
                     break;  
  }
  if(key=='B'){
     readcard();
  }else if(key=='A'){
    menuStart();
  }
}
/*********************************************************************************************/
void read_sd(String filename){
  while(!Serial){;}
  Serial.print(filename);
  Serial.print("Initializing SD card...");
  pinMode(53, OUTPUT);
   if (!SD.begin(chipSelect)) {
    Serial.println("Card failed");
    return;
  }Serial.println("ok");  
  File myFile; 
  myFile = SD.open(filename);
  Serial.println("yes");// สั่งให้เปิดไฟล์ชื่อ test.txt เพื่ออ่านข้อมูล
  z_controldown(155);
  delay(500);
  z_controlup(100);
  if (myFile) { 
      int countstart = 0;
  while (myFile.available()) {                             
      String buffer = myFile.readStringUntil('\n');
      int iconti = 0;
      int count = 0;
      String value1, value2;
      float valA, valX, valY;
      int checkpen = 0;
      int checky = 0;
      for (int i = 0; i < buffer.length(); i++) { // for =======================
        if (buffer.substring(i, i + 1) == " " ) {   
          if (count == 1 ) {
            value1 = buffer.substring(iconti, i);
            value2 = value1;
            valA = value2.toFloat(); 
          }
          else if (count == 2 ) {
            value1 = buffer.substring(iconti, i);
            value2 = value1;
            valX = value2.toFloat();
          }
          else if (count == 3){
            if (checky == 0){
              value1 = buffer.substring(iconti, buffer.length());
              value2 = value1;
              valY = value2.toFloat();
              count = 0;
            }
          }
          else if (count == 0) {
            buffer.remove(i);
          }
          else{
            buffer.remove(i);
          }
        }
  
        else if (buffer.substring(i, i + 1) == "G" ) {
          iconti = i+1;
          count++;  
        }

        else if (buffer.substring(i, i + 1) == "X" ) {
          iconti = i+1;
          count++;
          checkpen++;  
        }

        else if (buffer.substring(i, i + 1) == "Y" ) {
          iconti = i+1;
          count++;
          if (checky == 1 ) {
              value1 = buffer.substring(iconti, buffer.length());
              value2 = value1;
              valY = value2.toFloat();
              count = 0;
              checky = 0;
          }
        }

        else if (buffer.substring(i, i + 1) == "Z" ) {
          iconti = i+1;
          buffer.remove(i);
        }
        else if (buffer.substring(i, i + 1) == "(" ) {
          if (buffer.substring(i + 1, i + 2) == "E" ) {
             z_controlup(100);
             countstart = 0;
          }
          else if (buffer.substring(i + 1, i + 2) == "S" ) {
              checky++;
          }  
        }     
    
      } // for loop  =======================================
         
      if(checkpen == 1) {
       if( countstart == 0) {
          PBPR.dk_drive2motmmA(0,1,-valX,valY,600);
          z_controldown(157);
          countstart = 1;
       }
       else if ( countstart == 1) {
          PBPR.dk_drive2motmmA(0,1,-valX,valY,600);
         }
        checkpen = 0;
      }
      //
    } // while loop   -------------------------------------------
    //check last value
    z_controlup(100);      
    myFile.close();
    delay(1000);
    moveto_default();
    menuStart();
  } 
  else {
    Serial.println("error opening .txt file");
  } 
  
}

/***********************************************************************************************/
double DegreestoRadians(double degrees) {
  return degrees / 360 * 2 * PI;
}

void backHome(){
  int curPitch = myservo1.read();
  int curRoll  = myservo2.read();
  int cmpP = abs(curPitch-77);
  int cmpR = abs(curRoll-90);
  int cmpResult;
  float fragtwist;
  float carrytwist=0;
  
  if(curPitch>77 && curRoll>90){
    cmpP = curPitch-77;
    cmpR = curRoll-90;
    if(abs(cmpP) > abs(cmpR)){
      cmpResult = cmpP/cmpR;
      fragtwist = cmpP%cmpR;
      for(int count = 1;count<=abs(cmpR);count++){
        carrytwist = carrytwist + fragtwist;
        if(carrytwist >= 1){
          myservo1.write(curPitch-((count*cmpResult)+1));
          carrytwist = carrytwist - 1;
          delay(delaytime);
        }
        else{ 
          myservo1.write(curPitch-(count*cmpResult));
          delay(delaytime);
        }
        myservo2.write(curRoll-count);
        delay(delaytime);
        }
      }
   else if(abs(cmpP) <= abs(cmpR)){
      cmpResult = cmpR/cmpP;
      fragtwist = cmpR%cmpP;
      for(int count = 0;count<=abs(cmpP);count+=1){
        carrytwist = carrytwist + fragtwist;
        if(carrytwist >= 1){
          myservo2.write(curRoll-(count*cmpResult)-1);
          carrytwist = carrytwist - 1;
          delay(delaytime);
        }
        else{ 
          myservo2.write(curRoll-(count*cmpResult));
          delay(delaytime);
        }
        myservo1.write(curPitch-count);
        delay(delaytime);
      }
    }
  }
   else if(curPitch>77 && curRoll<90){
    cmpP = curPitch-77;
    cmpR = curRoll-90;
    if(abs(cmpP) > abs(cmpR)){
      cmpResult = cmpP/cmpR;
      fragtwist = cmpP%cmpR;
      for(int count = 0;count<=abs(cmpR);count+=1){
        carrytwist = carrytwist + fragtwist;
        if(carrytwist >= 1){
          myservo1.write(curPitch+(count*cmpResult)-1);
          carrytwist = carrytwist - 1;
          delay(delaytime);
        }
        else{ 
          myservo1.write(curPitch+(count*cmpResult));
          delay(delaytime);
        }
        
        myservo2.write(curRoll+count);
        delay(delaytime);
        }
      }
   else if(abs(cmpP) <= abs(cmpR)){
      cmpResult = abs(cmpR/cmpP);
      fragtwist = abs(cmpR)%abs(cmpP);
      for(int count = 0;count<=abs(cmpP);count+=1){
        carrytwist = carrytwist + fragtwist;
        if(carrytwist >= 1){
          myservo2.write(curRoll+((count*cmpResult)+1));
          carrytwist = carrytwist - 1;
          delay(delaytime);
        }
        else{ 
          myservo2.write(curRoll+(count*cmpResult));
          delay(delaytime);
        }
        myservo1.write(curPitch-count);
        delay(delaytime);
      }
    }
  }
   else if(curPitch<77 && curRoll>90){
    cmpP = curPitch-77;
    cmpR = curRoll-90;
    if(abs(cmpP) > abs(cmpR)){
      cmpResult = cmpP/cmpR;
      fragtwist = cmpP%cmpR;
      for(int count = 0;count<=abs(cmpR);count+=1){
        carrytwist = carrytwist + fragtwist;
        if(carrytwist >= 1){
          myservo1.write(curPitch-(count*cmpResult)-1);
          carrytwist = carrytwist - 1;
          delay(delaytime);
        }
        else{ 
          myservo1.write(curPitch-(count*cmpResult));
          delay(delaytime);
        }
        
        myservo2.write(curRoll-count);
        delay(delaytime);
        }
      }
   else if(abs(cmpP) <= abs(cmpR)){
      cmpResult = cmpR/cmpP;
      fragtwist = cmpR%cmpP;
      for(int count = 0;count<=abs(cmpP);count+=1){
        carrytwist = carrytwist + fragtwist;
        if(carrytwist >= 1){
          myservo2.write(curRoll+((count*cmpResult)+1));
          carrytwist = carrytwist - 1;
          delay(delaytime);
        }
        else{ 
          myservo2.write(curRoll+(count*cmpResult));
          delay(delaytime);
        }
        myservo1.write(curPitch+count);
        delay(delaytime);
      }
    }
  }
    else if(curPitch<77 && curRoll<90){
    cmpP = curPitch-77;
    cmpR = curRoll-90;
    if(abs(cmpP) > abs(cmpR)){
      cmpResult = cmpP/cmpR;
      fragtwist = cmpP%cmpR;
      for(int count = 0;count<=abs(cmpR);count+=1){
        carrytwist = carrytwist + fragtwist;
        if(carrytwist >= 1){
          myservo1.write(curPitch+(count*cmpResult)+1);
          carrytwist = carrytwist - 1;
          delay(delaytime);
        }
        else{ 
          myservo1.write(curPitch+(count*cmpResult));
          delay(delaytime);
        }
        
        myservo2.write(curRoll+count);
        delay(delaytime);
        }
      }
   else if(abs(cmpP) <= abs(cmpR)){
      cmpResult = cmpR/cmpP;
      fragtwist = cmpR%cmpP;
      for(int count = 0;count<=abs(cmpP);count+=1){
        carrytwist = carrytwist + fragtwist;
        if(carrytwist >= 1){
          myservo2.write(curRoll+(count*cmpResult)+1);
          carrytwist = carrytwist - 1;
          delay(delaytime);
        }
        else{ 
          myservo2.write(curRoll+(count*cmpResult));
          delay(delaytime);
        }
        myservo1.write(curPitch+count);
        delay(delaytime);
      }
    }
  }
  else if(curPitch>77){
    for(int count = 0;count<abs(cmpP);count+=1){
      myservo1.write(curPitch-count);
      delay(delaytime);
    }
  }
  else if(curPitch<77){
    for(int count = 0;count<abs(cmpP);count+=1){
      myservo1.write(curPitch+count);
      delay(delaytime);
    }  
  }
  else if(curRoll>90){
    for(int count = 0;count<abs(cmpR);count+=1){
      myservo2.write(curRoll-count);
      delay(delaytime);
    }
  }
  else if(curRoll<90){
    for(int count = 0;count<abs(cmpR);count+=1){
      myservo2.write(curRoll+count);
      delay(delaytime);
    }  
  }
}

void twistt(float deg, int brushup, int delaytime){

  int curPitch  = myservo1.read();
  int curRoll   = myservo2.read();
  int minPitch = map(brushup,0,75,77,2);
  int maxPitch = map(brushup,0,75,77,152);
  int minRoll = map(brushup,0,75,90,15);
  int maxRoll = map(brushup,0,75,90,165);
  
  float tmpPitch = cos(deg);
  float tmpRoll = sin(deg);
  
  int cmpP,cmpR;
  int cmpResult;
  float carrytwist = 0,fragtwist;
  tmpPitch = map(tmpPitch*100,-100,100,minPitch,maxPitch);
  tmpRoll  = map(tmpRoll*100,-100,100,minRoll,maxRoll);

  if(curPitch > tmpPitch && curRoll > tmpRoll){
    cmpP = tmpPitch-curPitch;
    cmpR = tmpRoll-curRoll;
    if(abs(cmpP) > abs(cmpR)){
      cmpResult = cmpP/cmpR;
      fragtwist = cmpP%cmpR;
      for(int count = 0;count<abs(cmpR);count+=resolution){
        carrytwist = carrytwist + fragtwist;
        if(carrytwist >= 1){
          myservo1.write(curPitch-(count*cmpResult)+1);
          carrytwist = carrytwist - 1;
          delay(delaytime);
        }
        else {
          myservo1.write(curPitch-(count*cmpResult));
          delay(delaytime);
        }
        myservo2.write(curRoll-count);
        delay(delaytime);
      }
    }
    else if(abs(cmpP) <= abs(cmpR)){
      cmpResult = cmpR/cmpP;
      fragtwist = cmpR%cmpP;
      for(int count = 0;count<abs(cmpP);count+=resolution){
        carrytwist = carrytwist + fragtwist;
        if(carrytwist >= 1){
          myservo2.write(curRoll-(count*cmpResult)-1);
          carrytwist = carrytwist - 1;
          delay(delaytime);
        }
        else {
          myservo2.write(curRoll-(count*cmpResult));
          delay(delaytime);
        }
        myservo1.write(curPitch-count);
        delay(delaytime);
      }
    }
  }
  else if(curPitch > tmpPitch && curRoll < tmpRoll){
    cmpP = abs(tmpPitch-curPitch);
    cmpR = abs(tmpRoll-curRoll);
    if(abs(cmpP) > abs(cmpR)){
      cmpResult = cmpP/cmpR;
      fragtwist = cmpP%cmpR;
      for(int count = 0;count<abs(cmpR);count+=resolution){
        carrytwist = carrytwist + fragtwist;
        if(carrytwist >= 1){
          myservo1.write(curPitch-(count*cmpResult)-1);
          carrytwist = carrytwist - 1;
          delay(delaytime);
        }
        else{ 
          myservo1.write(curPitch-(count*cmpResult));
          delay(delaytime);
        }
        myservo2.write(curRoll+count);
        delay(delaytime);
      }
    }
    else if(abs(cmpP) <= abs(cmpR)){
      cmpResult = cmpR/cmpP;
      fragtwist = cmpR%cmpP;
      for(int count = 0;count<abs(cmpP);count+=resolution){
        carrytwist = carrytwist + fragtwist;
        if(carrytwist >= 1){
          myservo2.write(curRoll+(count*cmpResult)+1);
          carrytwist = carrytwist - 1;
          delay(delaytime);
        }
        else{ 
          myservo2.write(curRoll+(count*cmpResult));
          delay(delaytime);
        }
        myservo1.write(curPitch-count);
        delay(delaytime);
      }
    }
  }
  else if(curPitch < tmpPitch && curRoll > tmpRoll){
    cmpP = abs(tmpPitch-curPitch);
    cmpR = abs(tmpRoll-curRoll);
    if(abs(cmpP) > abs(cmpR)){
      cmpResult = cmpP/cmpR;
      fragtwist = cmpP%cmpR;
      for(int count = 0;count<abs(cmpR);count+=resolution){
        carrytwist = carrytwist + fragtwist;
        if(carrytwist >= 1){
          myservo1.write(curPitch+(count*cmpResult)+1);
          carrytwist = carrytwist - 1;
          delay(delaytime);
        }
        else{
          myservo1.write(curPitch+(count*cmpResult));
          delay(delaytime);
        }
        myservo2.write(curRoll-count);
        delay(delaytime);
      }
    }
    else if(abs(cmpP) <= abs(cmpR)){
      cmpResult = cmpR/cmpP;
      fragtwist = cmpR%cmpP;
      for(int count = 0;count<abs(cmpP);count+=resolution){
        carrytwist = carrytwist + fragtwist;
        if(carrytwist >= 1){
          myservo2.write(curRoll-(count*cmpResult)-1);
          carrytwist = carrytwist - 1;
          delay(delaytime);
        }
        else {
          myservo2.write(curRoll-(count*cmpResult));
          delay(delaytime);
        }
        myservo1.write(curPitch+count);
        delay(delaytime);
      }
    }
  }
  else if(curPitch < tmpPitch && curRoll < tmpRoll){
    cmpP = abs(tmpPitch-curPitch);
    cmpR = abs(tmpRoll-curRoll);
    if(abs(cmpP) > abs(cmpR)){
      cmpResult = cmpP/cmpR;
      fragtwist = cmpP%cmpR;
      for(int count = 0;count<abs(cmpR);count+=resolution){
        carrytwist = carrytwist + fragtwist;
        if(carrytwist >= 1){
          myservo1.write(curPitch+((count*cmpResult)+1));
          carrytwist = carrytwist - 1;
          delay(delaytime);
        }
        else {
          myservo1.write(curPitch+(count*cmpResult));
          delay(delaytime);
        }
        myservo2.write(curRoll+count);
        delay(delaytime);
      }
    }
    else if(abs(cmpP) <= abs(cmpR)){
      cmpResult = cmpR/cmpP;
      fragtwist = cmpR%cmpP;
      for(int count = 0;count<abs(cmpP);count+=resolution){
        carrytwist = carrytwist + fragtwist;
        if(carrytwist >= 1){
          myservo2.write(curRoll+(count*cmpResult)+1);
          carrytwist = carrytwist - 1;
          delay(delaytime);
        }
        else {
          myservo2.write(curRoll+(count*cmpResult));
          delay(delaytime);
        }
        myservo1.write(curPitch+count);
        delay(delaytime);
      }
    }
  }
  else if(curPitch < tmpPitch){
    cmpP = abs(tmpPitch-curPitch);
    for(int count = 0; count < abs(cmpP); count+=resolution){
      myservo1.write(curPitch+count);
      delay(delaytime);
    }
  }
  else if(curPitch > tmpPitch){
    cmpP = abs(tmpPitch-curPitch);
    for(int count = 0; count < abs(cmpP); count+=resolution){
      myservo1.write(curPitch-count);
      delay(delaytime);
    }
  }
  else if(curRoll < tmpRoll){
    cmpR = abs(tmpRoll-curRoll);
    for(int count = 0; count < abs(cmpR); count+=resolution){
      myservo2.write(curRoll+count);
      delay(delaytime);
    }
  }
  else if(curRoll > tmpRoll){
    cmpR = abs(tmpRoll-curRoll);
    for(int count = 0; count < abs(cmpR); count+=resolution){
      myservo2.write(curRoll-count);
      delay(delaytime);
    }
  }
}

void twist(int theta, int falpha, int balpha, int fspeed, int bspeed, float brushlength){
  int btheta = theta-180;
  float tmpXYoffset = DegreestoRadians(balpha);
  float XYoffset = sin(tmpXYoffset)*brushlength;
  float tmpT = DegreestoRadians(theta);
  float tmpX = sin(tmpT)*XYoffset;
  float tmpY = cos(tmpT)*XYoffset;
  
  fspeed = map(fspeed,0,100,100,0);
  bspeed = map(bspeed,0,100,100,0);

  twistt(DegreestoRadians(btheta), balpha, bspeed);
  delay(1000);//Serial.print(tmpX);Serial.print(" ");Serial.println(tmpY);
  PBPR.dk_drive2motmmR(0,1,tmpX,tmpY,500);//ไปชดก่อนตวัด
  
  z_controldown(170);//pendown;
  twistt(DegreestoRadians(theta), falpha, fspeed);
  z_controlup(100);//penup;
  delay(1000);
  backHome();
  PBPR.dk_drive2motmmR(0,1,-tmpX,-tmpY,500);//drive -tmpX -tmpY //ถอยกลับหลังตวัด
  z_controlup(100);
}
/******************************************************/
void BrushLinearXYZ(int x,int y,int z,int ispeed){//ฟังก์ชันจุดเริ่มวาดลำต้นไผ่
  z_controlup(100);
  PBPR.dk_drive2motmmA(0,1,-x,y,ispeed);
  z_controldown(z);
}
void end_BrushLinearXYZ(int x,int y,int ispeed){//ฟังก์ชันจุดสิ้นสุดวาดลำต้นไผ่
  PBPR.dk_drive2motmmA(0,1,-x,y,ispeed);
  z_controlup(100);
 
}
void moveto_default(){
  int value_x, value_y;
  value_x = digitalRead(limit_x);
  value_y = digitalRead(limit_y);
  
  while(value_x&&value_y){
    if(value_x){
      PBPR.dk_drivemotor(0,1,50);
      //ซ้ายx_backward();
      delayMicroseconds(200);
    }
    if(value_y){
      PBPR.dk_drivemotor(1,-1,50);//ลงy_backward();
      delayMicroseconds(200);
    }
    value_x = digitalRead(limit_x);
    value_y = digitalRead(limit_y);
    if(!value_x) break;
    if(!value_y) break;
  }
  while(value_y){
    if(value_y){
      PBPR.dk_drivemotor(1,-1,50);//y_backward();
      delayMicroseconds(200);
    }
    value_y = digitalRead(limit_y);
  }
  while(value_x){
    if(value_x){
      PBPR.dk_drivemotor(0,1,50);//x_backward();
      delayMicroseconds(200);
    }
    value_x = digitalRead(limit_x);
  }
}
void z_controlup(int ongsa){
  //myservo3.write(100); 
  myservo3.write(ongsa);
  delay(300); }
void z_controldown(int ongsa){
  //myservo3.write(155);
   myservo3.write(ongsa);
  //myservo3.write(165);//ใช้กับต้นไผ่ 
   delay(300); 
}
void loop(){
  // put your main code here, to run repeatedly:

}
/****************************************************************************************/
