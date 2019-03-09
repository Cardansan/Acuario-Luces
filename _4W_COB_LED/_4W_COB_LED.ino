#define ACTIVATED LOW 
const int buttonPin = 2;     
const int MOSFETPIN =  10; 
boolean pressed = false; 


int buttonState = 0;    

void setup() {
  pinMode(MOSFETPIN, OUTPUT);
  pinMode(buttonPin, INPUT);
  digitalWrite(buttonPin,HIGH);
}

void loop() {

buttonState = digitalRead(buttonPin);
if(buttonState==ACTIVATED && pressed ==0)
{
  digitalWrite(MOSFETPIN,HIGH);
  pressed = true;
  delay(500);
}
else if (buttonState == ACTIVATED && pressed == 1)
{
  digitalWrite(MOSFETPIN,LOW);
  pressed = false;
  delay(500);
}
}

