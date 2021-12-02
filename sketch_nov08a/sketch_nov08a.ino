#include <Wire.h> 
 #include  <LiquidCrystal_I2C.h>
#include <DHT.h>  
//#include <CurieTime.h>
int DHT11value; 
#define relay_Temp 10 // temp relay
#define relay_Humid 11//humid relay
#define DHTTYPE DHT11 
#define maxTemp 28
#define DHT_A_PIN 8
#define DHT_A_TYPE DHT11
DHT dht_A(DHT_A_PIN, DHT_A_TYPE); //For DHT_A
unsigned long currentTime  = 0; //this variable will be overwritten by millis() each iteration of loop
unsigned long pastTime     = 0; //no time has passed yet
int   currentState = 0; //the default state
int seconds = 0;
int minutes = 0;
int state=0;
//int count_on=0;
#define DHT_B_PIN 9
#define DHT_B_TYPE DHT11
float humid, temp;// total temp and humid
DHT dht_B(DHT_B_PIN, DHT_B_TYPE);
int minHumid=50; 
LiquidCrystal_I2C lcd (0x27, 16, 2);
byte degree[8] ={ B00001100,B00010010, B00010010, B00001100,B00000000, B00000000, B00000000, B00000000,};               
 void setup()
  {
    
    pinMode(2, INPUT);//pin kelid1
    pinMode(3, INPUT); //pin kelid2
    pinMode(4, INPUT); //pin kelid3
    pinMode(5, INPUT); //pin kelid4
    Serial.begin(9600); 
    pinMode(relay_Temp,OUTPUT);
    pinMode(relay_Humid,OUTPUT);

  Wire.begin();
  lcd.begin(16,2); 
  lcd.clear(); 
  lcd.createChar(0, degree);
  lcd.setBacklight(HIGH);

  } 


void loop()
{
///////take humid and temp
float humid1 = dht_A.readHumidity(); // resd humid from first DHT11
float temp1 = dht_A.readTemperature(); //read temp from first DHT11
float humid2 = dht_B.readHumidity(); // resd humidfrom secound DHT11
float temp2 = dht_B.readTemperature(); //read tempfrom secound DHT11
Serial.print("humid1  ");
Serial.println(humid1);
Serial.print("humid2  ");
Serial.println(humid2);

temp=(temp1+temp2)/2;
humid=(humid1+humid2)/2;
Serial.print("humid  ");
Serial.println(humid);
////////////set time
  pastTime    =currentTime; //currentTime at this point is the current time from the previous iteration, this should now be pastTime
 currentTime = millis();    //currentTime is now the current time (again).
 
 unsigned long timePassed = currentTime - pastTime; //this is roll-over proof, if currentTime is small, and pastTime large, the result rolls over to a small positive value, the time that has passed

 //////////////////TIMER STARTdigitalWrite(relay_Temp, HIGH);&& relay_Temp==LOW
//delay(300);
//digitalWrite(relay_Temp, LOW);
//if ( timePassed >= 1000 );           // increasing variable "seconds" every second
 //{
  Serial.print("past   ");
  
  seconds +=2;
 Serial.println(seconds);
 // pastTime = currentTime;
//// } 
 if(seconds==60){
  if (state==0)                         // sending signal HIGH ( or low ) to relay when "seconds" reaches 60 and reseting variable
 {
  Serial.println("High ");
   digitalWrite(relay_Temp, HIGH); 
   state=1;
   seconds = 0;
 }
 if (state==1)   {
  
  Serial.println("low ");
  minutes+=1;
  seconds=0;
 }
 }
 Serial.print("second   ");
  Serial.println(seconds);
 Serial.print("minute   ");
  Serial.println(minutes);
 if(minutes==10){
  minutes=0;
  digitalWrite(relay_Temp, LOW);
  state =0;
 // delay(200);
 }
//minutes=minute();
//if((minutes-count_on)%10==0){
//   relay_Temp=LOW;
//}
  
//if ( seconds == 10 &&relay_Temp==0)                         // sending signal HIGH ( or low ) to relay when "seconds" reaches 60 and reseting variable
// {
//  Serial.println("10s past ");
//   digitalWrite(relay_Temp, HIGH); 
//   
//   seconds = 0;
//   
//  }
         
        //TIMER END    
//////////////////////////////////humid control       

if(humid<minHumid){
   Serial.print("minHumid ");
  Serial.println(minHumid);
  digitalWrite(relay_Humid,LOW); //Active relay 
  delay(100); 
}else{
   Serial.print("not minHumid ");
  Serial.println(humid,DEC);
 digitalWrite(relay_Humid,HIGH); 
  delay(100); 
}

 if(digitalRead(2)){ //kelid 1
     // if(minHumid<=50){
        minHumid+=5;
     // }
    
    }
    if(digitalRead(3)){ //kelid 2
     
      if(minHumid>=15){
        minHumid-=5;
      }
    
    }
///////////////////////////////print min humid in serial
       Serial.println();
       Serial.println("min Humid is:");
       Serial.println(minHumid);
 ////////////////print min humid and temp on lcd
lcd.setCursor(0,0);
lcd.print("Temp : "); //print temp
lcd.print(" ");
lcd.setCursor(7,0);
lcd.print(temp1,1);
lcd.setCursor(12,0);
lcd.write((byte)0);
lcd.setCursor(0,1);
lcd.print("Humid: "); // print humid
lcd.print(" ");
lcd.setCursor(7,1);
lcd.print(minHumid,1);
lcd.setCursor(12,1);
lcd.print("%");
delay(2000);
}
/////////////////temp control
//if(temp>maxTemp){
//  Serial.print("maxTemp");
//  Serial.println(maxTemp);
//   digitalWrite(relay_Temp,LOW); //ACtive relay
//
//  delay(100); 
//}else{
//   Serial.print(" not maxTemp ");
//  Serial.println(temp);
//  digitalWrite(relay_Temp,HIGH);
//  delay(100); 
//}
