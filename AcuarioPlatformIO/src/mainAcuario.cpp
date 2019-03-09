#include <Arduino.h>
#include "Wire.h"
#include "RTClib.h"
RTC_DS1307 rtc;
DateTime now;

const int botManAuto = 2;

const int ledRojo = 3; //Pines PWM del arduino UNO
const int ledAzul = 5;
const int ledBlanco = 6;
const int ledVioleta = 9;

int ciclo_ledBlanco=0;
int ciclo_ledAzul=0;
int ciclo_ledRojo=0;
int ciclo_ledVioleta=0;
int MinActual=0;
int dimeo_led=0;            // activar luces de 0 apagadas, 1 encendidas

void setup() {
  Serial.begin(9600);
  Wire.begin();
  rtc.begin();
  //USAR UNO DE ESTOS DOS COMANDOS PARA PONER EN HORA EL RTC. LUEGO COMPILAR DE NUEVO ANULANDO LA LINEA
  //rtc.adjust(DateTime(F(__DATE__), F(__TIME__))); //Ajusta la hora a la del PC en el momento de compilar
  //rtc.adjust(DateTime(2019, 2, 16, 12, 0, 55)); // Ajusta la hora a la indicada (año, mes, dia, hora, minuto, segundo)
  delay(500);

  pinMode(ledRojo, OUTPUT); /////////////////salidas  y entradas
  pinMode(ledAzul, OUTPUT);
  pinMode(ledBlanco, OUTPUT);
  pinMode(ledVioleta, OUTPUT);
  pinMode(botManAuto, INPUT);

  Serial.println("Luces Iniciadas.");
}

void loop (){

  if (! rtc.begin())              //comprobamos si tenemos conectado el rtc en Arduino
  {
    Serial.println("Couldn't find RTC");
    while (1);
  }
                                                                                            //////////////INICIA CICLO ILUMINACION
//MinActual = now.hour()*60+now.minute();                                                   ////////////CONVIERTO LAS HORAS A MINUTOS PARA OPERAR MEJOR
MinActual=millis()/100;                                                     
if ((MinActual >= 480) && (MinActual <= 509) && (dimeo_led==0))                                ////////////////////////// Entre las 8:00 y 8:29
  {
      ciclo_ledAzul = 0;
      analogWrite(ledAzul,ciclo_ledAzul);
      if (ciclo_ledRojo != 20)analogWrite(ledRojo,ciclo_ledRojo), ciclo_ledRojo++;
      if (ciclo_ledBlanco != 10) analogWrite(ledBlanco,ciclo_ledBlanco), ciclo_ledBlanco++;
      delay(100);
  }
else if ((MinActual >= 510) && (MinActual <= 539) && (dimeo_led==0))                                //////////////////////////////// Entre las 8:30 y 8:59
  {
      if (ciclo_ledRojo != 25) analogWrite(ledRojo,ciclo_ledRojo), ciclo_ledRojo++;
      if (ciclo_ledBlanco != 25) analogWrite(ledBlanco,ciclo_ledBlanco), ciclo_ledBlanco++;
      if (ciclo_ledAzul != 10) analogWrite(ledAzul,ciclo_ledAzul),ciclo_ledAzul++;
      delay(100);
  }
else if ((MinActual >= 540) && (MinActual <= 569) && (dimeo_led==0))                               //////////////////////////////// Entre las 9:00 y 9:29
  {
      if (ciclo_ledRojo != 35) analogWrite(ledRojo,ciclo_ledRojo), ciclo_ledRojo++;
      if (ciclo_ledBlanco != 35) analogWrite(ledBlanco,ciclo_ledBlanco), ciclo_ledBlanco++;
      if (ciclo_ledAzul != 35) analogWrite(ledAzul,ciclo_ledAzul),ciclo_ledAzul++;
      delay(100);
  }
else if ((MinActual >= 570) && (MinActual <= 599) && (dimeo_led==0))                                //////////////////////////////// Entre las 9:30 y 9:59
  {
      if (ciclo_ledRojo != 60) analogWrite(ledRojo,ciclo_ledRojo), ciclo_ledRojo++;
      if (ciclo_ledBlanco != 60) analogWrite(ledBlanco,ciclo_ledBlanco), ciclo_ledBlanco++;
      if (ciclo_ledAzul != 60) analogWrite(ledAzul,ciclo_ledAzul),ciclo_ledAzul++;
      delay(100);
  }
else if ((MinActual >= 600) && (MinActual <= 659) && (dimeo_led==0))                               //////////////////////////////// Entre las 10:00 y 10:59
  {
      if (ciclo_ledRojo != 100) analogWrite(ledRojo,ciclo_ledRojo), ciclo_ledRojo++;
      if (ciclo_ledBlanco != 100) analogWrite(ledBlanco,ciclo_ledBlanco), ciclo_ledBlanco++;
      if (ciclo_ledAzul != 100) analogWrite(ledAzul,ciclo_ledAzul),ciclo_ledAzul++;
      delay(100);
  }
else if ((MinActual >= 660) && (MinActual <= 1079) && (dimeo_led==0))                              //////////////////////////////// Entre las 11:00 y 17:59
  {
      if (ciclo_ledRojo != 225) analogWrite(ledRojo,ciclo_ledRojo), ciclo_ledRojo++;
      if (ciclo_ledBlanco != 255) analogWrite(ledBlanco,ciclo_ledBlanco), ciclo_ledBlanco++;
      if (ciclo_ledAzul != 225) analogWrite(ledAzul,ciclo_ledAzul),ciclo_ledAzul++;
      delay(100);
  }
else if ((MinActual >= 1080) && (MinActual <= 1139) && (dimeo_led==0))                             ////////////////////////////////Entre las 18:00 y 18:59
  {
      if (ciclo_ledRojo != 150) analogWrite(ledRojo,ciclo_ledRojo), ciclo_ledRojo--;
      if (ciclo_ledBlanco != 150) analogWrite(ledBlanco,ciclo_ledBlanco), ciclo_ledBlanco--;
      if (ciclo_ledAzul != 150) analogWrite(ledAzul,ciclo_ledAzul),ciclo_ledAzul--;
      delay(100);
  }
else if ((MinActual >= 1140) && (MinActual <= 1169) && (dimeo_led==0))                              //////////////////////////////// Entre las 19:00 y 19:29
  {
      if (ciclo_ledRojo != 125) analogWrite(ledRojo,ciclo_ledRojo), ciclo_ledRojo--;
      if (ciclo_ledBlanco != 125) analogWrite(ledBlanco,ciclo_ledBlanco), ciclo_ledBlanco--;
      if (ciclo_ledAzul != 125) analogWrite(ledAzul,ciclo_ledAzul),ciclo_ledAzul--;
      delay(100);
  }
else if ((MinActual >= 1170) && (MinActual <= 1199) && (dimeo_led==0))                               //////////////////////////////// Entre las 19:30 Y 19:50
  {
      if (ciclo_ledRojo != 110) analogWrite(ledRojo,ciclo_ledRojo), ciclo_ledRojo--;
      if (ciclo_ledBlanco != 110) analogWrite(ledBlanco,ciclo_ledBlanco), ciclo_ledBlanco--;
      if (ciclo_ledAzul != 110) analogWrite(ledAzul,ciclo_ledAzul),ciclo_ledAzul--;
      delay(100);
  }
else if ((MinActual >= 1200) && (MinActual <= 1229) && (dimeo_led==0))                               //////////////////////////////// Entre las 20:00 y 20:29
  {
      if (ciclo_ledRojo != 100) analogWrite(ledRojo,ciclo_ledRojo), ciclo_ledRojo--;
      if (ciclo_ledBlanco != 100) analogWrite(ledBlanco,ciclo_ledBlanco), ciclo_ledBlanco--;
      if (ciclo_ledAzul != 100) analogWrite(ledAzul,ciclo_ledAzul),ciclo_ledAzul--;
      delay(100);
  }
else if ((MinActual >= 1230) && (MinActual <= 1259) && (dimeo_led==0))                              //////////////////////////////// Entre las 20:30 y 20:59
  {
      if (ciclo_ledRojo != 100) analogWrite(ledRojo,ciclo_ledRojo), ciclo_ledRojo--;
      if (ciclo_ledBlanco != 80) analogWrite(ledBlanco,ciclo_ledBlanco), ciclo_ledBlanco--;
      if (ciclo_ledAzul != 80) analogWrite(ledAzul,ciclo_ledAzul),ciclo_ledAzul--;
      delay(100);
  }
else if ((MinActual >= 1260) && (MinActual <= 1289) && (dimeo_led==0))                             //////////////////////////////// Entre las 21:00 Y 21:29
  {
      if (ciclo_ledRojo != 90) analogWrite(ledRojo,ciclo_ledRojo), ciclo_ledRojo--;
      if (ciclo_ledBlanco !=70 ) analogWrite(ledBlanco,ciclo_ledBlanco), ciclo_ledBlanco--;
      if (ciclo_ledAzul != 70) analogWrite(ledAzul,ciclo_ledAzul),ciclo_ledAzul--;
      delay(100);
  }
else if ((MinActual >= 1290) && (MinActual <= 1319) && (dimeo_led==0))                             //////////////////////////////// Entre las 21:30 y 21:59
  {
      if (ciclo_ledRojo != 70) analogWrite(ledRojo,ciclo_ledRojo), ciclo_ledRojo--;
      if (ciclo_ledBlanco != 50) analogWrite(ledBlanco,ciclo_ledBlanco), ciclo_ledBlanco--;
      if (ciclo_ledAzul != 50) analogWrite(ledAzul,ciclo_ledAzul),ciclo_ledAzul--;
      delay(100);
  }
else if ((MinActual >= 1320) && (MinActual <= 1379) && (dimeo_led==0))                            //////////////////////////////// Entre las 22:00 y 22:59
  {
      if (ciclo_ledRojo != 50) analogWrite(ledRojo,ciclo_ledRojo), ciclo_ledRojo--;
      if (ciclo_ledBlanco != 40) analogWrite(ledBlanco,ciclo_ledBlanco), ciclo_ledBlanco--;
      if (ciclo_ledAzul != 40) analogWrite(ledAzul,ciclo_ledAzul),ciclo_ledAzul--;
      delay(100);
  }
else if ((MinActual >= 1380) && (MinActual <= 1439) && (dimeo_led==0))                            //////////////////////////////// Entre las 23:00 y 00:00
  {
      if (ciclo_ledRojo != 25) analogWrite(ledRojo,ciclo_ledRojo), ciclo_ledRojo--;
      if (ciclo_ledBlanco != 25) analogWrite(ledBlanco,ciclo_ledBlanco), ciclo_ledBlanco--;
      if (ciclo_ledAzul != 30) analogWrite(ledAzul,ciclo_ledAzul),ciclo_ledAzul--;
      delay(100);
  }
else if ((MinActual >= 0) && (MinActual <= 60) && (dimeo_led==0))                                 //////////////////////////////// Entre las 00:01 y 01:00
  {
      if (ciclo_ledRojo != 0) analogWrite(ledRojo,ciclo_ledRojo), ciclo_ledRojo--;
      if (ciclo_ledBlanco != 0) analogWrite(ledBlanco,ciclo_ledBlanco), ciclo_ledBlanco--;
      if (ciclo_ledAzul != 25) analogWrite(ledAzul,ciclo_ledAzul),ciclo_ledAzul--;
      delay(100);
  }
else if ((MinActual >= 61) && (MinActual <= 479) && (dimeo_led==0))                               //////////////////////////////// Entre las 01:01 y 07:59
  {
      //if (ciclo_ledRojo != 0) analogWrite(ledRojo,ciclo_ledRojo), ciclo_ledRojo--;
      // if (ciclo_ledBlanco != 0) analogWrite(ledBlanco,ciclo_ledBlanco), ciclo_ledBlanco--;
      ciclo_ledRojo = 0;
      analogWrite(ledRojo,ciclo_ledRojo);
      ciclo_ledBlanco = 0;
      analogWrite(ledBlanco,ciclo_ledBlanco);
      if (ciclo_ledAzul != 10) analogWrite(ledAzul,ciclo_ledAzul),ciclo_ledAzul--;
      delay(100);
  }
//delay(50);
}
