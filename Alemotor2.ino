#include <LiquidCrystal_I2C.h>
#include "AiEsp32RotaryEncoder.h"
#include "Arduino.h"

#define ROTARY_ENCODER_A_PIN 32
#define ROTARY_ENCODER_B_PIN 33
#define ROTARY_ENCODER_BUTTON_PIN 25
#define ROTARY_ENCODER_VCC_PIN -1

AiEsp32RotaryEncoder rotaryEncoder = AiEsp32RotaryEncoder(ROTARY_ENCODER_A_PIN, ROTARY_ENCODER_B_PIN, ROTARY_ENCODER_BUTTON_PIN, ROTARY_ENCODER_VCC_PIN);
LiquidCrystal_I2C lcd(0x27, 16, 2); // set the LCD address to 0x27 for a 16 chars and 2 line display

int rotaryValor=1;
bool modificar=false;
int check=0;

//Variables de distancia y velocidad
int velocidad=0;
int distancia=0;


void rotary_loop() {

	if (rotaryEncoder.currentButtonState() == BUT_RELEASED) {
		
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
  }

	int16_t encoderDelta = rotaryEncoder.encoderChanged();
	
	if (encoderDelta == 0) return;
	
	//if (encoderDelta>0) Serial.print("+");
	//if (encoderDelta<0) Serial.print("-");

	
	if (encoderDelta!=0) {
		int16_t encoderValue = rotaryEncoder.readEncoder();
    rotaryValor=encoderValue;	
    Serial.print("Value: ");
		Serial.println(encoderValue);
	} 
	
}



void setup()
{
  Serial.begin(115200);

  rotaryEncoder.begin();
	rotaryEncoder.setup([]{rotaryEncoder.readEncoder_ISR();});
	//optionally we can set boundaries and if values should cycle or not
	rotaryEncoder.setBoundaries(1, 4, true); //minValue, maxValue, cycle values (when max go to min and vice versa)

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
	if (millis()>20000) rotaryEncoder.enable ();

 
 // bool changeValue=false;
   //Serial.println(changeValue);

  if(modificar==false){
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
          //changeValue=true;
          rotaryEncoder.setBoundaries(1, 4, false); 
          lcd.setCursor(0,1);
          lcd.print(distancia);
          rotaryValor=distancia;
        }
        break;
    case 3:
        lcd.clear();
        lcd.setCursor(1,0);
        lcd.print("Encender");   
        break;
    case 4:
        lcd.clear();
        lcd.setCursor(1,0);
        lcd.print("Velocidad");   
        break;

  }
  }
}
