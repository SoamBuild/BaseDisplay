#include <LiquidCrystal_I2C.h>
#include "Arduino.h"
#include <RotaryEncoder.h>

LiquidCrystal_I2C lcd(0x27, 16, 2); // set the LCD address to 0x27 for a 16 chars and 2 line display
RotaryEncoder encoder(13, 27);
#define ROTARYSTEPS 1
int newPos;
int lastPos = -1;
int pulsador = 32;
int check=0;
bool modificar =false;
int indexmenu=1;



int velocidad=0;
int distancia=0;

void setup()
{
  pinMode (pulsador, INPUT_PULLUP); //boton de un esp32
  Serial.begin(115200);
  lcd.init();
  lcd.clear();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("MotorControl  v1");
  delay(2000);
  lcd.clear();
}
void loop()
{
  if(modificar==true) {
    rotary(1,100);
    cambiarValores(modificar,indexmenu);
  }
  if(modificar==false) rotary(1,4);
  buttonControl();
  
}
void rotary(int ROTARYMIN,int ROTARYMAX){
  encoder.tick();
  newPos= encoder.getPosition() * ROTARYSTEPS;
  if (newPos < ROTARYMIN) {
    encoder.setPosition(ROTARYMIN / ROTARYSTEPS);
    newPos = ROTARYMIN;
  } else if (newPos > ROTARYMAX) {
    encoder.setPosition(ROTARYMAX / ROTARYSTEPS);
    newPos = ROTARYMAX;
  } 
  if (lastPos != newPos) {
  
  Serial.println(newPos);
  if(modificar==false) indexmenu=newPos;
  Serial.println(newPos);
  menuDisplay(indexmenu);
  lastPos = newPos;
  }

}

void buttonControl(){
  int presionado = 0;

  if (digitalRead(pulsador) == LOW) 
  {
    presionado = 1; 
    delay(50);
  }
  if (digitalRead(pulsador) == HIGH && presionado == 1)
  {
    check++;
    switch(check){
      case 1:
        modificar=true;
        Serial.println("Modificando valores");
        break;
      case 2:
        modificar=false;
        Serial.println("Guardando los valores");
        check =0;
        break;
    }
    presionado = 0;
  }

}
void menuDisplay(int mode){

   switch(mode){
    case 1:
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Vel | ");
        lcd.setCursor(7,0);
        lcd.print(velocidad);
        lcd.setCursor(0,1);
        lcd.print("Dis | ");
        lcd.setCursor(7,1);
        lcd.print(distancia);
        break;
    case 2:
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Velocidad");
        break;
    case 3:
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Distancia"); 
        break;
    case 4:
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Encender");   
        break;
  }
}
void cambiarValores(bool ok,int index){

  if(ok==true){
    
    if (index==2)
    {
      lcd.setCursor(15,0);
      lcd.print("#");
      velocidad= newPos;
      lcd.setCursor(0,1);
      lcd.print(velocidad);
    }
    if (index==3)
    {
      lcd.setCursor(15,0);
      lcd.print("#");
      distancia= newPos;
      lcd.setCursor(0,1);
      lcd.print(distancia);
    }
    
    
  }else{
    lcd.clear();
  }
    
}

