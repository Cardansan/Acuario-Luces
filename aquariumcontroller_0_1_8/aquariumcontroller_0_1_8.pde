/* 
aquarium controller 0.1α

Analog Pin 0 = pt100 temperature in aquarium
Analog Pin 1 = temperature in room
Analog Pin 2 = pH
Analog Pin 3 = lightsensitive resistor
Analog Pin 4 = SDA
Analog Pin 5 = SCL

Digital Pin 0 = RX
Digital Pin 1 = TX
Digital Pin 2 = pH 230V or 24V coil +led
Digital Pin 3 = relay 1 heater 230V + led PWM
Digital Pin 4 = mosfet 2 12V for moonlight + led
Digital Pin 5 = relay 2 daylight 230V + led PWM
Digital Pin 6 = mosfet 1 heatingcable 24V PWM
Digital Pin 7 = automatic feeder fhaulhaber ?
Digital Pin 8 = input feeding mode + led  ?
Digital Pin 9 = mosfet 3 for cooling fan? PWM
Digital Pin 10 = ? PWM
Digital Pin 11 = ? PWM
Digital Pin 12 = input water change mode + led ?
Digital Pin 13 = program run led

todo:
* pH

changelog:
0.1.6 reomved 
const int heatingcable = 6;   // output for heatingcable
const int heater = 3;         // output for heater
0.1.7
added high/low temp in aquarium
corrected reversed function of coolingfan
0.1.8
added lcd
added lights
added rtc
added min/max pH
cleanup

*/
#include <Wire.h>
#include <ByVacLCD.h>
#include <DS1307.h>
 int tempa = A0;         // input for temperature in aquarium 0-5 v 0-50 °C
 int tempaValue = 0.00;  // variable for temperature in aquarium 0-5 v 0-50 °C
 int tempr = A1;         // input for temperature in room 0-5 v 0-50 °C
 int temprValue = 0.0;   // variabel for temperature in room 0-5 v 0-50 °C 
 int pH = A2;            // input for ph in aquarium 0-5 v
 int pHValue = 0.00;     // variable for ph in aquarium 0-5 v
 int foto = A3;          // input for lightint i aquarium 0-5 v
 int fotoValue = 0.0;    // variable for light in the aquarium 0-5 v
 int statusHeatingcable = 0;
 int statusHeater = 0; 
 int statusFan = 0;
 int statuspHsolenoid;
 int tempMin = 50;        // minimum temp value
 int tempMax = 0;         // maximum temp value
 int pHMin = 14;          // minimum temp value
 int pHMax = 0;           // maximum temp value
 int time = 1000;
 int dayLight =0;
 int year = 2011;
 int month = 01;
 int date = 01;
 int day =01;
 int hour = 00;
 int minute = 00;
 int second = 01;

 // ***** LCD *****
 int keypushed;
 int keymap;
 int addr;
 ByVacLCD lcd = ByVacLCD(0x42,2,16); // 0x42= i2c adress 2=rows 16=coloms
 //ByVacLCD lcd = ByVacLCD(0x21,4,20);

void setup() 
{
 // setup
 Serial.begin(9600);                                     // set up Serial library at 9600 bps
 lcd.init();
 pinMode(6,  OUTPUT); // heatingcable
 pinMode(3,  OUTPUT); // heater
 pinMode(13, OUTPUT); // program run led
 pinMode(9,  OUTPUT); // cooling fan
 pinMode(5,  OUTPUT); // daylights 
 pinMode(4,  OUTPUT); // moonlights 
 }

void loop() 
{
  // ***** read rtc *****
  year = RTC.get(DS1307_YR,false);
  month = RTC.get(DS1307_MTH,false);
  date = RTC.get(DS1307_DATE,false);
  hour = RTC.get(DS1307_HR,true);                      //This is in military time  [0,23]
  minute = RTC.get(DS1307_MIN,false);
  second = RTC.get(DS1307_SEC,false);
  time = (hour * 100) + minute;                        //create military time output [0000,2400)
  Serial.println("rtc"); 
  Serial.println(date && "." && month && "." && year); // print out rtc date and year 29.12.2011
  
  // ***** read temperature in aquarium *****
  Serial.println("Aquarium controller 0.1 alfa");
  tempaValue = analogRead(tempa);                     // 0-50 °C 0 to 1023 50/1024 resolution 0.048828125°C
  int temp = tempaValue * 0.048828125;                // example input 2.5V/0512*048828125=25°C
  Serial.println("Measured tempaValue binary"); 
  Serial.println(tempaValue);                         // print out temp in binary form
  Serial.println("temp in celsius");
  Serial.println(temp);                               // print out temp in °C
  
  // ***** record the maximum temp value *****
  if (temp > tempMax) tempMax = temp;
  Serial.println("max temperature in aquarium");
  Serial.println(tempMax); 
  
  // ***** record the minimum temp value *****
  if (temp < tempMin) tempMin = temp;
  Serial.println("minimum temperature in aquarium");
  Serial.println(tempMin);
  
  // ***** turn heatingcable on or off *****
  if (temp <= 24.9)digitalWrite(6, HIGH);
  if (temp >= 25.2)digitalWrite(6, LOW);
  statusHeatingcable = digitalRead(6);
  Serial.println("status heatingcable");
  Serial.println(statusHeatingcable);                // print out state of heatingcable
  
  // ***** turn heater on or off *****
  if (temp <= 24.7)digitalWrite(3, HIGH);
  if (temp >= 25.0)digitalWrite(3, LOW);
  statusHeater = digitalRead(3);
  Serial.println("status heater");
  Serial.println(statusHeater);                     // print out state of heater
  
  // ***** turn Cooling fan on or off *****
  if (temp >= 29)digitalWrite(9, HIGH);
  if (temp <= 28)digitalWrite(9, LOW);
  statusFan = digitalRead(9);
  Serial.println("status Fan");
  Serial.println(statusFan);                       // print out state of fan
  
  // ***** pH *****
  pHValue = analogRead(pH);                        // 0-14 pH °C 0 to 1023 15/1024 resolution 0.0146484375
  int pH = pHValue * 0;                            // x = pH factor
  Serial.println("Measured pHValue binary"); 
  Serial.println(pHValue);                         // print out pH in binary form
  Serial.println("Measured pHValue");
  Serial.println(pHValue);                         // print out pH
  
  // ***** record the maximum pH value *****
  if (pH > pHMax) pHMax = pH;
  Serial.println("max pH in aquarium");
  Serial.println(pHMax); 
  
  // ***** record the minimum pH value *****
  if (pH < pHMin) pHMin = pH;
  Serial.println("minimum pH in aquarium");
  Serial.println(pHMin);
  
  // *****turn pH solenoid on/off *****
  if (pH <= 6.8 || dayLight <1)digitalWrite(2, LOW);
  if (pH >= 7.2 && dayLight ==1)digitalWrite(2, HIGH);  // lights on required
  statuspHsolenoid = digitalRead(2);
  Serial.println("status pHsolenoid");
  Serial.println(statuspHsolenoid);                     // print out state of pHsolenoid
  
  // ***** LCD *****
  lcd.clear();
  lcd.print("Hello");
  lcd.setCursor(1,0);
  lcd.print("set cursor");
  lcd.setCursor(2,0);
  lcd.print("Key Pushed: ");
  lcd.print(lcd.get_key());
  delay(500);
  
  // ***** Day Lights *****
  if (time > 1100) digitalWrite(5, HIGH);
  if (time > 2300) digitalWrite(5, LOW);
  int dayLight = digitalRead(5);
  // ***** Moon Lights *****
  if (time > 1000) digitalWrite(4, HIGH);
  if (time > 1115) digitalWrite(4, LOW);
  if (time > 2245) digitalWrite(4, HIGH);
  if (time > 0100 && time < 1000)  digitalWrite(4, LOW);
    
  // *********** program run led and pause for 10 seconds to avoid blocking serial com *************************
  // *********** 1 long 5 short blinks *************************************************************************
  
  digitalWrite(13, HIGH);   // set the LED on
  delay(2500);              // wait for 5 second
  digitalWrite(13, LOW);    // set the LED off
  delay(500);               
  digitalWrite(13, HIGH);   // set the LED on
  delay(500);
  digitalWrite(13, LOW);    // set the LED off
  delay(500);               
  digitalWrite(13, HIGH);   // set the LED on
  delay(500); 
  digitalWrite(13, LOW);    // set the LED off
  delay(500);               
  digitalWrite(13, HIGH);   // set the LED on
  delay(500);
  digitalWrite(13, LOW);    // set the LED off
  delay(500);               
  digitalWrite(13, HIGH);   // set the LED on
  delay(500);   
  digitalWrite(13, LOW);    // set the LED off
  delay(500);               
  digitalWrite(13, HIGH);   // set the LED on
  delay(500);   
  digitalWrite(13, LOW);    // set the LED off
  delay(2500);              // wait for 5 second
  
  }
