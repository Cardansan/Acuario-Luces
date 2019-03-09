#include <Wire.h>
#include "RTClib.h"
RTC_DS3231 rtc;
#include <LCD.h>
#include <LiquidCrystal_I2C.h>
#include <OneWire.h> //biblioteca protocolo OneWire
#include <DallasTemperature.h> //Biblioteca sensor
#include<EEPROM.h>

LiquidCrystal_I2C lcd( 0x3F, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE ); //0x3F direccion I2C display, resto pines de funcinamiento del display(no variarlo) no tiene que ver con arduino, POSITIVE back light

// Pin donde se conecta el bus 1-Wire
const int pinDatosDQ = 9;//PIN DONDE SE CONECTAN LOS SENSORES

// Instancia a las clases OneWire y DallasTemperature
OneWire oneWireObjeto(pinDatosDQ);
DallasTemperature sensorDS18B20(&oneWireObjeto);

// Variables con las direcciones únicas de los 2 sensores DS18B20
DeviceAddress sensor1 = {0x28, 0xFF, 0x3E, 0xC7, 0x31, 0x17, 0x03, 0xC4}; //NUMERO DE SERIE 28FF3EC7311703C4
DeviceAddress sensor2 = {0x28, 0xFF, 0x23, 0x0A, 0x54, 0x17, 0x04, 0xE3};

const int out_blanco=3;  //Nombra out_blanco para pin 3
const int out_azul=6;
const int out_rojo=5;
const int out_fan=10; // Salida motor ventilador
int ciclo_blanco=0; // ciclo de trabajo WFM salida led
int ciclo_azul=0;
int ciclo_rojo=0;
int ciclo_fan=0;
long pulso=0;
int teclado = A0; // Pin A0 entrada teclado pulsadores
int key=0;
int MinActual=0;
float termo=0; // Variable con el valor real de temperatura
float termo2=0; // Variable con el valor real de temperatura
int buzzer=8; //Buzzer alarma
int temp_memo; // Variable para guardar el valor de temperatura objetivo en canal 1
int temp_memo2; // Variable para guardar el valor de temperatura objetivo en canal 2  
int flag=0; //Bandera para desactivar buzzer  buzzer=LOW
int flag_led=0; //Bandera para activar luces de mantenimiento. 0 apagadas, 1 encendidas
int flag_display=0; //Bandera para mostrar dos capas de display
float temp; // Variable de conmutación del relé (LED Rojo)
float temp2; // Variable de conmutación del relé (LED Rojo)
int led=1; // LED de salida pin 1
int led2=0; // Led de salida2 en pin 0
int key_valor=0; //Variable para asignar un valor a cada tecla


//int a=0;
/////////////////////////////////////////////////////////////////////////////////////////////////////////

// Subprograma de alarma
void alarma()// Suena alarma
  { 
    digitalWrite(buzzer,HIGH);
    delay(50);
    digitalWrite(buzzer,LOW);
    delay(50);
    digitalWrite(buzzer,HIGH);
    delay(50);
    digitalWrite(buzzer,LOW);
  }// Fin Subprograma de alarma

/////////////////////////////////////////////////////////////////////////////////////////////////////////
// Subprograma de temperatura
void temperatura()// Control de seleccion de temperatura del canal 1. Ajuste de umbral 
  { 
    lcd.clear();  
    key_valor=0;
    digitalWrite(buzzer,HIGH);
    delay(100);
    digitalWrite(buzzer,LOW);    
    delay(500);   
   
    while(key_valor != 5) // Umbral de conmutación del relé (LED Rojo)

  { 
    key=analogRead(teclado); //////////////////////////////////////////////////////////LEO LOS SWITCH DEL TECLADO     
    if ((key>900) && (key<1024))key_valor=1;  /////////////////// UP
    if ((key>200) && (key<400)) key_valor=5; /////////////////////CENTER
    if (key<199)  key_valor=0; ///////////////////////////////////NO PUSH


    if (key_valor == 1)        
      {         
        temp = temp + 1; // Incrementos de  grados
      }
       
    if ((temp < 24) or (temp > 31))
      {
        temp = 24;
      }
        
    lcd.setCursor(2,0);            
    lcd.print("AJUSTE TEMP1");
    lcd.setCursor(2,1);            
    lcd.print("TEMP1:");
    lcd.print(temp);
    lcd.blink();
    delay(500);
  }
  
  digitalWrite(buzzer,HIGH);
  delay(100);
  digitalWrite(buzzer,LOW);  
  lcd.clear();
  lcd.setCursor(2,1);
  lcd.noBlink();
  lcd.print(temp);
  lcd.print(" Grados");
  delay(1000);
  lcd.setCursor(0,1);
  lcd.print("TEMP MEMORIZADA");
  temp_memo=temp*100;
  EEPROM.put(0,temp_memo);
  delay(1000); 
  lcd.clear();
  
} // Fin Subprograma de temperatura


// Subprograma de temperatura2
void temperatura2()// Control de seleccion de temperatura del canal 1. Ajuste de umbral 
  { 
    lcd.clear();  
    key_valor=0;
    digitalWrite(buzzer,HIGH);
    delay(100);
    digitalWrite(buzzer,LOW);    
    delay(500);   
   
    while(key_valor != 5) // Umbral de conmutación del relé (LED Rojo)

  { 
    key=analogRead(teclado); //////////////////////////////////////////////////////////LEO LOS SWITCH DEL TECLADO
     
    if ((key>700) && (key<900)) key_valor=2;  //lcd.print(key_valor);// DOWN
    if ((key>200) && (key<400)) key_valor=5;  //lcd.print(key_valor);//CENTER
    if (key<199)  key_valor=0;  //lcd.print(key_valor);//NO PUSH
    
    if (key_valor == 2)
        
      {         
        temp2 = temp2 + 1; // Incrementos de  grados
      }
       
    if ((temp2 < 24) or (temp2 > 31))
      {
        temp2 = 24;
      }
        
    lcd.setCursor(2,0);            
    lcd.print("AJUSTE TEMP2");
    lcd.setCursor(2,1);            
    lcd.print("TEMP2:");
    lcd.print(temp2);
    lcd.blink();
    delay(500);
  }
  
  digitalWrite(buzzer,HIGH);
  delay(100);
  digitalWrite(buzzer,LOW);  
  lcd.clear();
  lcd.setCursor(2,1);
  lcd.noBlink();
  lcd.print(temp2);
  lcd.print(" Grados");
  delay(1000);
  lcd.setCursor(0,1);
  lcd.print("TEMP MEMORIZADA");
  temp_memo2=temp2*100;
  EEPROM.put(10,temp_memo2);
  delay(1000); 
  lcd.clear();
  
} // Fin Subprograma de temperatura2

// Subprograma activacion luces de mantenimiento
void mantenimiento()
 {
  flag_led=flag_led+1;
  digitalWrite(buzzer,HIGH);
  delay(50);
  digitalWrite(buzzer,LOW);
  lcd.setCursor(0,1); 
  lcd.print("*"); //Indica que la alarma está silenciada.
  if (flag_led > 1) 
   {
    flag_led=0;
    digitalWrite(buzzer,HIGH);
    delay(50);
    digitalWrite(buzzer,LOW);
    lcd.setCursor(0,1); 
    lcd.print(" "); //Borro el indicador de alarma desactivada
   }
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
 }
// Fin Subprograma activacion luces de mantenimiento


void setup()
   {

  sensorDS18B20.begin(); // Iniciamos el bus 1-Wire
  sensorDS18B20.setResolution(sensor1, 10); // ajustamos la resolución del sensor a 10 bits. Puede ser 9, 10, 11 o 12   (0,5 ° C, 0,25 ° C, 0.125 ° C y 0,0625)
  sensorDS18B20.setResolution(sensor2, 10); // ajustamos la resolución del sensor a 10 bits. Puede ser 9, 10, 11 o 12   (Por defecto la resolucion es 12 bits. Se guarda en la eeprom del sensor)

  lcd.begin(16, 2);
  lcd.setCursor(2, 0); 
  lcd.print("TERMOSTATO");
  lcd.setCursor(2, 1);
  lcd.print("ACUARIO Ver1");
  delay(1000);
  lcd.clear();

  lcd.setCursor(3, 0); 
  lcd.print(sensorDS18B20.getDeviceCount()); //INTERROGO CUANTOS SENSORES HAY CONECTADOS Y LOS MUESTRO EN DISPLAY
  lcd.print(" SENSORES");
  lcd.setCursor(3, 1);
  lcd.print("ENCONTRADOS");
  delay(2000);
  lcd.clear();
  
  if (sensorDS18B20.getDeviceCount() == 2)

    {
     lcd.setCursor(4, 0); 
    lcd.print("SENSORES");
     lcd.setCursor(7, 1);
     lcd.print("OK");
     delay(2000);
     lcd.clear(); 
    }
    
  else
  
    {
     lcd.setCursor(5, 0); 
     lcd.print("ERROR");
     lcd.setCursor(4, 1);
     lcd.print("SENSORES");
     delay(2000);
     lcd.clear(); 
   }
     
     
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
    lcd.begin (16,2);    // Inicializar el display con 16 caraceres 2 lineas

/////////////////////////////////////////////////////////////ASIGNO VALORES A LOS PULSOS PWM POR SI HAY CORTE DE LUZ

DateTime now = rtc.now(); 
MinActual = now.hour()*60+now.minute();
//if ((MinActual >= 480) & (MinActual <= 659))////////////////////////////// Entre las 8:00 y 10:59
 // {
 //   ciclo_blanco=0; 
//    ciclo_azul=0;
//    ciclo_rojo=0;
// }
 
if ((MinActual >= 660) & (MinActual <= 1439))////////////////////////////// Entre las 11:00 y 17:59
 {
   ciclo_blanco=250; 
   ciclo_azul=220;
   ciclo_rojo=220;
 } 
if ((MinActual >= 0) & (MinActual <= 479))////////////////////////////// Entre las 00:00 y 07:59
 {
   ciclo_blanco=0; 
   ciclo_azul=25;
   ciclo_rojo=0;
 } 


  digitalWrite(buzzer,HIGH);
  delay(50);
  digitalWrite(buzzer,LOW);

 EEPROM.get(0,temp_memo);//Leo el valor de la eeprom temperatura objetivo ajustada para canal 1
  temp=temp_memo/100;

 EEPROM.get(10,temp_memo2);//Leo el valor de la eeprom temperatura objetivo ajustada para canal 2
  temp2=temp_memo2/100;

  pinMode(led,OUTPUT);
  pinMode(led2,OUTPUT);
  digitalWrite(led,LOW);
  digitalWrite(led2,LOW);
  pinMode(buzzer,OUTPUT); 
   digitalWrite(buzzer,HIGH);
  delay(50);
  digitalWrite(buzzer,LOW);
   
  pinMode (out_blanco, OUTPUT);  // configura out_blanco "pin3" como salida
  analogWrite(out_blanco,LOW);  
  pinMode (out_azul, OUTPUT);  // configura out_azul "pin9" como salida
  analogWrite(out_azul,LOW);   
  pinMode (out_rojo, OUTPUT);  // configura out_rojo "pin5" como salida
  analogWrite(out_rojo,LOW);  
} 
void loop()
{ 
  ///////////////////////////////////////////////////////CONTROL VELOCIDAD VENTILADOR
  ciclo_fan=ciclo_rojo;
  if (ciclo_rojo < 75) ciclo_fan=0;
  if (ciclo_rojo > 175) ciclo_fan=175;
  analogWrite(out_fan,ciclo_fan);

  if (ciclo_rojo < 0) ciclo_rojo=250;/////////////////////////////Cuando vuelvo de luz mantenimento, impido que el valor entre en negativo
  if (ciclo_blanco < 0) ciclo_blanco=250;
  if (ciclo_azul < 0) ciclo_azul=250;

  //pulso = pulseIn (7, HIGH); //////////////////////////////////////////////////////////////000000000

  // LECTURA DE LOS SENSORES
  sensorDS18B20.requestTemperatures();  // Mandamos comandos para toma de temperatura a los sensores
  termo=(sensorDS18B20.getTempC(sensor1)); //Igualo la variable con el valor leido del sensor
  termo2=(sensorDS18B20.getTempC(sensor2));
  
  /////////////////////////////////////////IMPRIMO LOS DATOS DE TEMPERATURA

 if ((termo > 0)&&(termo2 > 0))   // Imprimo si la temperatura es mayor a 0º. Por algun motivo los sensores aleatoriamente leen -127º

  {
   lcd.setCursor(1,0);
   lcd.print("T1:"); 
   lcd.print(termo); 
   lcd.setCursor(1,1);
   lcd.print("T2:");
   lcd.print(termo2);
   lcd.setCursor(9,0);
   lcd.print(" ");
   lcd.setCursor(9,1);
   lcd.print(" ");
  }

  ////////////////////////////////////////IMPRIMO HORA DEL RELOJ
  DateTime now = rtc.now(); 
  lcd.setCursor ( 10, 1 ); 
  if (now.hour()<=9) lcd.print('0'); //Si es valor < a 9 pongo un cero para que los caracteres sean siempre de dos digitos 00:00
  lcd.print(now.hour(), DEC);
  lcd.print(':');
  if (now.minute()<=9) lcd.print('0');
  lcd.print(now.minute(), DEC);

  ///////////////////////////////////////////////////////////////////////////ACTIVO LAS SALIDAS DE LO CALENTADORES


 if ((termo < temp) && (termo > 0)) // Activacion rele calentador canal 1. Activo la salida si la temperatura es mayor a 0º. Por algun motivo los sensores aleatoriamente leen -127º
  
   {
    digitalWrite(led,HIGH);
    lcd.setCursor(10,0);
    lcd.print("ON");

   }
   
 if (termo >= temp)// Desactivacion rele calentador canal 1 
 
   {
    digitalWrite(led,LOW);
    lcd.setCursor(10,0);
    lcd.print("  ");
    delay (100);
   }
   
 if ((termo2 < temp2) && (termo2 > 0)) // Activacion rele calentador canal 2. Activo la salida si la temperatura es mayor a 0º. Por algun motivo los sensores aleatoriamente leen -127º
  
   {
    digitalWrite(led2,HIGH);
    lcd.setCursor(13,0);
    lcd.print("ON");
   }
   
 if (termo2 >= temp2)// Desactivacion rele claentador canal 2  
 
   {
    digitalWrite(led2,LOW);
    lcd.setCursor(13,0);
    lcd.print("  ");
    delay (100);
   }
  
key=analogRead(teclado); ///////////////////////////////////////////////////////// LEO LOS SWITCH DEL TECLADO
if ((key>900) && (key<1024))  //////////////////////////////////////////////////// UP
{
  temperatura(); //VOY A SUBRUTINA DE TEMPERATURA
}
if ((key>700) && (key<900))  ///////////////////////////////////////////////////// DOWN
{
  temperatura2(); //VOY A SUBRUTINA DE TEMPERATURA2
}

//if ((key>550) && (key<700))// key_valor=3; ///////////////////////////////////////// RIGHT

if ((key>400) && (key<550)) //key_valor=4; ///////////////////////////////////////// LEFT
{
mantenimiento();
}
if ((key>200) && (key<400))  /////////////////////////////////////////////////////// CENTER
 {  
  flag=flag+1;
  digitalWrite(buzzer,HIGH);
  delay(50);
  digitalWrite(buzzer,LOW);
  lcd.setCursor(0,0); 
  lcd.print("*"); //Indica que la alarma está silenciada.
  delay(10);
  if (flag > 1) 
   {
    flag=0;
    digitalWrite(buzzer,HIGH);
    delay(50);
    digitalWrite(buzzer,LOW);
    lcd.setCursor(0,0); 
    lcd.print(" "); //Borro el indicador de alarma desactivada
   }
 }
if (key<199)  key_valor=0; //////////////////////////////////////////////////////////NO PUSH

 
 if ((termo >0) && (termo <24) && (flag==0)) {alarma();} //termo>0 es porque a veces los sensores leen -127º. Asi cuando lee el error no suena la alarma
 if ((termo <24) && (flag==1)) {digitalWrite(buzzer,LOW);}
 if ((termo >31) && (flag==0)) {alarma();}
 if ((termo >31) && (flag==1)) {digitalWrite(buzzer,LOW);}
 if ((termo >=24) && (termo<=31)) {digitalWrite(buzzer,LOW);}

 if ((termo2 >0) && (termo2 <24) && (flag==0)) {alarma();}  //termo2>0 es porque a veces los sensores leen -127º. Asi cuando lee el error no suena la alarma
 if ((termo2 <24) && (flag==1)) {digitalWrite(buzzer,LOW);}
 if ((termo2 >31) && (flag==0)) {alarma();}
 if ((termo2 >31) && (flag==1)) {digitalWrite(buzzer,LOW);}
 if ((termo2 >=24) && (termo<=31)) {digitalWrite(buzzer,LOW);}
 

   


///////////////////////////////////////////////////////////////////////////COMIENZA CICLO ILUMINACION
MinActual = now.hour()*60+now.minute();///////////////////////////////////CONVIERTO LA HORA A MINUTOS PARA OPERAR MEJOR
if ((MinActual >= 480) & (MinActual <= 509) & (flag_led==0))////////////////////////////// Entre las 8:00 y 8:29
  {      
      ciclo_azul = 0;  
      analogWrite(out_azul,ciclo_azul);   
      if (ciclo_rojo != 20)analogWrite(out_rojo,ciclo_rojo), ciclo_rojo++;
      if (ciclo_blanco != 10) analogWrite(out_blanco,ciclo_blanco), ciclo_blanco++;
      delay(100); 
  }
if ((MinActual >= 510) & (MinActual <= 539) & (flag_led==0))////////////////////////////// Entre las 8:30 y 8:59
  {      
      if (ciclo_rojo != 25) analogWrite(out_rojo,ciclo_rojo), ciclo_rojo++;
      if (ciclo_blanco != 25) analogWrite(out_blanco,ciclo_blanco), ciclo_blanco++; 
      if (ciclo_azul != 10) analogWrite(out_azul,ciclo_azul),ciclo_azul++;      
      delay(100); 
  }
if ((MinActual >= 540) & (MinActual <= 569) & (flag_led==0))////////////////////////////// Entre las 9:00 y 9:29
  {      
      if (ciclo_rojo != 35) analogWrite(out_rojo,ciclo_rojo), ciclo_rojo++;
      if (ciclo_blanco != 35) analogWrite(out_blanco,ciclo_blanco), ciclo_blanco++; 
      if (ciclo_azul != 35) analogWrite(out_azul,ciclo_azul),ciclo_azul++;      
      delay(100); 
  }
if ((MinActual >= 570) & (MinActual <= 599) & (flag_led==0))////////////////////////////// Entre las 9:30 y 9:59
  {      
      if (ciclo_rojo != 60) analogWrite(out_rojo,ciclo_rojo), ciclo_rojo++;
      if (ciclo_blanco != 60) analogWrite(out_blanco,ciclo_blanco), ciclo_blanco++; 
      if (ciclo_azul != 60) analogWrite(out_azul,ciclo_azul),ciclo_azul++;      
      delay(100); 
  }
if ((MinActual >= 600) & (MinActual <= 659) & (flag_led==0))////////////////////////////// Entre las 10:00 y 10:59
  {      
      if (ciclo_rojo != 100) analogWrite(out_rojo,ciclo_rojo), ciclo_rojo++;
      if (ciclo_blanco != 100) analogWrite(out_blanco,ciclo_blanco), ciclo_blanco++; 
      if (ciclo_azul != 100) analogWrite(out_azul,ciclo_azul),ciclo_azul++;      
      delay(100); 
  }
if ((MinActual >= 660) & (MinActual <= 1079) & (flag_led==0))////////////////////////////// Entre las 11:00 y 17:59
  {      
      if (ciclo_rojo != 225) analogWrite(out_rojo,ciclo_rojo), ciclo_rojo++;
      if (ciclo_blanco != 255) analogWrite(out_blanco,ciclo_blanco), ciclo_blanco++; 
      if (ciclo_azul != 225) analogWrite(out_azul,ciclo_azul),ciclo_azul++;      
      delay(100); 
  }
if ((MinActual >= 1080) & (MinActual <= 1139) & (flag_led==0))////////////////////////////// Entre las 18:00 y 18:59
  {      
      if (ciclo_rojo != 150) analogWrite(out_rojo,ciclo_rojo), ciclo_rojo--;
      if (ciclo_blanco != 150) analogWrite(out_blanco,ciclo_blanco), ciclo_blanco--; 
      if (ciclo_azul != 150) analogWrite(out_azul,ciclo_azul),ciclo_azul--;      
      delay(100); 
  }
if ((MinActual >= 1140) & (MinActual <= 1169) & (flag_led==0))////////////////////////////// Entre las 19:00 y 19:29
  {      
      if (ciclo_rojo != 125) analogWrite(out_rojo,ciclo_rojo), ciclo_rojo--;
      if (ciclo_blanco != 125) analogWrite(out_blanco,ciclo_blanco), ciclo_blanco--; 
      if (ciclo_azul != 125) analogWrite(out_azul,ciclo_azul),ciclo_azul--;      
      delay(100); 
  }
if ((MinActual >= 1170) & (MinActual <= 1199) & (flag_led==0))////////////////////////////// Entre las 19:30 Y 19:50
  {      
      if (ciclo_rojo != 110) analogWrite(out_rojo,ciclo_rojo), ciclo_rojo--;
      if (ciclo_blanco != 110) analogWrite(out_blanco,ciclo_blanco), ciclo_blanco--;
      if (ciclo_azul != 110) analogWrite(out_azul,ciclo_azul),ciclo_azul--; 
      delay(100); 
  }
if ((MinActual >= 1200) & (MinActual <= 1229) & (flag_led==0))////////////////////////////// Entre las 20:00 y 20:29
  {      
      if (ciclo_rojo != 100) analogWrite(out_rojo,ciclo_rojo), ciclo_rojo--;
      if (ciclo_blanco != 100) analogWrite(out_blanco,ciclo_blanco), ciclo_blanco--;
      if (ciclo_azul != 100) analogWrite(out_azul,ciclo_azul),ciclo_azul--;     
      delay(100); 
  }
if ((MinActual >= 1230) & (MinActual <= 1259) & (flag_led==0))////////////////////////////// Entre las 20:30 y 20:59
  {      
      if (ciclo_rojo != 100) analogWrite(out_rojo,ciclo_rojo), ciclo_rojo--;
      if (ciclo_blanco != 80) analogWrite(out_blanco,ciclo_blanco), ciclo_blanco--; 
      if (ciclo_azul != 80) analogWrite(out_azul,ciclo_azul),ciclo_azul--;     
      delay(100); 
  }
if ((MinActual >= 1260) & (MinActual <= 1289) & (flag_led==0))////////////////////////////// Entre las 21:00 Y 21:29
  {      
      if (ciclo_rojo != 90) analogWrite(out_rojo,ciclo_rojo), ciclo_rojo--;
      if (ciclo_blanco !=70 ) analogWrite(out_blanco,ciclo_blanco), ciclo_blanco--;
      if (ciclo_azul != 70) analogWrite(out_azul,ciclo_azul),ciclo_azul--;     
      delay(100); 
  }
if ((MinActual >= 1290) & (MinActual <= 1319) & (flag_led==0))////////////////////////////// Entre las 21:30 y 21:59
  {      
      if (ciclo_rojo != 70) analogWrite(out_rojo,ciclo_rojo), ciclo_rojo--;
      if (ciclo_blanco != 50) analogWrite(out_blanco,ciclo_blanco), ciclo_blanco--; 
      if (ciclo_azul != 50) analogWrite(out_azul,ciclo_azul),ciclo_azul--;     
      delay(100); 
  }
if ((MinActual >= 1320) & (MinActual <= 1379) & (flag_led==0))////////////////////////////// Entre las 22:00 y 22:59
  {      
      if (ciclo_rojo != 50) analogWrite(out_rojo,ciclo_rojo), ciclo_rojo--;
      if (ciclo_blanco != 40) analogWrite(out_blanco,ciclo_blanco), ciclo_blanco--;
      if (ciclo_azul != 40) analogWrite(out_azul,ciclo_azul),ciclo_azul--;    
      delay(100); 
  }
if ((MinActual >= 1380) & (MinActual <= 1439) & (flag_led==0))////////////////////////////// Entre las 23:00 y 00:00
  {      
      if (ciclo_rojo != 25) analogWrite(out_rojo,ciclo_rojo), ciclo_rojo--;
      if (ciclo_blanco != 25) analogWrite(out_blanco,ciclo_blanco), ciclo_blanco--;
      if (ciclo_azul != 30) analogWrite(out_azul,ciclo_azul),ciclo_azul--;    
      delay(100); 
  }
if ((MinActual >= 0) & (MinActual <= 60) & (flag_led==0))////////////////////////////// Entre las 00:01 y 01:00
  {      
      if (ciclo_rojo != 0) analogWrite(out_rojo,ciclo_rojo), ciclo_rojo--;
      if (ciclo_blanco != 0) analogWrite(out_blanco,ciclo_blanco), ciclo_blanco--;
      if (ciclo_azul != 25) analogWrite(out_azul,ciclo_azul),ciclo_azul--;  
      delay(100); 
  }
if ((MinActual >= 61) & (MinActual <= 479) & (flag_led==0))////////////////////////////// Entre las 01:01 y 07:59
  {      
      //if (ciclo_rojo != 0) analogWrite(out_rojo,ciclo_rojo), ciclo_rojo--;
      // if (ciclo_blanco != 0) analogWrite(out_blanco,ciclo_blanco), ciclo_blanco--;
      ciclo_rojo = 0;  
      analogWrite(out_rojo,ciclo_rojo); 
      ciclo_blanco = 0;  
      analogWrite(out_blanco,ciclo_blanco);   
      if (ciclo_azul != 10) analogWrite(out_azul,ciclo_azul),ciclo_azul--;  
      delay(100); 
  } 
delay(50);
}
