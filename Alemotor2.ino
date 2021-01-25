#include <LiquidCrystal_I2C.h>
#include "Arduino.h"
#include <RotaryEncoder.h>


LiquidCrystal_I2C lcd(0x27, 16, 2); // set the LCD address to 0x27 for a 16 chars and 2 line display
RotaryEncoder encoder(13, 27);

int lastPos = -1;




int rotaryValor=1;
bool modificar=false;
int check=0;

//Variables de distancia y velocidad
int velocidad=0;
int distancia=0;


void setup()
{
  pinMode (32, INPUT_PULLUP); //boton de un esp32
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
  //int ROTARYMIN = 1;
  //int ROTARYMAX = 4;
  rotary_loop(1,4);
  
  //int valor  = limites(newPos,1,4);
															 
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
void rotary_loop(int ROTARYMIN, int ROTARYMAX){
  encoder.tick();
  int ROTARYSTEPS = 1;
  int newPos = encoder.getPosition() * ROTARYSTEPS;

  if (newPos < ROTARYMIN) {
    encoder.setPosition(ROTARYMIN / ROTARYSTEPS);
    newPos = ROTARYMIN;

  } else if (newPos > ROTARYMAX) {
    encoder.setPosition(ROTARYMAX / ROTARYSTEPS);
    newPos = ROTARYMAX;
  } // if

  if (lastPos != newPos) {
    Serial.print(newPos);
    Serial.println();
    lastPos = newPos;
  } // if
}


