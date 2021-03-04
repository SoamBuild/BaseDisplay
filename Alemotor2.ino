#include <LiquidCrystal_I2C.h>
#include "Arduino.h"
#include <RotaryEncoder.h>
#include <ESP_FlexyStepper.h>

//Pines de conexion stepper Motor
const int MOTOR_STEP_PIN = 25;
const int MOTOR_DIRECTION_PIN = 26;
const int LIMIT_SWITCH_PIN = 33; 
const int DEBUG_LED = 23;

int lastContador;
int contador;
int lastindex;

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

bool task=true; //No loop motores

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
  lcd.clear();
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
  if(modificar==false) rotary(1,10);
  
  buttonControl();
 // Serial.println("State "+ String(digitalRead(pulsador)));
  
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
Serial.println("1_press: "+ String(presionado)+ "/ STATE:"+ String(digitalRead(pulsador))+ "/ CHECK: "+ String(check));    //Serial.println("state2");
  }
  if (digitalRead(pulsador) == HIGH && presionado == 1 &&indexmenu>=3)
  {
    check++;
    switch(check){
      case 1:
        modificar=true;
        Serial.println("Modificando valores");
        Serial.println("2_press: "+ String(presionado)+ "/ STATE:"+ String(digitalRead(pulsador))+ "/ CHECK: "+ String(check));

        break;
      case 2:
        Serial.println("state5");
        modificar=false;
        Serial.println("Guardando los valores");
        check =0;
        Serial.println("3_press: "+ String(presionado)+ "/ STATE:"+ String(digitalRead(pulsador))+ "/ CHECK: "+ String(check));
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
        lcd.print("VelM1");
        lcd.setCursor(6,0);
        lcd.print(String(velocidad)+" mm/s");
        lcd.setCursor(0,1);
        lcd.print("DisM1");
        lcd.setCursor(6,1);
        lcd.print(String(distancia)+" mm");
        break;
    case 2:
        lcd.clear();
        lcd.noCursor();
        lcd.setCursor(0,0);
        lcd.print("Velocidad M1");
        break;
    case 3:
        lcd.clear();
        lcd.noCursor();
        lcd.setCursor(0,0);
        lcd.print("Distancia M1"); 
        break;
    case 4:
        lcd.clear();
        lcd.noCursor();
        lcd.setCursor(0,0);
        lcd.print("VelM2");
        lcd.setCursor(6,0);
        lcd.print(String(velocidad)+" mm/s");
        lcd.setCursor(0,1);
        lcd.print("DisM2");
        lcd.setCursor(6,1);
        lcd.print(String(distancia)+" mm");
        break;
    case 5:
        lcd.clear();
        lcd.noCursor();
        lcd.setCursor(0,0);
        lcd.print("Velocidad M2");
        break;
    case 6:
        lcd.clear();
        lcd.noCursor();
        lcd.setCursor(0,0);
        lcd.print("Distancia M2"); 
        break;
    case 7:
        lcd.clear();
        lcd.noCursor();
        lcd.setCursor(0,0);
        lcd.print("PosM1 | ");
        lcd.setCursor(8,0);
        lcd.print(String(stepper.getCurrentPositionInMillimeters())+" mm");
        lcd.setCursor(0,1);
        lcd.print("PosM2 | ");
        lcd.setCursor(8,1);
        lcd.print(String(stepper.getCurrentPositionInMillimeters())+" mm");
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
void homi() {
  lcd.setCursor(0,0);
  lcd.print("AutoHome"); 
  
  if (stepper.moveToHomeInMillimeters(-1, 30, 380, LIMIT_SWITCH_PIN) == true)
    {
      Serial.println("HOMING OK");
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("AutoHome OK");  
      delay(1000);
    }
    else {
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("AutoHome Error"); 
      Serial.println("Error"); 
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
    if (index==3)
    {
      velocidad=newPos;
      /*
       if (lastVel != newPos) {
        if(lastVel<newPos)velocidad=velocidad+1;
        if(lastVel>newPos)velocidad=velocidad-1;
        lastVel=newPos;
      }
      */
      lcd.setCursor(0,1);
      lcd.cursor();
      lcd.setCursor(0,1);
      lcd.print(String(velocidad)+" mm/s");
    }
    if (index==4)
    {
      distancia=newPos;
      /*
      if (lastDis != newPos) {
        if(lastDis<newPos)distancia=distancia+1;
        if(lastDis>newPos)distancia=distancia-1;
        lastDis=newPos;
      }
      */
      lcd.setCursor(0,1);
      lcd.cursor();
      lcd.setCursor(0,1);
      lcd.print(String(distancia)+" mm");
    }
     if (index==5)
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
      stepper.moveToPositionInMillimeters(distancia);
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
    if (index==6)
    {
     homi(); 
      check=0;
      modificar=false;
      lcd.clear();
      menuDisplay(1);

    }
     if (index==7)
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
      velocidad=0;
      distancia=0;
      lcd.setCursor(0,1);
      lcd.print("Valores en 0");
      delay(1000);
      check=0;
      modificar=false;
      lcd.clear();
      menuDisplay(1);
      }
    } 
  }else 
  {
    lcd.clear();
    lcd.noCursor();
    //menuDisplay(1);
    //newPos=1;
  }
    
}

