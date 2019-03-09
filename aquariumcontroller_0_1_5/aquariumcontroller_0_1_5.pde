// aquarium controller 0.1α
//
// Analog Pin 0 = pt100 temperature in aquarium
// Analog Pin 1 = temperature in room
// Analog Pin 2 = pH
// Analog Pin 3 = lightsensitive resistor
// Analog Pin 4 = SDA
// Analog Pin 5 = SCL
//
// Digital Pin 0 = RX ?
// Digital Pin 1 = TX ?
// Digital Pin 2 = pH + led 230V or 24V coil +led
// Digital Pin 3 = relay 1 heater 230V + led PWM
// Digital Pin 4 = mosfet 2 12V for moonlight + led
// Digital Pin 5 = relay 2 daylight 230V + led PWM
// Digital Pin 6 = mosfet 1 heatingcable 24V PWM
// Digital Pin 7 = automatic feeder fhaulhaber ?
// Digital Pin 8 = input feeding mode + led  ?
// Digital Pin 9 = mosfet 3 for cooling fan? PWM
// Digital Pin 10 = ? PWM
// Digital Pin 11 = ? PWM
// Digital Pin 12 = input water change mode + led ?
// Digital Pin 13 = filterpump ?
//#include <Wire.h>
//#include <DS1307.h>
 int tempa = A0;               // input for temperature in aquarium 0-5 v 0-50 °C
 int tempaValue = 0.00;        // variable for temperature in aquarium 0-5 v 0-50 °C
 int tempr = A1;               // input for temperature in room 0-5 v 0-50 °C
 int temprValue = 0.0;         // variabel for temperature in room 0-5 v 0-50 °C 
 int ph = A2;                  // input for ph in aquarium 0-5 v
 int phValue = 0.00;           // variable for ph in aquarium 0-5 v
 int foto = A3;                // input for lightint i aquarium 0-5 v
 int fotoValue = 0.0;          // variable for light in the aquarium 0-5 v 
 int heatingcable = 6;         // output for heatingcable
 int heater = 3;               // output for heater
 
void setup() 
{
 // setup
 Serial.begin(9600);           // set up Serial library at 9600 bps
 pinMode(heatingcable,   OUTPUT);
 pinMode(heater,         OUTPUT);
}

void loop() 
{
   // ********** read temperature in aquarium ******************
  Serial.println("Aquarium controller 0.1α");
  tempaValue = analogRead(tempa);                        // 0-50 °C 0 to 1023 50/1024 resolution 0.048828125°C
  int temp = tempaValue * 0.048828125;                   // example input 2.5V/0512*048828125=25°C
  Serial.println("Measured tempaValue"); 
  Serial.println(tempaValue);                            // print out temp in binary form
  Serial.println("temp in °C");
  Serial.println(temp);                                  // print out temp in °C
  
  // *********** turn heatingcable on or off *******************
  if (temp <= 24.9)digitalWrite(heatingcable, HIGH);
  if (temp >= 25.2)digitalWrite(heatingcable, LOW);
  Serial.println("status heatingcable");
  Serial.println(heatingcable);                         // print out state of heatingcable
  
  // *********** turn heater on or off *************************
  if (temp <= 24.7)digitalWrite(heater, HIGH);
  if (temp >= 25.0)digitalWrite(heater, LOW);
  Serial.println("status heater");
  Serial.println(heater);                               // print out state of heater
  delay (10000);  // pause for 10 seconds to avoid blocking serial com
  }
