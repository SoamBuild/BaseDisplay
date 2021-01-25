#include <LiquidCrystal_I2C.h>
#include "Arduino.h"

LiquidCrystal_I2C lcd(0x27, 16, 2); // set the LCD address to 0x27 for a 16 chars and 2 line display

int rotaryValor=1;
bool modificar=false;
int check=0;

//Variables de distancia y velocidad
int velocidad=0;
int distancia=0;


void setup()
{
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
  rotary_loop();
	delay(50);															 
	//if (millis()>20000) rotaryEncoder.enable ();
  /*
  switch(rotaryValor){
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
        if (modificar==true){
          modificar=2;
          velocidad = encoderValue;
          lcd.setCursor(0,1);
          lcd.print(velocidad);
        }
        break;
    case 3:
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Distancia"); 
        if (modificar==true){
          modificar=3;
          distancia = encoderValue;
          lcd.setCursor(0,1);
          lcd.print(distancia);
        }  
        break;
    case 4:
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Encender");   
        break;
  }
  */
}
void rotaryloop(){
  
}