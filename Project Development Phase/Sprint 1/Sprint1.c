#include<LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(32, 16, 2);
int green = 2;
int yellow = 3;
int red = 4;
int siren = 5;
int gas = A0;
int sensorValue = 0;
void setup()
{
  Serial.begin(9600);
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
  pinMode(green, OUTPUT);
  pinMode(yellow, OUTPUT);
  pinMode(red, OUTPUT);
  pinMode(siren, OUTPUT);
  digitalWrite(red, LOW);
  digitalWrite(yellow, LOW);
  digitalWrite(green, LOW);
}
void loop()
{
  sensorValue = analogRead(gas);
  Serial.println(sensorValue);
  lcd.setCursor(11,0);
  lcd.print(sensorValue);
  if(sensorValue > 500)
  {
    lcd.setCursor(0,1);
    lcd.print("GAS DETECTED");
    digitalWrite(red, HIGH);
    digitalWrite(yellow, LOW);
    digitalWrite(green, LOW);
    tone(siren, 200);
  }
  else if(sensorValue > 281 && sensorValue < 500)
  {
    lcd.setCursor(0,1);
    lcd.print("            ");
    digitalWrite(yellow, HIGH);
    digitalWrite(red, LOW);
    digitalWrite(green, LOW);
    noTone(siren);
  }
  else
  {
    lcd.setCursor(0,1);
    lcd.print("            ");
    digitalWrite(green, HIGH);
    digitalWrite(red, LOW);
    digitalWrite(yellow, LOW);
    noTone(siren);
  }
  delay(1000);
}