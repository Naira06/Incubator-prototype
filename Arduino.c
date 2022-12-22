//Thermistor Sensor 
int ThermistorPin = 0; 
int Vo;
float R1 = 10000;
float logR2, R2, T, Tc, Tf;
float c1 = 1.009249522e-03, c2 = 2.378405444e-04, c3 = 2.019202697e-07;

//Buzzer 
const int buzzer = 6; //buzzer to arduino pin 6

//LDR Sensor
int ldrPin = A4; // LDR on Analog Pin 4

// HumiditySensor
#include "DHT.h"
#define DHTPIN 4     // Digital pin connected to the DHT sensor
#define DHTTYPE DHT11  
DHT dht(DHTPIN, DHTTYPE);

// Fan
#define pwm 7

//LCD
#include <LiquidCrystal.h> // includes the LiquidCrystal Library 
LiquidCrystal lcd(12, 13,11, 10, 9, 8); // Creates an LCD object. Parameters: (rs, enable, d4, d5, d6, d7) 

// SoundSensor
int led = 3;
int sound_analog = A2;
boolean LEDStatus=false;

 void setup() {
pinMode(buzzer, OUTPUT); // Set buzzer - pin 6 as an output
pinMode(ldrPin , INPUT); // Set buzzer -A4 as an output
lcd.begin(16,2); // Initializes the interface to the LCD screen, and specifies the dimensions (width and height) of the display } 
pinMode(led, OUTPUT);
pinMode(sound_analog, INPUT);
Serial.begin(9600);
dht.begin();
}

void loop() {
  ThermistorFun();
  LDRFun();
  HumidityFun();
  SoundFun();
if(ThermistorFun()==0 || LDRFun()==0 || HumidityFun()==0 || SoundFun()==0){
  tone(buzzer, 5000);
    delay(100);
    noTone(buzzer);
    delay(100);
}
 lcd.setCursor(2,1); // Sets the location at which subsequent text written to the LCD will be displayed  
 lcd.clear(); // Clears the display 
 lcd.blink(); //Displays the blinking LCD cursor 
 delay(4000); 
 lcd.setCursor(7,1); 
 delay(3000); 
 lcd.noBlink(); // Turns off the blinking LCD cursor 
 lcd.cursor(); // Displays an underscore (line) at the position to which the next character will be written 
 delay(4000); 
 lcd.noCursor(); // Hides the LCD cursor 
 lcd.clear(); // Clears the LCD screen 
}
//Thermistor Sensor
bool ThermistorFun(){
  Vo = analogRead(ThermistorPin);
  R2 = R1 * (1023.0 / (float)Vo - 1.0);
  logR2 = log(R2);
  T = (1.0 / (c1 + c2*logR2 + c3*logR2*logR2*logR2));
  Tc = T - 273.15;
  Serial.print(" Baby Temperature: "); 
  Serial.print(Tc);
  Serial.print("C");
  if(Tc<32){
  Serial.println("  warning severe hypothermia");
  lcd.print(" Hypothermia");
   delay(1000);
  return 0;
  }
  else if (Tc<36){
    Serial.println("   warning moderate hypothermia");
    lcd.print(" Hypothermia");
     delay(1000);
    return 0;
    }
  else if (Tc>37.5){
    Serial.println("   warning the baby temprature is above normal");
    lcd.print (" Hyperthermia");
     delay(1000);
    return 0;
    }
    else {
    return 1;
    }   
  delay(1000);
}
// LDR Sensor
bool LDRFun(){
int ldrValue = analogRead(ldrPin); // read the value from the LDR
Serial.println(ldrValue);
 if (ldrValue <= 100) {
return 0;
  }
  else {
return 1;
  }
}
// Humidity Sensor
bool HumidityFun(){
  float h = dht.readHumidity();        // read humidity
  float t = dht.readTemperature();     // read temperature
  if (isnan(h) || isnan(t)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
}
float hic = dht.computeHeatIndex(t, h, false);
Serial.print(F(" Humidity: "));
Serial.println(h);
Serial.print(F("%  Temperature: "));
Serial.println(t);
Serial.print(hic);
Serial.print(F("°C "));
lcd.print(F(" Humidity: "));
lcd.println(h);
lcd.print(F("%  Temperature: "));
lcd.println(t);
lcd.print(hic);
lcd.print(F("°C "));

  if(t >20 )
    { 
      analogWrite(pwm,0);
      delay(1000);
    }
if(h<40){
  Serial.println("   warning low humidity level ");
  lcd.println("   low humidity  ");
  return 0;
  }
else if(h>80){
  Serial.println("   warning high humidity  level ");
   lcd.println("   high humidity  ");
  return 0;
  }
else{
  return 1;
  }
delay(1000);
}

// Sound Sensor
 bool SoundFun(){
  int val_analog = analogRead(sound_analog);

Serial.println(val_analog);

  if (val_analog >20)
  {
    digitalWrite (led, HIGH);
    return 0;
    delay(3000);
    }
  else
  {
    digitalWrite (led, LOW);
    return 1;
    }

  }
