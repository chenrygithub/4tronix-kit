#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

int Temp, Light, Humidity, hCheck;
boolean Tilt;

#define tempPin 0
#define lightPin 1
#define tiltPin 6
#define humidityPin 7

#include <dht11.h>

dht11 DHT11;

// initialise LCD library and pins
void setup() {
  lcd.begin(16, 2);
  pinMode(tiltPin, INPUT);
}

void loop()
{
  Temp = (5.0 * analogRead(tempPin) * 100.0) / 1024;
  delay(10);
  Light = toLux(analogRead(lightPin));
  hCheck = DHT11.read(humidityPin);
  if(hCheck != 0)
    Humidity = 255; //Must be an error
  else
    Humidity = DHT11.humidity;
  Tilt = (digitalRead(tiltPin) == LOW);
  
  showData(Temp, Light, Humidity, Tilt);
  delay(1000);
}

// Change the ADC reading to Lux. Assumes a 10K pull down resistor and 5V
int toLux(int adc)
{
  if (adc > 868)
    return 300 + 5.6 * (adc - 868);
  else if (adc > 853)
    return 250 + 3.46 * (adc - 853);
  else if (adc > 788)
    return 150 + 1.52 * (adc - 788);
  else if (adc > 731)
    return 120 + 0.53 * (adc - 731);
  else if (adc > 683)
    return 80 + 0.82 * (adc - 683);
  else if (adc > 256)
    return 10 + 0.16 * (adc - 256);
  else
    return 0.04 * adc;
}

void showData (int temp, int light, int humidity, boolean tilt)
{
  String s1, s2, s3;
  String spaces = "              ";
  s1 = String(temp) + char(0xdf) + "C";
  s2 = String (light) + "Lux";
  s3 = s1 + spaces.substring(0, 16 - s1.length() - s2.length()) + s2;
  lcd.setCursor(0,0);
  lcd.print(s3);
  
  if(humidity == 255)
    s1 = "ERROR";
  else
    s1 = String(humidity) + "%";
  if (tilt)
    s2 = "TILT";
  else
    s2 = "FLAT";
  s3 = s1 + spaces.substring(0, 16 - s1.length() - s2.length()) + s2;
  lcd.setCursor(0,1);
  lcd.print(s3);
}
