#include "Wire.h"
#include "RTClib.h"
RTC_DS3231 rtc;
float TempC;

                                                            /////////////////Asignar Pines
const int pot = 0;
const int botManAutoValue = 2;
const int botOnOff = 3;
const int led1 = 4;
const int led2 = 5;                                         ////////////led azul
const int led3 = 6;                                         ////////////led blanco
const int fan = 9;
const int sensor lm35 = A0;

int brillo;
int ciclo_led3=0;                                         // ciclo de trabajo WFM salida led
int ciclo_led2=0;
int ciclo_led1=0;
int ciclo_fan=0;
int MinActual=0;
int flag_led=0;                                          //Bandera para activar luces de 0 apagadas, 1 encendidas

if (flag_led==1)
  {
    ciclo_rojo = 254;
    analogWrite(out_rojo,ciclo_rojo);
    ciclo_blanco = 254;
    analogWrite(out_blanco,ciclo_blanco);
    ciclo_azul = 254;
    analogWrite(out_azul,ciclo_azul);
  }
else
{
  ciclo_rojo = 0;
  ciclo_blanco = 0;
  ciclo_azul = 0;
}

void setup() {
  Serial.begin(9600);
  Wire.begin();
  RTC.begin();
  //RTC.adjust(DateTime(__DATE__, __TIME__));
  delay(500);

                                                                /////////////////salidas  y entradas

  pinMode(led1Pin, OUTPUT);
  pinMode(led2Pin, OUTPUT);
  pinMode(led3Pin, OUTPUT);
  pinMode(botManAutoPin, INPUT);
  pinMode(botOnOffPin, INPUT); 
  pinMode(fan, OUTPUT);
}

void loop (){
                                                                  
    //Serial.begin(9600);  
    //delay(3000); // wait for console opening

    if (! rtc.begin()) //No se porque, pero es necesario para pasar la hora a Arduino por el puerto serie
  {
    //Serial.println("Couldn't find RTC");
    while (1);
  }
   
    //USAR UNO DE ESTOS DOS COMANDOS PARA PONER EN HORA EL RTC. LUEGO COMPILAR DE NUEVO ANULANDO LA LINEA
    //rtc.adjust(DateTime(F(__DATE__), F(__TIME__))); //Ajusta la hora a la del PC en el momento de compilar
    //rtc.adjust(DateTime(2014, 1, 21, 9, 0, 55)); // Ajusta la hora a la indicada (año, mes, dia, hora, minuto, segundo)

    
    //Serial.begin(9600);
    Wire.begin();
 
  botManAutoValue = digitalRead(botManAutoPin);
  if (botManAutoValue == HIGH && modoOperacion == 0){
    modoOperacion = 1;
    delay(250);
  }
  else if (botManAutoValue == HIGH && modoOperacion == 1){
    modoOperacion = 0;
    delay(250);
  }
  
  if(modoOperacion == 1){
    ldrValue = analogRead(ldrPin);
    if (ldrValue < 500) {
      digitalWrite(ledPin, HIGH);
      ledStatus = 1;
    }
    
    else {
      digitalWrite(ledPin, LOW);
      ledStatus = 0;
    }
  }
  
  else{
      botOnOffValue = digitalRead(botOnOffPin);
      Serial.print("El valor de la fotorresistencia es: ");
      Serial.println(botOnOffValue, DEC);
      if (botOnOffValue == HIGH && ledStatus == 0){
        digitalWrite(ledPin, HIGH);
        ledStatus = 1;
        delay(250);
      }
      else if (botOnOffValue == HIGH && ledStatus ==1){
        digitalWrite(ledPin, LOW);
        ledStatus = 0;
        delay(250);
  }
}
}

                                                                                          //////////////////---Potenciometro---//////////////////
  
 brillo = analogRead(pot) / 4;    //leemos el valor del pot que es 1023 y lo dividimos
  analogWrite(led, brillo);        //entre 4 para usarlo en el anaolgwrite

                                                                                            //////////////////---Temperatura---//////////////////
  
  TempC = analogRead(Sensor_LM35);
  TempC = 5.0 * TempC * 10.0 / 1024.0;
  Serial.print("Temp_Circuito: ");
  Serial.print(TempC);
  Serial.println(" °C");

                                                                                            /////////////---Sistema de Ventilacion---/////////////
  
  if (TempC >= 16)
    digitalWrite(Fan, HIGH);
  else
    digitalWrite(Fan, LOW);
  if (TempC >= 12)

                                                                                            //////////////INICIA CICLO ILUMINACION 
MinActual = now.hour()*60+now.minute();                                                     ////////////CONVIERTO LAS HORAS A MINUTOS PARA OPERAR MEJOR 
if ((MinActual >= 480) & (MinActual <= 509) & (flag_led==0))                                ////////////////////////// Entre las 8:00 y 8:29 
  {      
      ciclo_led2 = 0;  
      analogWrite(out_led2,ciclo_led2);   
      if (ciclo_led1 != 20)analogWrite(out_led1,ciclo_led1), ciclo_led1++;
      if (ciclo_led3 != 10) analogWrite(out_led3,ciclo_led3), ciclo_led3++;
      delay(100); 
  }
if ((MinActual >= 510) & (MinActual <= 539) & (flag_led==0))                                //////////////////////////////// Entre las 8:30 y 8:59
  {      
      if (ciclo_led1 != 25) analogWrite(out_led1,ciclo_led1), ciclo_led1++;
      if (ciclo_led3 != 25) analogWrite(out_led3,ciclo_led3), ciclo_led3++; 
      if (ciclo_led2 != 10) analogWrite(out_led2,ciclo_led2),ciclo_led2++;      
      delay(100); 
  }
if ((MinActual >= 540) & (MinActual <= 569) & (flag_led==0))                               //////////////////////////////// Entre las 9:00 y 9:29
  {      
      if (ciclo_led1 != 35) analogWrite(out_led1,ciclo_led1), ciclo_led1++;
      if (ciclo_led3 != 35) analogWrite(out_led3,ciclo_led3), ciclo_led3++; 
      if (ciclo_led2 != 35) analogWrite(out_led2,ciclo_led2),ciclo_led2++;      
      delay(100); 
  }
if ((MinActual >= 570) & (MinActual <= 599) & (flag_led==0))                                //////////////////////////////// Entre las 9:30 y 9:59
  {      
      if (ciclo_led1 != 60) analogWrite(out_led1,ciclo_led1), ciclo_led1++;
      if (ciclo_led3 != 60) analogWrite(out_led3,ciclo_led3), ciclo_led3++; 
      if (ciclo_led2 != 60) analogWrite(out_led2,ciclo_led2),ciclo_led2++;      
      delay(100); 
  }
if ((MinActual >= 600) & (MinActual <= 659) & (flag_led==0))                               //////////////////////////////// Entre las 10:00 y 10:59
  {      
      if (ciclo_led1 != 100) analogWrite(out_led1,ciclo_led1), ciclo_led1++;
      if (ciclo_led3 != 100) analogWrite(out_led3,ciclo_led3), ciclo_led3++; 
      if (ciclo_led2 != 100) analogWrite(out_led2,ciclo_led2),ciclo_led2++;      
      delay(100); 
  }
if ((MinActual >= 660) & (MinActual <= 1079) & (flag_led==0))                              //////////////////////////////// Entre las 11:00 y 17:59
  {      
      if (ciclo_led1 != 225) analogWrite(out_led1,ciclo_led1), ciclo_led1++;
      if (ciclo_led3 != 255) analogWrite(out_led3,ciclo_led3), ciclo_led3++; 
      if (ciclo_led2 != 225) analogWrite(out_led2,ciclo_led2),ciclo_led2++;      
      delay(100); 
  }
if ((MinActual >= 1080) & (MinActual <= 1139) & (flag_led==0))                             ////////////////////////////////Entre las 18:00 y 18:59
  {      
      if (ciclo_led1 != 150) analogWrite(out_led1,ciclo_led1), ciclo_led1--;
      if (ciclo_led3 != 150) analogWrite(out_led3,ciclo_led3), ciclo_led3--; 
      if (ciclo_led2 != 150) analogWrite(out_led2,ciclo_led2),ciclo_led2--;      
      delay(100); 
  }
if ((MinActual >= 1140) & (MinActual <= 1169) & (flag_led==0))                              //////////////////////////////// Entre las 19:00 y 19:29
  {      
      if (ciclo_led1 != 125) analogWrite(out_led1,ciclo_led1), ciclo_led1--;
      if (ciclo_led3 != 125) analogWrite(out_led3,ciclo_led3), ciclo_led3--; 
      if (ciclo_led2 != 125) analogWrite(out_led2,ciclo_led2),ciclo_led2--;      
      delay(100); 
  }
if ((MinActual >= 1170) & (MinActual <= 1199) & (flag_led==0))                               //////////////////////////////// Entre las 19:30 Y 19:50
  {      
      if (ciclo_led1 != 110) analogWrite(out_led1,ciclo_led1), ciclo_led1--;
      if (ciclo_bled3 != 110) analogWrite(out_led3,ciclo_led3), ciclo_led3--;
      if (ciclo_led2 != 110) analogWrite(out_led2,ciclo_led2),ciclo_led2--; 
      delay(100); 
  }
if ((MinActual >= 1200) & (MinActual <= 1229) & (flag_led==0))                               //////////////////////////////// Entre las 20:00 y 20:29
  {      
      if (ciclo_led1 != 100) analogWrite(out_led1,ciclo_led1), ciclo_led1--;
      if (ciclo_led3 != 100) analogWrite(out_led3,ciclo_led3), ciclo_led3--;
      if (ciclo_led2 != 100) analogWrite(out_led2,ciclo_led2),ciclo_led2--;     
      delay(100); 
  }
if ((MinActual >= 1230) & (MinActual <= 1259) & (flag_led==0))                              //////////////////////////////// Entre las 20:30 y 20:59
  {      
      if (ciclo_led1 != 100) analogWrite(out_led1,ciclo_led1), ciclo_led1--;
      if (ciclo_led3 != 80) analogWrite(out_led3,ciclo_led3), ciclo_led3--; 
      if (ciclo_led2 != 80) analogWrite(out_led2,ciclo_led2),ciclo_led2--;     
      delay(100); 
  }
if ((MinActual >= 1260) & (MinActual <= 1289) & (flag_led==0))                             //////////////////////////////// Entre las 21:00 Y 21:29
  {      
      if (ciclo_led1 != 90) analogWrite(out_led1,ciclo_led1), ciclo_led1--;
      if (ciclo_led3 !=70 ) analogWrite(out_led3,ciclo_led3), ciclo_led3--;
      if (ciclo_led2 != 70) analogWrite(out_led2,ciclo_led2),ciclo_led2--;     
      delay(100); 
  }
if ((MinActual >= 1290) & (MinActual <= 1319) & (flag_led==0))                             //////////////////////////////// Entre las 21:30 y 21:59
  {      
      if (ciclo_led1 != 70) analogWrite(out_led1,ciclo_led1), ciclo_led1--;
      if (ciclo_led3 != 50) analogWrite(out_led3,ciclo_led3), ciclo_led3--; 
      if (ciclo_led2 != 50) analogWrite(out_led2,ciclo_led2),ciclo_led2--;     
      delay(100); 
  }
if ((MinActual >= 1320) & (MinActual <= 1379) & (flag_led==0))                            //////////////////////////////// Entre las 22:00 y 22:59
  {      
      if (ciclo_led1 != 50) analogWrite(out_led1,ciclo_led1), ciclo_led1--;
      if (ciclo_led3 != 40) analogWrite(out_led3,ciclo_led3), ciclo_led3--;
      if (ciclo_led2 != 40) analogWrite(out_led2,ciclo_led),ciclo_led2--;    
      delay(100); 
  }
if ((MinActual >= 1380) & (MinActual <= 1439) & (flag_led==0))                            //////////////////////////////// Entre las 23:00 y 00:00
  {      
      if (ciclo_led1 != 25) analogWrite(out_led1,ciclo_led1), ciclo_led1--;
      if (ciclo_led3 != 25) analogWrite(out_led3,ciclo_led3), ciclo_led3--;
      if (ciclo_led2 != 30) analogWrite(out_led2,ciclo_led2),ciclo_led2--;    
      delay(100); 
  }
if ((MinActual >= 0) & (MinActual <= 60) & (flag_led==0))                                 //////////////////////////////// Entre las 00:01 y 01:00
  {      
      if (ciclo_led1 != 0) analogWrite(out_led1,ciclo_led1), ciclo_led1--;
      if (ciclo_led3 != 0) analogWrite(out_led,ciclo_led3), ciclo_led3--;
      if (ciclo_led2 != 25) analogWrite(out_led2,ciclo_led2),ciclo_led2--;  
      delay(100); 
  }
if ((MinActual >= 61) & (MinActual <= 479) & (flag_led==0))                               //////////////////////////////// Entre las 01:01 y 07:59
  {      
      //if (ciclo_led1 != 0) analogWrite(out_led1,ciclo_led1), ciclo_led1--;
      // if (ciclo_led3 != 0) analogWrite(out_led3,ciclo_led3), ciclo_led3--;
      ciclo_led1 = 0;  
      analogWrite(out_led1,ciclo_led1); 
      ciclo_led3 = 0;  
      analogWrite(out_led3,ciclo_led3);   
      if (ciclo_led2 != 10) analogWrite(out_led2,ciclo_led2),ciclo_led2--;  
      delay(100); 
  } 
delay(50);
}
