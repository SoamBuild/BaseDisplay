/*
*Codigo de mesa vibratoria para conocer el funcionamiento de las funciones y del menu
*revisar readme.md ubicado en la rama main
*los pines del sistema se encuentran detallados en pin32.md en la rama main
*/


#include <LiquidCrystal_I2C.h>
#include "Arduino.h"
#include <RotaryEncoder.h>
#include <ESP_FlexyStepper.h>

//Pines de conexion stepper Motor
const int MOTOR_X_STEP_PIN = 16;
const int MOTOR_X_DIRECTION_PIN = 17;
const int MOTOR_X_ENABLE = 18;
const int LIMIT_X_SWITCH_PIN = 4; 



const int MOTOR_Y_STEP_PIN = 33;
const int MOTOR_Y_DIRECTION_PIN = 25;
const int MOTOR_Y_ENABLE = 26;
const int LIMIT_Y_SWITCH_PIN = 19;

const int DEBUG_LED = 23;

//Objeto FlexyStepper
ESP_FlexyStepper stepper_X;
ESP_FlexyStepper stepper_Y;

//SCL 22
//SDA 21
LiquidCrystal_I2C lcd(0x27, 16, 2); // set the LCD address to 0x27 for a 16 chars and 2 line display

//Configuracion de rotaryEncoder
const int ENCODER_CLK = 13;
const int ENCODER_DT = 27;
const int ENCODER_SW = 32;

RotaryEncoder encoder(ENCODER_CLK, ENCODER_DT);
#define ROTARYSTEPS 1
int newPos;
int lastPos = -1;
//const int pulsador = 32;

//Debounce encodervariable
int check=0;

//Navegacion de menu
bool modificar =false;
int indexmenu=1;
//Variables de velocidad y distancia de eje (X,Y)
int velocidad_X=0;
int distancia_X=0;
int velocidad_Y=0;
int distancia_Y=0;

void setup()
{
  Serial.begin(115200);
  stepper_X.connectToPins(MOTOR_X_STEP_PIN, MOTOR_X_DIRECTION_PIN);
  stepper_Y.connectToPins(MOTOR_Y_STEP_PIN, MOTOR_Y_DIRECTION_PIN);
  pinMode(LIMIT_X_SWITCH_PIN, INPUT_PULLUP);
  pinMode(LIMIT_Y_SWITCH_PIN, INPUT_PULLUP);
  pinMode(DEBUG_LED,OUTPUT);
  pinMode (ENCODER_SW, INPUT_PULLUP); //boton de un esp32
  lcd.init();
  lcd.clear();
  lcd.backlight();
  homi_X();
  delay(1000);
 // lcd.clear();
  homi_Y();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("MotorControl 1.1");
  delay(2000);
  lcd.clear();
}
void loop()
{
  if(modificar==true) {
    rotary(1,100);
    cambiarValores(modificar,indexmenu);
  }
  if(modificar==false) rotary(1,10);
  
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
  if(modificar==false){
    indexmenu=newPos;
    menuDisplay(indexmenu);
  } //indexmenu=newPos;
 // menuDisplay(indexmenu);
  lastPos = newPos;
  }

}
void buttonControl(){
  int presionado = 0;
  if (digitalRead(ENCODER_SW) == LOW) 
  {
    presionado = 1; 
    delay(50);
  }
  if (digitalRead(ENCODER_SW) == HIGH && presionado == 1)
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
        lcd.print("Vel_X");
        lcd.setCursor(6,0);
        lcd.print(String(velocidad_X)+" mm/s");
        lcd.setCursor(0,1);
        lcd.print("Dis_X");
        lcd.setCursor(6,1);
        lcd.print(String(distancia_X)+" mm");
        break;
    case 2:
        lcd.clear();
        lcd.noCursor();
        lcd.setCursor(0,0);
        lcd.print("Velocidad X");
        break;
    case 3:
        lcd.clear();
        lcd.noCursor();
        lcd.setCursor(0,0);
        lcd.print("Distancia X"); 
        break;
    case 4:
        lcd.clear();
        lcd.noCursor();
        lcd.setCursor(0,0);
        lcd.print("Vel_Y");
        lcd.setCursor(6,0);
        lcd.print(String(velocidad_Y)+" mm/s");
        lcd.setCursor(0,1);
        lcd.print("Dis_Y");
        lcd.setCursor(6,1);
        lcd.print(String(distancia_Y)+" mm");
        break;
    case 5:
        lcd.clear();
        lcd.noCursor();
        lcd.setCursor(0,0);
        lcd.print("Velocidad Y");
        break;
    case 6:
        lcd.clear();
        lcd.noCursor();
        lcd.setCursor(0,0);
        lcd.print("Distancia Y"); 
        break;
    case 7:
        lcd.clear();
        lcd.noCursor();
        lcd.setCursor(0,0);
        lcd.print("PosM1 | ");
        lcd.setCursor(8,0);
        lcd.print(String(stepper_X.getCurrentPositionInMillimeters())+" mm");
        lcd.setCursor(0,1);
        lcd.print("PosM2 | ");
        lcd.setCursor(8,1);
        lcd.print(String(stepper_Y.getCurrentPositionInMillimeters())+" mm");
        break;
    case 8:
        lcd.clear();
        lcd.noCursor();
        lcd.setCursor(0,0);
        lcd.print("Home");   
        break;
    case 9:
        lcd.clear();
        lcd.noCursor();
        lcd.setCursor(0,0);
        lcd.print("Encender");   
        break;
    case 10:
        lcd.clear();
        lcd.noCursor();
        lcd.setCursor(0,0);
        lcd.print("Reset valores");   
        break;
  }
}
void homi_X() {
  lcd.clear();
  Serial.println("AutoHomeX");
  lcd.setCursor(0,0);
  lcd.print("AutoHome X"); 
  
  if (stepper_X.moveToHomeInMillimeters(-1, 30, 380, LIMIT_X_SWITCH_PIN) == true)
    {
      Serial.println("HOMING X OK");
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("AutoHome X OK");  
      delay(1000);
    }
    else {
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("AutoHome X Error"); 
      Serial.println("Error"); 
      while (true) {
      digitalWrite(DEBUG_LED, HIGH);
      delay(50);
      digitalWrite(DEBUG_LED, LOW);
      delay(50);
    }
    Serial.println("Error eje X");
  }
  
}
void homi_Y() {
  lcd.clear();
  Serial.println("AutoHomeY");
  lcd.setCursor(0,0);
  lcd.print("AutoHome Y"); 
  
  if (stepper_Y.moveToHomeInMillimeters(-1, 30, 380, LIMIT_Y_SWITCH_PIN) == true)
    {
      Serial.println("HOMING Y OK");
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("AutoHome Y OK");  
      delay(1000);
    }
    else {
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("AutoHome Y Error"); 
      Serial.println("Error"); 
      while (true) {
      digitalWrite(DEBUG_LED, HIGH);
      delay(50);
      digitalWrite(DEBUG_LED, LOW);
      delay(50);
    }
    Serial.println("Error eje Y");
  }
  
}
void cambiarValores(bool ok,int index){

  if(ok==true){
    if (index==2)
    {
      //Modificador Velocidad motorX
      velocidad_X=newPos;
      lcd.setCursor(0,1);
      lcd.cursor();
      lcd.setCursor(0,1);
      lcd.print(String(velocidad_X)+" mm/s");
    }
    if (index==3)
    {
      //Modificador distancia motorX
      distancia_X=newPos;
      lcd.setCursor(0,1);
      lcd.cursor();
      lcd.setCursor(0,1);
      lcd.print(String(distancia_X)+" mm");
    }
    if (index==5)
    {
      //Modificador Velocidad motorY
      velocidad_Y=newPos;
      lcd.setCursor(0,1);
      lcd.cursor();
      lcd.setCursor(0,1);
      lcd.print(String(velocidad_Y)+" mm/s");
    }
    if (index==6)
    {
      //Modificador distancia motorY
      distancia_Y=newPos;
      lcd.setCursor(0,1);
      lcd.cursor();
      lcd.setCursor(0,1);
      lcd.print(String(distancia_Y)+" mm");
    }
    if (index==8)
    {
      //Autohome de ambos ejes 
      homi_X(); 
      delay(1000);
      homi_Y();
      check=0;
      modificar=false;
      lcd.clear();
      menuDisplay(1);
    }
    if (index==9)
    {
      //Check si hay datos para moverse.
      if(velocidad_X==0 || distancia_X==0){

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
      //Se mueven ambos motores.
      stepper_X.setSpeedInMillimetersPerSecond(velocidad_X);
      stepper_Y.setSpeedInMillimetersPerSecond(velocidad_Y);
      lcd.setCursor(0,1);
      lcd.print("Moviendo");
      Serial.println("Moviendo");
      stepper_X.setTargetPositionInMillimeters(distancia_X);
      stepper_Y.setTargetPositionInMillimeters(distancia_Y);
      while ((!stepper_X.motionComplete()) || (!stepper_Y.motionComplete())){
        stepper_X.processMovement();
        stepper_Y.processMovement();
      }
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.println("   Completado   ");
      delay(1000);
      check=0;
      modificar=false;
      lcd.clear();
      menuDisplay(1);
      }
    } 
    if (index==10)
    {
     //reiniciar todos los datos.
      velocidad_X=0;
      distancia_X=0;
      velocidad_Y=0;
      distancia_Y=0;
      lcd.setCursor(0,1);
      lcd.print("Valores en 0");
      delay(1000);
      check=0;
      modificar=false;
      lcd.clear();
      menuDisplay(1);
    } 
  }
  else{
    lcd.clear();
    lcd.noCursor();
  }
}

