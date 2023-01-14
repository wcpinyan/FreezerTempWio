//Libraries 
#include "TFT_eSPI.h" //TFT LCD library 
#include "DHT.h" // DHT library 


//Definitions
#define DHTPIN 0 //Define signal pin of DHT sensor 
// #define DHTPIN PIN_WIRE_SCL //Use I2C port as Digital Port */
#define DHTTYPE DHT22 //Define DHT sensor type 
float threshold=77.0;
//Initializations
DHT dht(DHTPIN, DHTTYPE); //Initializing DHT sensor
TFT_eSPI tft; //Initializing TFT LCD
TFT_eSprite spr = TFT_eSprite(&tft); //Initializing buffer

String highmsg="";
bool isTempHigh;
float ftemp;
//Soil Moisture Sensor definitions
//int sensorPin = A1; //Define variable to store soil moisture sensor pin
//int sensorValue = 0; //Define variable to store soil moisture sensor value

void setup() {
  Serial.begin(115200);
  
  
  //pinMode(WIO_LIGHT, INPUT); //Set light sensor pin as INPUT
  pinMode(WIO_BUZZER, OUTPUT); //Set buzzer pin as OUTPUT
  
  dht.begin(); //Start DHT sensor 
  tft.begin(); //Start TFT LCD
  tft.setRotation(3); //Set TFT LCD rotation
 
  spr.createSprite(TFT_HEIGHT,TFT_WIDTH); //Create buffer
}

void loop() {
  
  
  float t = dht.readTemperature(); //Assign variable to store temperature 
  float h = dht.readHumidity(); //Assign variable to store humidity 
  //int light = analogRead(WIO_LIGHT); //Assign variable to store light sensor values
  ftemp=(1.8*t)+32;
   //flash background if temp is above threshold
  isTempHigh=CheckForHighTemp();
  //Setting the title header 

  if(isTempHigh){    
    spr.fillSprite(TFT_RED);
    // Buzzer
    analogWrite(WIO_BUZZER,150);
    delay(1000);
    analogWrite(WIO_BUZZER,0);;
    delay(1000);
    //draw hightemp msg.
    spr.fillRect(45,130,215,40,TFT_BLACK); //Rectangle fill with dark green
    spr.setTextColor(TFT_WHITE); //Setting text color
    highmsg="Check the door!";
    spr.setTextSize(2);
    spr.drawString(highmsg,55,140);
  }else {   
    spr.fillSprite(TFT_WHITE);
  }
  spr.fillRect(0,0,400,55,TFT_DARKGREEN); //Rectangle fill with dark green 
  spr.setTextColor(TFT_WHITE); //Setting text color
  spr.setTextSize(3); //Setting text size   
  spr.drawString("Freezer",90,15); //Drawing a text string 

  //spr.drawFastVLine(150,50,190,TFT_DARKGREEN); //Drawing verticle line
  spr.drawFastHLine(0,140,320,TFT_DARKGREEN); //Drawing horizontal line

  //Setting temperature
  spr.setTextColor(TFT_BLACK);
  spr.setTextSize(2);
  spr.drawString("Temperature",85,65);
  spr.setTextSize(4);
 
  spr.drawFloat(ftemp,1,85,95); //Display temperature values 
  spr.drawString("F",200,95);
  
  Serial.println(t);
  Serial.println(ftemp);
  
  //Setting humidity
  spr.setTextSize(2);
  spr.drawString("Humidity",100,160);
  spr.setTextSize(4);
  spr.drawNumber(h,90,190); //Display humidity values 
  spr.drawString("%RH",150,190);

  //Setting soil moisture
  //sensorValue = analogRead(sensorPin); //Store sensor values 
  //sensorValue = map(sensorValue,1023,400,0,100); //Map sensor values 
  //spr.setTextSize(2);
  //spr.drawString("Soil Moisture",160,65);
  //spr.setTextSize(3);
  //spr.drawNumber(sensorValue,200,95); //Display sensor values as percentage  
  //spr.drawString("%",240,95);
  
  //Setting light 
  //spr.setTextSize(2);
  //spr.drawString("Light",200,160);
  //spr.setTextSize(3);
  //light = map(light,0,1023,0,100); //Map sensor values 
 // spr.drawNumber(light,205,190); //Display sensor values as percentage  
 // spr.drawString("%",245,190);

  //Condition for low soil moisture
  /*
  if(sensorValue < 50){
    spr.fillSprite(TFT_RED);
    spr.drawString("Time to water!",35,100);
    analogWrite(WIO_BUZZER, 150); //beep the buzzer
    delay(1000);
    analogWrite(WIO_BUZZER, 0); //Silence the buzzer
    delay(1000);
  }
*/
  spr.pushSprite(0,0); //Push to LCD
  delay(50);
}
bool CheckForHighTemp(){
  if(ftemp > threshold){
    return true;
  }
  else return false;
 
}