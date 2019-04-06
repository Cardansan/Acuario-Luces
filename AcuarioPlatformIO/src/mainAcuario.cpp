#include <Arduino.h>
#include "Wire.h"
#include "RTClib.h"
RTC_DS1307 rtc;

DateTime now; //Para registrar la hora actual
int MinActual=0;

const int pinModo = A0; //Pin del modo manual o automático
int modo; //Variable del modo actual

const int ledRojo = 3; //Pines PWM del arduino UNO
const int ledAzul = 5;
const int ledBlanco = 6;
const int ledVioleta = 9;

int ciclo_ledBlanco; //Variables del ciclo de cada color
int ciclo_ledAzul;
int ciclo_ledRojo;
int ciclo_ledVioleta;

long unsigned int segundo; //Variables para esperar un segundo entre mensajes del puerto Serial;
long unsigned int prevSegundo;

void setup() {
  pinMode(ledRojo, OUTPUT); /////////////////salidas  y entradas
  pinMode(ledAzul, OUTPUT);
  pinMode(ledBlanco, OUTPUT);
  pinMode(ledVioleta, OUTPUT);
  pinMode(pinModo, INPUT); //Entrada del Potenciometro

  pinMode(A3, OUTPUT); //crea un falso VCC y GND para el rtc
  digitalWrite(A3,HIGH);
  pinMode(A2, OUTPUT);
  digitalWrite(A2,LOW);

  Serial.begin(57600);
  delay(200);
  Wire.begin();
  rtc.begin();
  //USAR UNO DE ESTOS DOS COMANDOS PARA PONER EN HORA EL RTC. LUEGO COMPILAR DE NUEVO ANULANDO LA LINEA
  rtc.adjust(DateTime(F(__DATE__), F(__TIME__))); //Ajusta la hora a la del PC en el momento de compilar
  //rtc.adjust(DateTime(2019, 2, 16, 12, 0, 55)); // Ajusta la hora a la indicada (año, mes, dia, hora, minuto, segundo)
  delay(500);
  Serial.println("Luces Iniciadas.");
}

void loop (){

  if (!rtc.begin())              //comprobamos si tenemos conectado el rtc en Arduino
  {
    Serial.println("Couldn't find RTC");
  }

modo=analogRead(pinModo); //Lee el potenciómetro
if(modo==0){ //MODO AUTOMÁTICO SI EL POTENCIÓMETRO ESTÁ EN 0

   now=rtc.now(); //Actualiza la info del rtc
   MinActual = now.hour()*60 + now.minute(); //Convierto las horas a minutos para hacer más fácil las cuentas
}
else { //MODO MANUAL SI EL POTENCIÓMETRO NO ESTÁ EN 0
  MinActual=map(modo,1,1023,0,1439);
}


 //////////////INICIA CICLO ILUMINACION
if ((MinActual >= 480) && (MinActual <= 509) )                                ////////////////////////// Entre las 8:00 y 8:29
  {
      ciclo_ledAzul = map(MinActual, 480, 509, 10, 0);                        //Distribuyo la transición de la luz a lo largo del bloque de tiempo
      ciclo_ledVioleta = map(MinActual, 480, 509, 10, 5);
      ciclo_ledRojo = map(MinActual, 480, 509, 0, 20);
      ciclo_ledBlanco = map(MinActual, 480, 509, 0, 10);
  }
else if ((MinActual >= 510) && (MinActual <= 539) )                                //////////////////////////////// Entre las 8:30 y 8:59
  {
    ciclo_ledRojo = map(MinActual, 510, 539, 20, 25);
    ciclo_ledBlanco = map(MinActual, 510, 539, 10, 25);
    ciclo_ledAzul = map(MinActual, 510, 539, 0, 10);
    ciclo_ledVioleta = map(MinActual, 510, 539, 5, 10);
  }
else if ((MinActual >= 540) && (MinActual <= 569) )                               //////////////////////////////// Entre las 9:00 y 9:29
  {
    ciclo_ledRojo = map(MinActual, 540, 569, 25, 35);
    ciclo_ledBlanco = map(MinActual, 540, 569, 25, 35);
    ciclo_ledAzul = map(MinActual, 540, 569, 10, 35);
    ciclo_ledVioleta = map(MinActual, 540, 569, 10, 35);
  }
else if ((MinActual >= 570) && (MinActual <= 599) )                                //////////////////////////////// Entre las 9:30 y 9:59
  {
    ciclo_ledRojo = map(MinActual, 570, 599, 35, 60);
    ciclo_ledBlanco = map(MinActual, 570, 599, 35, 60);
    ciclo_ledAzul = map(MinActual, 570, 599, 35, 60);
    ciclo_ledVioleta = map(MinActual, 570, 599, 35, 60);
  }
else if ((MinActual >= 600) && (MinActual <= 659) )                               //////////////////////////////// Entre las 10:00 y 10:59
  {
    ciclo_ledRojo = map(MinActual, 600, 659, 60, 100);
    ciclo_ledBlanco = map(MinActual, 600, 659, 60, 100);
    ciclo_ledAzul = map(MinActual, 600, 659, 60, 100);
    ciclo_ledVioleta = map(MinActual, 600, 659, 60, 100);
  }
else if ((MinActual >= 660) && (MinActual <= 1079) )                              //////////////////////////////// Entre las 11:00 y 17:59
  {
    ciclo_ledRojo = map(MinActual, 660, 1079, 100, 255);
    ciclo_ledBlanco = map(MinActual, 660, 1079, 100, 255);
    ciclo_ledAzul = map(MinActual, 660, 1079, 100, 255);
    ciclo_ledVioleta = map(MinActual, 660, 1079, 100, 255);
  }
else if ((MinActual >= 1080) && (MinActual <= 1139) )                             ////////////////////////////////Entre las 18:00 y 18:59
  {
    ciclo_ledRojo = map(MinActual, 1080, 1139, 255, 150);
    ciclo_ledBlanco = map(MinActual, 1080, 1139, 255, 150);
    ciclo_ledAzul = map(MinActual, 1080, 1139, 255, 150);
    ciclo_ledVioleta = map(MinActual, 1080, 1139, 255, 150);
  }
else if ((MinActual >= 1140) && (MinActual <= 1169) )                              //////////////////////////////// Entre las 19:00 y 19:29
  {
    ciclo_ledRojo = map(MinActual, 1140, 1169, 150, 125);
    ciclo_ledBlanco = map(MinActual, 1140, 1169, 150, 125);
    ciclo_ledAzul = map(MinActual, 1140, 1169, 150, 125);
    ciclo_ledVioleta = map(MinActual, 1140, 1169, 150, 125);
  }
else if ((MinActual >= 1170) && (MinActual <= 1199) )                               //////////////////////////////// Entre las 19:30 Y 19:50
  {
    ciclo_ledRojo = map(MinActual, 1170, 1199, 125, 110);
    ciclo_ledBlanco = map(MinActual, 1170, 1199, 125, 110);
    ciclo_ledAzul = map(MinActual, 1170, 1199, 125, 110);
    ciclo_ledVioleta = map(MinActual, 1170, 1199, 125, 110);
  }
else if ((MinActual >= 1200) && (MinActual <= 1229) )                               //////////////////////////////// Entre las 20:00 y 20:29
  {
    ciclo_ledRojo = map(MinActual, 1200, 1229, 110, 100);
    ciclo_ledBlanco = map(MinActual, 1200, 1229, 110, 100);
    ciclo_ledAzul = map(MinActual, 1200, 1229, 110, 100);
    ciclo_ledVioleta = map(MinActual, 1200, 1229, 110, 100);
  }
else if ((MinActual >= 1230) && (MinActual <= 1259) )                              //////////////////////////////// Entre las 20:30 y 20:59
  {
    ciclo_ledRojo = map(MinActual, 1230, 1259, 100, 100);
    ciclo_ledBlanco = map(MinActual, 1230, 1259, 100, 80);
    ciclo_ledAzul = map(MinActual, 1230, 1259, 100, 80);
    ciclo_ledVioleta = map(MinActual, 1230, 1259, 100, 80);
  }
else if ((MinActual >= 1260) && (MinActual <= 1289) )                             //////////////////////////////// Entre las 21:00 Y 21:29
  {
    ciclo_ledRojo = map(MinActual, 1260, 1289, 100, 90);
    ciclo_ledBlanco = map(MinActual, 1260, 1289, 80, 70);
    ciclo_ledAzul = map(MinActual, 1260, 1289, 80, 70);
    ciclo_ledVioleta = map(MinActual, 1260, 1289, 80, 70);
  }
else if ((MinActual >= 1290) && (MinActual <= 1319) )                             //////////////////////////////// Entre las 21:30 y 21:59
  {
    ciclo_ledRojo = map(MinActual, 1290, 1319, 90, 70);
    ciclo_ledBlanco = map(MinActual, 1290, 1319, 70, 50);
    ciclo_ledAzul = map(MinActual, 1290, 1319, 70, 50);
    ciclo_ledVioleta = map(MinActual, 1290, 1319, 70, 50);
  }
else if ((MinActual >= 1320) && (MinActual <= 1379) )                            //////////////////////////////// Entre las 22:00 y 22:59
  {
    ciclo_ledRojo = map(MinActual, 1320, 1379, 70, 50);
    ciclo_ledBlanco = map(MinActual, 1320, 1379, 50, 40);
    ciclo_ledAzul = map(MinActual, 1320, 1379, 50, 40);
    ciclo_ledVioleta = map(MinActual, 1320, 1379, 50, 40);
  }
else if ((MinActual >= 1380) && (MinActual <= 1439) )                            //////////////////////////////// Entre las 23:00 y 00:00
  {
    ciclo_ledRojo = map(MinActual, 1380, 1439, 50, 25);
    ciclo_ledBlanco = map(MinActual, 1380, 1439, 40, 25);
    ciclo_ledAzul = map(MinActual, 1380, 1439, 40, 30);
    ciclo_ledVioleta = map(MinActual, 1380, 1439, 40, 30);
  }
else if ((MinActual >= 0) && (MinActual <= 60) )                                 //////////////////////////////// Entre las 00:01 y 01:00
  {
    ciclo_ledRojo = map(MinActual, 0, 60, 25, 0);
    ciclo_ledBlanco = map(MinActual, 0, 60, 25, 0);
    ciclo_ledAzul = map(MinActual, 0, 60, 30, 25);
    ciclo_ledVioleta = map(MinActual, 0, 60, 30, 25);
  }
else if ((MinActual >= 61) && (MinActual <= 479) )                               //////////////////////////////// Entre las 01:01 y 07:59
  {
      ciclo_ledRojo = 0;
      ciclo_ledBlanco = 0;
      ciclo_ledAzul = map(MinActual, 61, 479, 25, 10);
      ciclo_ledVioleta = map(MinActual, 61, 479, 25, 10);
  }


analogWrite(ledRojo,ciclo_ledRojo);    //Actualiza el pwm de los leds
analogWrite(ledBlanco,ciclo_ledBlanco);
analogWrite(ledAzul,ciclo_ledAzul);
analogWrite(ledVioleta,ciclo_ledVioleta);

segundo = millis(); //Checa los milisegundos actuales desde que se empezó el programa
if(segundo>=(prevSegundo+1000)){ //Compara si ya pasó un segundo completo
 if(modo==0) Serial.println("Modo Automatico");
 else Serial.println("Modo Manual");
 Serial.print("HORA: ");
 Serial.println(MinActual);
 Serial.print("Ciclo Azul: ");
 Serial.println(ciclo_ledAzul);
 Serial.print("Ciclo Violeta: ");
 Serial.println(ciclo_ledVioleta);
 Serial.print("Ciclo Blanco: ");
 Serial.println(ciclo_ledBlanco);
 Serial.print("Ciclo Rojo: ");
 Serial.println(ciclo_ledRojo);
 Serial.println();
 prevSegundo=millis(); //Actualiza el segundo
}

}
