//Libraries 
#include "TFT_eSPI.h" //TFT LCD library 
#include "DHT.h" // DHT library 

//Definitions
#define DHTPIN 0 //Define signal pin of DHT sensor 
// #define DHTPIN PIN_WIRE_SCL //Use I2C port as Digital Port */
#define DHTTYPE DHT22 //Define DHT sensor type 

//Initializations
DHT dht(DHTPIN, DHTTYPE); //Initializing DHT sensor
TFT_eSPI tft; //Initializing TFT LCD
TFT_eSprite spr = TFT_eSprite(&tft); //Initializing buffer
#define BUZZER_PIN PIN_WIRE_SCL /* sig pin of the buzzer */

float threshold=78.0; //********* change to 15 (deg)

char chr = 248; //stores the extended ASCII of a symbol
String status_msg="";
void setup() {
  Serial.begin(115200); //Start serial communication
 
  pinMode(WIO_LIGHT, INPUT); //Set light sensor pin as INPUT
  pinMode(BUZZER_PIN,OUTPUT);
  
  dht.begin(); //Start DHT sensor 
  tft.begin(); //Start TFT LCD
  tft.setRotation(3); //Set TFT LCD rotation
  spr.createSprite(TFT_HEIGHT,TFT_WIDTH); //Create buffer
}

void loop() {
  
  status_msg="    OK!";
  int t = dht.readTemperature(); //Assign variable to store temperature 
  float tf=t*9/5+32.0;
  int h = dht.readHumidity(); //Assign variable to store humidity 
  //int buzzer = analogWrite(WIO_BUZZER); //Assign variable to send buzzer signals
 
  //Setting the title header
  if(tf < threshold) {
    spr.fillSprite(TFT_WHITE); //Fill background with white color
  }else{
    status_msg=" Check Door!";
    spr.fillSprite(TFT_RED); //Fill background with white color
    digitalWrite(BUZZER_PIN, HIGH);
    delay(1000);
    digitalWrite(BUZZER_PIN, LOW);
    delay(2000);
  }
  
  spr.fillRect(0,0,320,50,TFT_DARKGREEN); //Rectangle fill with dark green 
  spr.setTextColor(TFT_WHITE); //Setting text color
  spr.setTextSize(3); //Setting text size 
  spr.drawString("Di's Freezer",50,15); //Drawing a text string 

  spr.drawFastVLine(150,50,190,TFT_DARKGREEN); //Drawing verticle line
  spr.drawFastHLine(0,140,320,TFT_DARKGREEN); //Drawing horizontal line

  //Setting temperature
  spr.setTextColor(TFT_BLACK);
  spr.setTextSize(2);
  spr.drawString("Temperature",10,65);
  spr.setTextSize(3);
  spr.drawNumber(tf,50,95); //Display temperature values 
  //spr.drawChar(chr,96,95);
  spr.drawString("F",95,97);

  //Setting humidity
  spr.setTextSize(2);
  spr.drawString("Humidity",25,160);
  spr.setTextSize(3);
  spr.drawNumber(h,30,190); //Display humidity values 
  spr.drawString("%RH",70,190);

  //Setting light This is in lower right quadrant
  //spr.setTextSize(2);
  //spr.drawString("Light",200,160);
  ///spr.setTextSize(3);
  //light = map(light,0,1023,0,100); //Map sensor values 
  //int light =150; //Map sensor values 
  //spr.drawNumber(light,205,190); //Display sensor values as percentage  
  //spr.drawString("%",245,190);

  //Condition for low soil moisture This is in top right quadrant
   spr.fillRect(160,85,140,45,TFT_YELLOW); //Rectangle fill with dark green 
  spr.setTextColor(TFT_WHITE); //Setting text color
  spr.setTextSize(2); //Setting text size 
   spr.setTextColor(TFT_BLACK); //Setting text color  
   spr.drawString("Status", 180,65);
  spr.drawString(status_msg,155,95); //Drawing Current Temp Status (if over 15 deg. F screen will turn red)
   
  spr.pushSprite(0,0); //Push to LCD
  delay(50);


}
