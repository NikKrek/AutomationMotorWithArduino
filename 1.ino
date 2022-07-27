#include <Wire.h> 
#include <LiquidCrystal_I2C.h> 

#include "DHT.h"

#include <Servo.h>
Servo servo;
int angle = 0;
int millisEngene;

#define DHTPIN 2
DHT dht(DHTPIN, DHT11);

unsigned long timing;

#define R 10000 
const int sensorPrPin1 = A1; 
const int sensorPrPin2 = A2; 
const int sensorPrPin3 = A3; 
int ValPr1;
int ValPr2;
int ValPr3;
int lux1; 
int lux2;
int lux3;
int luxMid;
int lux1And2;
int lux2And3;
int lux3And1;

LiquidCrystal_I2C LCD(0x27,16,2);

int regPotValRaw = analogRead(0);
int regPotVal;

int redP= 6;
int grnP = 5;
int bluP = 3;

boolean butt;
unsigned long timingButt;
int flagButt = 1;

unsigned long timingLux;

void setup() {

Serial.begin(9600);

servo.attach(8);
servo.write(angle);

Serial.println("Humidity, Tempure, MaxTempure, LUX");
Serial.flush();
  
  dht.begin();
    
   LCD.begin();   
   LCD.backlight(); 
   
   LCD.setCursor(0,0);   
   LCD.print("   NikKrek");  
   delay(1000);

   LCD.setCursor(0,0);    
   LCD.print("H=humidity");
   LCD.setCursor(0,1);    
   LCD.print("Tem=temperature");  
   delay(4000);
   LCD.clear();

  pinMode(redP, OUTPUT);
  pinMode(grnP, OUTPUT);
  pinMode(bluP, OUTPUT);

 pinMode(7, INPUT_PULLUP);

}

void loop() {

  int h = dht.readHumidity(); 
  float t = dht.readTemperature(); 
  
regPotVal = analogRead(regPotValRaw);
regPotVal = map(regPotVal, 0, 1023, 20, 40);
regPotVal = constrain(regPotVal, 20, 40);

butt = !(digitalRead(7));
  if ( butt == 1) {
    if (millis() - timingButt > 350){ 
  timingButt = millis();
  flagButt++;
  if(flagButt > 3) {
    flagButt = 1;
  }
    }
  }

if (flagButt == 3) {

ValPr1 = analogRead(sensorPrPin1);
ValPr2 = analogRead(sensorPrPin2);
ValPr3 = analogRead(sensorPrPin3);

lux1 = sensorRawToPhys(ValPr1);
lux2 = sensorRawToPhys(ValPr2);
lux3 = sensorRawToPhys(ValPr3);

luxMid = (lux1 + lux2 + lux3);
luxMid = (luxMid / 3);

//Serial.print("1  = ");
//Serial.println(lux1); 
//Serial.print("2  = ");
//Serial.println(lux2); 
//Serial.print("3  = ");
//Serial.println(lux3); 
Serial.print(h);
  Serial.print(",");
  Serial.print(t);
  Serial.print(",");
  Serial.print(regPotVal);
Serial.print(",");
Serial.println(luxMid);

if(lux1 + lux2 >=100 && lux3 < 90) {
  //Serial.println("fire"); 
 //Serial.println("probably LDR3 broken");
  LCD.setCursor(0, 0);
   LCD.print("fire LUX:");
   lux1And2 = (lux1 + lux2);
   lux1And2 = (lux1And2 / 2);
   LCD.print(lux1And2);
   if (luxMid <10) LCD.print ("   "); 
   if (luxMid >=10 && luxMid <100) LCD.print ("  ");
   if (luxMid >=100 && luxMid <1000) LCD.print (" "); 

   LCD.setCursor(0, 1);
   LCD.print("MB LDR3 BKN");
}
if(lux2 + lux3 >=100 && lux1 < 90) {
  //Serial.println("fire"); 
  //Serial.println("maybe LDR1 BKN");
    LCD.setCursor(0, 0);
   LCD.print("fire LUX:");
   lux2And3 = (lux2 + lux3);
   lux2And3 = (lux2And3 / 2);
   LCD.print(lux2And3);
   if (luxMid <10) LCD.print ("   "); 
   if (luxMid >=10 && luxMid <100) LCD.print ("  ");
   if (luxMid >=100 && luxMid <1000) LCD.print (" "); 
   LCD.setCursor(0, 1);
   LCD.print("MB LDR1 BKN");
}
if(lux3 + lux1 >=100 && lux2 < 90) {
  //Serial.println("fire"); 
  //Serial.println("maybe LDR2 broken");
    LCD.setCursor(0, 0);
   LCD.print("fire LUX:");
   lux3And1 = (lux3 + lux1);
   lux3And1 = (lux3And1 / 2);
   LCD.print(lux3And1);
   if (luxMid <10) LCD.print ("   "); 
   if (luxMid >=10 && luxMid <100) LCD.print ("  ");
   if (luxMid >=100 && luxMid <1000) LCD.print (" "); 
   LCD.setCursor(0, 1);
   LCD.print("MB LDR2 BKN");
}
if(lux1 + lux2 + lux3 > 270) {
  //Serial.println("fire"); 
    LCD.setCursor(0, 0);
   LCD.print("fire LUX:");
   LCD.print(luxMid);
   if (luxMid <10) LCD.print ("   "); 
   if (luxMid >=10 && luxMid <100) LCD.print ("  ");
   if (luxMid >=100 && luxMid <1000) LCD.print (" "); 
  LCD.setCursor(0, 1);
}
if(lux1 + lux2 + lux3 <= 270) {
  //Serial.println("not fire"); 
      LCD.setCursor(0, 0);
   LCD.print("not fire LUX:");
   LCD.print(luxMid);
   if (luxMid <10) LCD.print ("   "); 
   if (luxMid >=10 && luxMid <100) LCD.print ("  ");
   if (luxMid >=100 && luxMid <1000) LCD.print (" "); 
   LCD.setCursor(0, 1);
   LCD.print("                           ");
}
delay(1000);

if (millis() - timingLux > 10000){ // Вместо 10000 подставьте нужное вам значение паузы 
  timingLux = millis();
 LCD.clear();
}
//delay(500);  
}
  if (flagButt == 2) {
  
  LCD.setCursor(0,0);
   LCD.print ("Regulation Max   ");
   LCD.setCursor(0,1);
   LCD.print ("Temperature = ");
   
  LCD.print (regPotVal);
   if (regPotVal <10) LCD.print ("   "); 
   if (regPotVal >=10 && lux1 <100) LCD.print ("  ");
   if (regPotVal >=100 && lux1 <1000) LCD.print (" ");
}


if (flagButt == 1) {
  
    LCD.setCursor(0, 0);
   LCD.print("H:");
   LCD.print (h);
   LCD.print("% ");
   LCD.print("Tem:");
   LCD.print(t);
   LCD.print("C");

  Serial.print(h);
  Serial.print(",");
  Serial.print(t);
  Serial.print(",");
  Serial.println(regPotVal);
  
ValPr1 = analogRead(sensorPrPin1);
lux1 = sensorRawToPhys(ValPr1);

LCD.setCursor(0, 1);
   LCD.print("Lux:");
   LCD.print (lux1);
   if (lux1 <10) LCD.print ("   "); 
   if (lux1 >=10 && lux1 <100) LCD.print ("  ");
   if (lux1 >=100 && lux1 <1000) LCD.print (" "); 

   LCD.print(" ");

   

   if (t >= regPotVal)
   {
    RGB_color(255, 0, 0); // Red
    
    LCD.print("hot              ");
    servo.write(180);
    delay(30);
   }
   else {
    RGB_color(0, 0, 0); // Green
    
     LCD.print("                     ");
     servo.write(10);
     delay(30);
   }



delay(200);

}


    
}

int sensorRawToPhys(int raw){
float Vout = float(raw) * (5 / float(1023));// Conversion analog to voltage
float RLDR = (R * (5 - Vout))/Vout; // Conversion voltage to resistance
int phys=500/(RLDR/1000); // Conversion resitance to lumen
return phys;
}

void RGB_color(int redVal, int grnVal, int bluVal)
 {
  analogWrite(redP, redVal);
  analogWrite(grnP, grnVal);
  analogWrite(bluP, bluVal);
}
