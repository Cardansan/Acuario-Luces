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
* rtc
* lights
* pH

changelog:
0.1.6 reomved 
const int heatingcable = 6;   // output for heatingcable
const int heater = 3;         // output for heater
0.1.7
added high/low temp in aquarium
corrected reversed function of coolingfan
0.1.8

*/
#include <Wire.h>
#include <ByVacLCD.h>
#include <DS1307.h>
 int tempa = A0;         // input for temperature in aquarium 0-5 v 0-50 °C
 int tempaValue = 0.00;        // variable for temperature in aquarium 0-5 v 0-50 °C
 int tempr = A1;         // input for temperature in room 0-5 v 0-50 °C
 int temprValue = 0.0;         // variabel for temperature in room 0-5 v 0-50 °C 
 int pH = A2;            // input for ph in aquarium 0-5 v
 int pHValue = 0.00;           // variable for ph in aquarium 0-5 v
 int foto = A3;          // input for lightint i aquarium 0-5 v
 int fotoValue = 0.0;          // variable for light in the aquarium 0-5 v
 int statusHeatingcable = 0;
 int statusHeater = 0; 
 int statusFan = 0;
 int statuspHsolenoid;
 int tempMin = 50;        // minimum temp value
 int tempMax = 0;         // maximum temp value

/* 
void readClock() {
  clock.read();
  
  Serial.print("(clock= ");
  if (clock.hour_ < 10) Serial.print('0');
  Serial.print((int)clock.hour_);
  if (clock.minute_ < 10) Serial.print('0');
  Serial.print((int)clock.minute_);
  if (clock.second_ < 10) Serial.print('0');
  Serial.print((int)clock.second_);
  Serial.println(")");
} 
*/
void setup() 
{
 // setup
 Serial.begin(9600);                                     // set up Serial library at 9600 bps
 pinMode(6,  OUTPUT); // heatingcable
 pinMode(3,  OUTPUT); // heater
 pinMode(13, OUTPUT); // program run led
 pinMode(9,  OUTPUT); // cooling fan
 }

void loop() 
{
  // ********** read temperature in aquarium ******************
  Serial.println("Aquarium controller 0.1 alfa");
  tempaValue = analogRead(tempa);                        // 0-50 °C 0 to 1023 50/1024 resolution 0.048828125°C
  int temp = tempaValue * 0.048828125;                   // example input 2.5V/0512*048828125=25°C
  Serial.println("Measured tempaValue"); 
  Serial.println(tempaValue);                            // print out temp in binary form
  Serial.println("temp in celsius");
  Serial.println(temp);                                  // print out temp in °C
  
  // ***** record the maximum temp value *****
  if (temp > tempMax) tempMax = temp;
  Serial.println("max temperature in aquarium");
  Serial.println(tempMax); 
  
  // ***** record the minimum temp value *****
  if (temp < tempMin) tempMin = temp;
  Serial.println("minimum temperature in aquarium");
  Serial.println(tempMin);
  
  // *********** turn heatingcable on or off *******************
  if (temp <= 24.9)digitalWrite(6, HIGH);
  if (temp >= 25.2)digitalWrite(6, LOW);
  statusHeatingcable = digitalRead(6);
  Serial.println("status heatingcable");
  Serial.println(statusHeatingcable);                      // print out state of heatingcable
  
  // *********** turn heater on or off *************************
  if (temp <= 24.7)digitalWrite(3, HIGH);
  if (temp >= 25.0)digitalWrite(3, LOW);
  statusHeater = digitalRead(3);
  Serial.println("status heater");
  Serial.println(statusHeater);                            // print out state of heater
  
  // *********** turn Cooling fan on or off *************************
  if (temp >= 29)digitalWrite(9, HIGH);
  if (temp <= 28)digitalWrite(9, LOW);
  
  statusFan = digitalRead(9);
  Serial.println("status Fan");
  Serial.println(statusFan);                              // print out state of fan
  
  // ***** pH *****
  pHValue = analogRead(pH);                        // 0-14 pH °C 0 to 1023 15/1024 resolution 0.0146484375
  int pH = pHValue * 0;                            // x = pH factor
  Serial.println("Measured pHValue binary"); 
  Serial.println(pHValue);                         // print out pH in binary form
  Serial.println("Measured pHValue");
  Serial.println(pHValue);                         // print out pH
  
  //*****turn pH solenoid on/off *****
  if (pH <= 6.8)digitalWrite(2, LOW);
  if (pH >= 7.2)digitalWrite(2, HIGH);
  statuspHsolenoid = digitalRead(2);
  Serial.println("status pHsolenoid");
  Serial.println(statuspHsolenoid);                       // print out state of pHsolenoid
  
  
  /*
  hour = RTC.get(DS1307_HR,true);  //This is in military time  [0,23]
  minute = RTC.get(DS1307_MIN,false);
  second = RTC.get(DS1307_SEC,false);
  date = RTC.get(DS1307_DATE,false);
  month = RTC.get(DS1307_MTH,false);
  year = RTC.get(DS1307_YR,false);
  time = (hour * 100) + minute;  //create military time output [0000,2400)
  
  int lights_on_time_1 = -100;  //Turn day lights on at this time (military time)
  int lights_off_time_1 = -100; //Turn day lights off at this time
  int lights_on_time_2 = 1200;  //Turn day lights on at this time (military time)
  int lights_off_time_2 = 2000; //Turn day lights off at this time

  int wait_time = 0; //how long to wait before turning on lights if they are turned off due to power loss or overheating
  int lights_out = -100;  //placeholder  --don't change

  int moonl_on_time = 2100;  //Turn on moon lights at this time
  int moonl_off_time = 700;  //Turn off moon lights at this time
  
  //****************Day Lights****************************************************************************
  
  
  //  Two On/off cycles no waiting to turn the light on, ballast has internal switch
  
     if((lights_on_time_1 < mil_time && lights_off_time_1 >= mil_time && lights_off_temp > Tc_100) ||  (lights_on_time_2 < mil_time && lights_off_time_2 >= mil_time && lights_off_temp > Tc_100) || (alarm_low_temp > Tc_100)) {

     digitalWrite(day_light, HIGH);
   }
   else{
     digitalWrite(day_light, LOW);
   }
  
  //****************Moon Lights*******************************************************************************

  if( ((moonl_off_time < moonl_on_time) && (moonl_on_time <= mil_time || moonl_off_time > mil_time)) || (moonl_on_time <= mil_time && moonl_off_time > mil_time)  ) {
    digitalWrite(moon_light, HIGH);
  }
  else{
    digitalWrite(moon_light, LOW);
  }
   
     //****************Fan****************************************************************************************
   if(Tc_100 > fan_on_temp){     // turn fan on if temp is above fan_on_temp
    digitalWrite(fan, HIGH);
  }  
  if(Tc_100 < fan_off_temp){    //turn fan off if temp is below fan_off_temp
   digitalWrite(fan, LOW);
  }    
  */
  
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
