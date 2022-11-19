#include<Servo.h>
#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include<LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(32, 16, 2);
int GPSBaud = 9600;
TinyGPSPlus gps;
SoftwareSerial sgps(13, 15); //Rx ,  Tx gps
SoftwareSerial sgsm(3, 1); // Rx ,  Tx gsm
#define KNOB 3
#define LEVER 2
Servo myservo;
int gas = A5;
int sensorValue = 0;
bool gateClosed = true;


void setup() 
{
  Serial.begin(9600);
  pinMode(LEVER, INPUT);
  myservo.attach(KNOB);
  myservo.write(90);
  sgsm.begin(9600);
  sgps.begin(9600);
  lcd.init();
  lcd.clear();
  lcd.backlight();
  lcd.setCursor(3,0);
  lcd.print("GAS LEAKAGE");
  lcd.setCursor(4,1);
  lcd.print("DETECTION");
  delay(3000);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Gas Value: ");
}

void loop() 
{
  sensorValue = analogRead(gas);
  Serial.println(sensorValue);
  if(sensorValue > 500 && !gateClosed)
  {
    Serial.println("GAS DETECTED");
    lcd.setCursor(0,1);
    lcd.print("GAS DETECTED  ");
    sendSMS("GAS IS DETECTED!!");
    myservo.write(90);
    gateClosed = true;
    sendSMS("THE KNOB IS CLOSED");
    lcd.setCursor(0,1);
    lcd.print("KNOB IS CLOSED");
    delay(1000);
  }
  else if(sensorValue > 500 && gateClosed)
  {
    Serial.println("GAS DETECTED");
    lcd.setCursor(0,1);
    lcd.print("GAS DETECTED  ");
    sendSMS("GAS IS DETECTED!!");
    sendSMS("THE KNOB IS ALREADY CLOSED");
    lcd.setCursor(0,1);
    lcd.print("KNOB IS CLOSED");
    delay(1000);
  }
  else
  {
    byte buttonState = digitalRead(LEVER);
    if(buttonState == HIGH)
    {
      myservo.write(0);
      gateClosed = false;
      Serial.println("GATE IS OPENED");
    }
    else
    {
      myservo.write(90);
      gateClosed = true;
      Serial.println("GATE IS CLOSED");
    }
  }
}
void sendSMS(char*message)
{
  while (sgps.available() > 0)
    if (gps.encode(sgps.read()))
    {
      if (gps.location.isValid())
      {
        sgsm.listen();
        sgsm.print("\r");
        delay(1000);
        sgsm.print("AT+CMGF=1\r"); // AT COMMAND TO SEND SMS
        delay(1000);
        /*Replace XXXXXXXXXX to 10 digit mobile number &
        ZZ to 2 digit country code*/
        sgsm.print("AT+CMGS=\"+919025681637\"\r"); // REGISTERED NUMBER TO SEND SMS 
        delay(1000);
        //The text of the message to be sent.
        sgsm.print(message);
        sgsm.print("https://www.google.com/maps/?q="); // MAPS 
        sgsm.print(gps.location.lat(), 6); // LAT
        sgsm.print(",");
        sgsm.print(gps.location.lng(), 6); // LONG    delay(1000);
        sgsm.write(0x1A);
        delay(1000);
      }
    }
}