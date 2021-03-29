#include <SD.h>
#include <SPI.h> 
#include <dk_Stepper6.h>
#include <String.h>
#include <Servo.h>
const int chipSelect = 53;
const int limit_x = 5;
const int limit_y = 4;
dk_Stepper6 PBPR;
Servo myservo1,myservo2,myservo3;

  int theta = 45;
  int falpha = 50; //ระดับยกปลายพู่กัน เป็น % (0-100);
  int balpha = 30;
  int resolution = 2;  //ความละเอียดของ Servo ;
  int fspeed = 90;
  int bspeed = 20;
  int delaytime = 20;
  float brushlength = 65; //ความยาวพู่กันจากจุดหมุนถึงปลายพู่กัน 

void setup()
{
  Serial.begin(9600);
  PBPR.dk_configStepmot(0,11,10);
  PBPR.dk_configStepmot(1,9,8);
  PBPR.dk_configMech(0,400,6);
  PBPR.dk_configMech(1,400,12);
  myservo1.attach(14);//pitch
  myservo2.attach(16);//roll
  myservo3.attach(18);//z  
  myservo1.write(77);
  myservo2.write(90);

  read_sd();
  //flower();
  ///moveto_default();
  //bamboo();
 // bamboo2();
}

void read_sd(){
  Serial.print("Initializing SD card...");
  pinMode(53, OUTPUT);
   if (!SD.begin(chipSelect)) {
    
    Serial.println("Card failed");
    return;
  }Serial.println("ok");  
  File myFile; 
  myFile = SD.open("nike.txt"); // สั่งให้เปิดไฟล์ชื่อ test.txt เพื่ออ่านข้อมูล
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
          PBPR.dk_drive2motmmA(0,1,-valX,valY,400);
          z_controldown(155);
          countstart = 1;
       }
       else if ( countstart == 1) {
          PBPR.dk_drive2motmmA(0,1,-valX,valY,400);
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
  } 
  else {
    Serial.println("error opening .txt file");
  } 
  
}
void flower(){
  PBPR.dk_drive2motmmR(0,1,-100,100,500);
  twist(0, 50, 27, 95, 50, brushlength);
  twist(10, 50, 27, 95, 50, brushlength);
  twist(22, 50, 27, 95, 50, brushlength);
  twist(45, 50, 27, 95, 50, brushlength);
  twist(67, 50, 27, 95, 50, brushlength);
  twist(80, 50, 27, 95, 50, brushlength);
  twist(90, 50, 27, 95, 50, brushlength);
  twist(100, 50, 27, 95, 50, brushlength);
  twist(112, 50, 27, 95, 50, brushlength);
  twist(135, 50, 27, 95, 50, brushlength);
  twist(157, 50, 27, 95, 50, brushlength);
  twist(170, 50, 27, 95, 50, brushlength);
  twist(180, 50, 27, 95, 50, brushlength);
  twist(190, 50, 27, 95, 50, brushlength);
  twist(202, 50, 27, 95, 50, brushlength);
  twist(225, 50, 27, 95, 50, brushlength);
  twist(247, 50, 27, 95, 50, brushlength);
   twist(260, 50, 27, 95, 50, brushlength);
  twist(270, 50, 27, 95, 50, brushlength);
  twist(292, 50, 27, 95, 50, brushlength);
  twist(315, 50, 27, 95, 50, brushlength);
  twist(337, 50, 27, 95, 50, brushlength);
  twist(350, 50, 27, 95, 50, brushlength);
  moveto_default();
}
/************************************************************/
void bamboo(){
  int speed1 = 500;
   BrushLinearXYZ(50,30,170,speed1);
   end_BrushLinearXYZ(55,70,speed1);
   twist(300, 50, 27, 95, 50, brushlength);
   twist(270, 50, 27, 95, 50, brushlength);
   BrushLinearXYZ(55,70,170,speed1);
   end_BrushLinearXYZ(60,120,speed1);
   twist(315, 50, 27, 95, 50, brushlength);
   twist(90, 50, 27, 95, 50, brushlength);
   twist(120, 50, 27, 95, 50, brushlength);
   BrushLinearXYZ(60,120,170,speed1);
   end_BrushLinearXYZ(65,170,speed1);
   twist(40, 50, 27, 95, 50, brushlength);
   twist(60, 50, 27, 95, 50, brushlength);
   BrushLinearXYZ(65,170,175,speed1);
   end_BrushLinearXYZ(70,220,speed1);
   twist(300, 50, 27, 95, 50, brushlength);
   twist(45, 50, 27, 95, 50, brushlength);
   moveto_default();
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
   twist(180, 50, 27, 95, 50, brushlength);
 
    moveto_default();
}

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
  
  z_controldown(165);//pendown;
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
/*************************************************************/
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
    if(!value_x)  break;
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
void loop(){
 
}
