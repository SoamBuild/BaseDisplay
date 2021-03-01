#include <LiquidCrystal_I2C.h>
#include "Arduino.h"
#include <RotaryEncoder.h>
#include <ESP_FlexyStepper.h>

//Pines de conexion stepper Motor
const int MOTOR_STEP_PIN = 25;
const int MOTOR_DIRECTION_PIN = 26;
const int LIMIT_SWITCH_PIN = 33; 
const int DEBUG_LED = 23;


//Objeto FlexyStepper
ESP_FlexyStepper stepper;

//SCL 22
//SDA 21
LiquidCrystal_I2C lcd(0x27, 16, 2); // set the LCD address to 0x27 for a 16 chars and 2 line display

RotaryEncoder encoder(13, 27);
#define ROTARYSTEPS 1
int newPos;
int lastPos = -1;
const int pulsador = 32;
int check=0;
bool modificar =false;
int indexmenu=1;

int velocidad=0;
int distancia=0;
int lastVel=0;
int lastDis=0;

void setup()
{
  Serial.begin(115200);
  stepper.connectToPins(MOTOR_STEP_PIN, MOTOR_DIRECTION_PIN);
  pinMode(DEBUG_LED,OUTPUT);
  pinMode(LIMIT_SWITCH_PIN, INPUT_PULLUP);
  pinMode (pulsador, INPUT_PULLUP); //boton de un esp32
  lcd.init();
  lcd.clear();
  lcd.backlight();
  homi();
  lcd.setCursor(0, 0);
  lcd.print("MotorControl 1.1");
  delay(2000);
  lcd.clear();
}
void loop()
{
 // Serial.println(stepper.motionComplete());
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
        lcd.noCursor();
        lcd.setCursor(0,0);
        lcd.print("Vel | ");
        lcd.setCursor(7,0);
        lcd.print(String(velocidad)+" mm/s");
        lcd.setCursor(0,1);
        lcd.print("Dis | ");
        lcd.setCursor(7,1);
        lcd.print(String(distancia)+" mm");
        break;
    case 2:
        lcd.clear();
        lcd.noCursor();
        lcd.setCursor(0,0);
        lcd.print("Velocidad");
        break;
    case 3:
        lcd.clear();
        lcd.noCursor();
        lcd.setCursor(0,0);
        lcd.print("Distancia"); 
        break;
    case 4:
        lcd.clear();
        lcd.noCursor();
        lcd.setCursor(0,0);
        lcd.print("Encender");   
        break;
  }
}
void homi() {
  if (stepper.moveToHomeInMillimeters(-1, 30, 380, LIMIT_SWITCH_PIN) == true)
  {
    Serial.println("HOMING OK");
  }
  else {
    while (true) {
      digitalWrite(DEBUG_LED, HIGH);
      delay(50);
      digitalWrite(DEBUG_LED, LOW);
      delay(50);
    }
    Serial.println("Error");

  }
}
void cambiarValores(bool ok,int index){

  if(ok==true){
    if (index==2)
    {
       if (lastVel != newPos) {
        if(lastVel<newPos)velocidad=velocidad+1;
        if(lastVel>newPos)velocidad=velocidad-1;
        lastVel=newPos;
      }
      lcd.setCursor(0,1);
      lcd.cursor();
      lcd.setCursor(0,1);
      lcd.print(String(velocidad)+" mm/s");
    }
    if (index==3)
    {
      if (lastDis != newPos) {
        if(lastDis<newPos)distancia=distancia+1;
        if(lastDis>newPos)distancia=distancia-1;
        lastDis=newPos;
      }
      lcd.setCursor(0,1);
      lcd.cursor();
      lcd.setCursor(0,1);
      lcd.print(String(distancia)+" mm");
    }
     if (index==4)
    {
      if(velocidad==0 || distancia==0){

        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Error");
        lcd.setCursor(0,1);
        lcd.print("valores = 0");
        delay(2000);
        menuDisplay(1);
        modificar=false;

      }
      else{
      lcd.setCursor(0,1);
      lcd.print("Moviendo");
      Serial.println("Moviendo");
      stepper.setSpeedInMillimetersPerSecond(velocidad);
      stepper.moveRelativeInMillimeters(distancia);
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.println("   Completado   ");
      delay(1000);
      menuDisplay(1);
      modificar=false;
      }
    } 
  }else 
  {
    lcd.clear();
    lcd.noCursor();
  }
    
}

