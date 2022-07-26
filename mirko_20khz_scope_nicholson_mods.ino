// Oscilloscope project for Nano

#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
#include <SPI.h>
#include <FreqCount.h>

Adafruit_PCD8544 display = Adafruit_PCD8544(7, 6, 4, 3, 2);//CLK,DIN,DC,CE,RST |||| VCC +3.3 V , BL ++ 200 OM ++ 3.3 V

int izm,x,y,u,i2,zz,hold,h0,h1,h2,raz=0,menu,sss=512,u_dig,data[168]{};
unsigned long time,times;
float per;
byte i;


// int i,i1,i2,V_min,V_max,V_mid,t,t0,t1,sta,Key=1,hold=0; 
 long Freq;
 float Vpp;
 int Y[96];
 int Buffer[192]; //192




void setup() {Serial.begin(9600);

    display.begin();
    display.clearDisplay();
    display.display();
    display.setContrast(45); // contrast setting
    display.setTextSize(1);  // setting font size
    display.setTextColor(BLACK); // setting text color
    pinMode(10,INPUT_PULLUP);  // +
    pinMode(11,INPUT_PULLUP);  // –
    pinMode(12,INPUT_PULLUP);  // hold
    pinMode(8,INPUT_PULLUP);  // sync
    ADMUX  = 0b01000000; // 0B0100000 10 bit A0 // 0B01100000 8 bit A0
    ADCSRA = 0b11110010;// CLK/4;
    analogWrite (9, 124); // PWM 9 OUTPUT
    //FreqCount.begin(1000);
}

void loop() {
///////////////////////////BUTTON CONTROL//////////////////////////////
/*  if (FreqCount.available()) {
    unsigned long count = FreqCount.read();
    display.setCursor(12,40);
    display.print(count);
    display.print("       ");
    display.display();
  } */

if(menu==0){
  if(digitalRead(10)==HIGH){if(hold==0){raz++;}if(hold==1){i2=i2+1;}delay(100);}
  if(digitalRead(11)==HIGH){if(hold==0){raz--;}if(hold==1&&hold>0){i2=i2-1;}delay(100);}
  }
  if(digitalRead(12)==LOW){hold++;i2=0;delay(100);}
  if(digitalRead(8)==LOW){menu++;delay(100);}
  if(hold>1){hold=0;}if(menu>1||menu<0){menu=0;}
  if(raz<=0){raz=0;}if(raz>8){raz=8;}
  if(menu==1){hold=0;
  if(digitalRead(10)==HIGH){sss+=24;delay(100);}
  if(digitalRead(11)==HIGH){sss-=24;delay(100);}
  if(sss>1023){sss=1023;}if(sss<0){sss=0;}
  }
   display.setCursor(0,0); // setting cursor position

/////////////////////////SWEEP TIME calibrated by generator ////////////////////////////
  if(raz==0){zz=1;h2=2;per=0.1;}
  if(raz==1){zz=1;h2=1;per=0.2;}
  if(raz==2){zz=12;h2=1;per=0.5;}
  if(raz==3){zz=32;h2=1;per=1;}
  if(raz==4){zz=75;h2=1;per=2;}
  if(raz==5){zz=200;h2=1;per=5;}
  if(raz==6){zz=380;h2=1;per=10;}
  if(raz==7){zz=750;h2=1;per=20;}
  if(raz==8){zz=1900;h2=1;per=50;}
///////////////////////////////////////////////////////////////////
if(hold==0&&millis()-time>0){
 ads();while(izm<sss){ads();h0++;if(h0>5000){break;}}h0=0;// SYNCHRONIZATION
times=micros();
while(i<167){i++;delayMicroseconds(zz);
  ads();data[i]=izm; // MEASUREMENT 10 bit
  }i=0;times=micros()-times;
  Serial.println(times);
}
////////////////////OUTPUT ON DISPLAY///////////////////////////////
  if(millis()-time>100){
    display.clearDisplay();
    if(sss<204&&sss>100){u_dig=10;display.setCursor(0,40);display.print("0.4V");}
    else if(sss<100){u_dig=5; display.setCursor(0,40);display.print("0.2V");}
    else{u_dig=25;}
     display.setCursor(0,0);
while(i<167){i++;setka();
    display.drawLine(i*h2-i2, 47-data[i]/u_dig,i*h2-i2+h2-1, 47-data[i+1]/u_dig, BLACK);}i=0;
    display.print(per,1);display.print(" mS  ");
  if(menu==0){if(hold==1){display.print("HOLD   ");}else{display.print("AUTO   ");}}
  if(menu==1){display.print(sss/200.0,1);display.print(" V ");}
  if(menu==1){display.drawLine(0, 48-sss/u_dig,4, 48-sss/u_dig, BLACK);
  }
    time=millis();
}    
   display.display();    
}// loop

void ads(){ //////// 10 bit ///////////
  do{ADCSRA |= (1 << ADSC);}
  while((ADCSRA & (1 << ADIF)) == 0);izm = (ADCL|ADCH << 8);}
  ////////////////// 8 bit ///////////
  // REQUIRED ALL CHANGEABLE VARIABLES (sss, u_dig)
  // while ((ADCSRA & 0x10)==0);
  //   ADCSRA|=0x10;
  //   izm = ADCH;
  
void setka(){
for(y=8;y<47;y=y+8){
for(x=0;x<83;x=x+4){
   display.drawPixel(x, y, BLACK);}}
 for(x=0;x<83;x=x+26){
for(y=10;y<47;y=y+4){
   display.drawPixel(x, y, BLACK);}}
   }

 
