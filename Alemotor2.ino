/*
  Codigo de mesa vibratoria para conocer el funcionamiento de las funciones y del menu
  revisar readme.md ubicado en la rama main
  los pines del sistema se encuentran detallados en pin32.md en la rama main
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
int check = 0;

//Navegacion de menu
bool modificar = false;
int indexmenu = 1;
//Navegacion de submenu
bool submenu = false;
bool submenu_count = false;
int indexmenu2;
bool submenu_modificar = false;
bool rutina_Task = false;
//Variables de velocidad y distancia de eje (X,Y)
int velocidad_X = 10;
int distancia_X = 10;
int velocidad_Y = 10;
int distancia_Y = 10;

void IRAM_ATTR ISR() {

  if (rutina_Task == true) {
    
   // finaltask();
    rutina_Task = false;
    submenu_count = true;
    submenu_modificar = false;
    stepper_X.emergencyStop();
    stepper_Y.emergencyStop();
    digitalWrite(MOTOR_X_ENABLE,HIGH);
    digitalWrite(MOTOR_Y_ENABLE,HIGH);
   

    
  } else {
    Serial.println("No rutina");
  }
}
void setup()
{
  Serial.begin(115200);
  stepper_X.connectToPins(MOTOR_X_STEP_PIN, MOTOR_X_DIRECTION_PIN);
  pinMode(MOTOR_X_ENABLE,OUTPUT);
  digitalWrite(MOTOR_X_ENABLE,HIGH);
  stepper_Y.connectToPins(MOTOR_Y_STEP_PIN, MOTOR_Y_DIRECTION_PIN);
  pinMode(MOTOR_Y_ENABLE,OUTPUT);
  digitalWrite(MOTOR_Y_ENABLE,HIGH);

  pinMode(LIMIT_X_SWITCH_PIN, INPUT_PULLUP);
  pinMode(LIMIT_Y_SWITCH_PIN, INPUT_PULLUP);
  pinMode(DEBUG_LED, OUTPUT);
  pinMode (ENCODER_SW, INPUT_PULLUP); //boton de un esp32
  //Interrupcion no funcional
  attachInterrupt(ENCODER_SW, ISR, HIGH);
  lcd.init();
  lcd.clear();
  lcd.backlight();
  homi_X();
  delay(1000);
  homi_Y();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("MotorControl 1.2");
  delay(2000);
  lcd.clear();
}
void loop()
{
  if (modificar == true) {
    rotary(1, 100);
    cambiarValores(modificar, indexmenu);
  }
  if (modificar == false && submenu == false) rotary(1, 10);
  if (submenu_count == true)rotary(1, 3);

  if (submenu_modificar == true) {
    rotary(1, 100);
    sub_cambiarValores(submenu_modificar, indexmenu2);
  }

  if (rutina_Task == false) buttonControl();

}
void rotary(int ROTARYMIN, int ROTARYMAX) {
  encoder.tick();
  newPos = encoder.getPosition() * ROTARYSTEPS;
  if (newPos < ROTARYMIN) {
    encoder.setPosition(ROTARYMIN / ROTARYSTEPS);
    newPos = ROTARYMIN;
  } else if (newPos > ROTARYMAX) {
    encoder.setPosition(ROTARYMAX / ROTARYSTEPS);
    newPos = ROTARYMAX;
  }
  if (lastPos != newPos) {
    if (modificar == false && submenu == false) {
      indexmenu = newPos;
      menuDisplay(indexmenu);
    }
    if (submenu_count == true) {
      indexmenu2 = newPos;
      submenu_display(indexmenu2);

    }
    lastPos = newPos;
  }

}
void buttonControl() {
  int presionado = 0;
  if (digitalRead(ENCODER_SW) == LOW)
  {
    presionado = 1;
    delay(50);
  }
  if (digitalRead(ENCODER_SW) == HIGH && presionado == 1 && submenu == false)
  {
    check++;
    switch (check) {
      case 1:
        modificar = true;
        Serial.println("Modificando valores");
        break;
      case 2:
        modificar = false;
        Serial.println("Guardando los valores");
        check = 0;
        break;
    }
    presionado = 0;
  }
  if (digitalRead(ENCODER_SW) == HIGH && presionado == 1 && submenu == true)
  {
    check++;
    switch (check) {
      case 1:
        submenu_count = false;
        submenu_modificar = true;
        Serial.println("Sub menu ");
        // lcd.clear();
        break;
      case 2:
        submenu_count = true;
        submenu_modificar = false;;
        Serial.println("No Submenu");
        check = 0;
        //lcd.clear();
        break;
    }
    presionado = 0;
  }

}
void menuDisplay(int mode) {

  switch (mode) {
    case 1:
      lcd.clear();
      lcd.noCursor();
      lcd.setCursor(0, 0);
      lcd.print("Vel_X");
      lcd.setCursor(6, 0);
      lcd.print(String(velocidad_X) + " mm/s");
      lcd.setCursor(0, 1);
      lcd.print("Dis_X");
      lcd.setCursor(6, 1);
      lcd.print(String(distancia_X) + " mm");
      break;
    case 2:
      lcd.clear();
      lcd.noCursor();
      lcd.setCursor(0, 0);
      lcd.print("Velocidad X");
      break;
    case 3:
      lcd.clear();
      lcd.noCursor();
      lcd.setCursor(0, 0);
      lcd.print("Distancia X");
      break;
    case 4:
      lcd.clear();
      lcd.noCursor();
      lcd.setCursor(0, 0);
      lcd.print("Vel_Y");
      lcd.setCursor(6, 0);
      lcd.print(String(velocidad_Y) + " mm/s");
      lcd.setCursor(0, 1);
      lcd.print("Dis_Y");
      lcd.setCursor(6, 1);
      lcd.print(String(distancia_Y) + " mm");
      break;
    case 5:
      lcd.clear();
      lcd.noCursor();
      lcd.setCursor(0, 0);
      lcd.print("Velocidad Y");
      break;
    case 6:
      lcd.clear();
      lcd.noCursor();
      lcd.setCursor(0, 0);
      lcd.print("Distancia Y");
      break;
    case 7:
      lcd.clear();
      lcd.noCursor();
      lcd.setCursor(0, 0);
      lcd.print("PosM1 | ");
      lcd.setCursor(8, 0);
      lcd.print(String(stepper_X.getCurrentPositionInMillimeters()) + " mm");
      lcd.setCursor(0, 1);
      lcd.print("PosM2 | ");
      lcd.setCursor(8, 1);
      lcd.print(String(stepper_Y.getCurrentPositionInMillimeters()) + " mm");
      break;
    case 8:
      lcd.clear();
      lcd.noCursor();
      lcd.setCursor(0, 0);
      lcd.print("Home");
      break;
    case 9:
      lcd.clear();
      lcd.noCursor();
      lcd.setCursor(0, 0);
      lcd.print("Encender");
      break;
    case 10:
      lcd.clear();
      lcd.noCursor();
      lcd.setCursor(0, 0);
      lcd.print("Reset valores");
      break;
  }
}
void homi_X() {

  digitalWrite(MOTOR_X_ENABLE,LOW);
  lcd.clear();
  Serial.println("AutoHomeX");
  lcd.setCursor(0, 0);
  lcd.print("AutoHome X");

  if (stepper_X.moveToHomeInMillimeters(-1, 30, 380, LIMIT_X_SWITCH_PIN) == true)
  {
    Serial.println("HOMING X OK");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("AutoHome X OK");
    delay(1000);
    digitalWrite(MOTOR_X_ENABLE,HIGH);

  }
  else {
    lcd.clear();
    lcd.setCursor(0, 0);
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
  digitalWrite(MOTOR_Y_ENABLE,LOW);

  lcd.clear();
  Serial.println("AutoHomeY");
  lcd.setCursor(0, 0);
  lcd.print("AutoHome Y");

  if (stepper_Y.moveToHomeInMillimeters(-1, 30, 380, LIMIT_Y_SWITCH_PIN) == true)
  {
    Serial.println("HOMING Y OK");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("AutoHome Y OK");
    delay(1000);
    digitalWrite(MOTOR_Y_ENABLE,HIGH);

  }
  else {
    lcd.clear();
    lcd.setCursor(0, 0);
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
void cambiarValores(bool ok, int index) {

  if (ok == true) {
    if (index == 2)
    {
      //Modificador Velocidad motorX
      velocidad_X = newPos*10;
      lcd.setCursor(0, 1);
      lcd.cursor();
      lcd.setCursor(0, 1);
      lcd.print(String(velocidad_X) + " mm/s");
    }
    if (index == 3)
    {
      //Modificador distancia motorX
      distancia_X = newPos*10;
      lcd.setCursor(0, 1);
      lcd.cursor();
      lcd.setCursor(0, 1);
      lcd.print(String(distancia_X) + " mm");
    }
    if (index == 5)
    {
      //Modificador Velocidad motorY
      velocidad_Y = newPos*10;
      lcd.setCursor(0, 1);
      lcd.cursor();
      lcd.setCursor(0, 1);
      lcd.print(String(velocidad_Y) + " mm/s");
    }
    if (index == 6)
    {
      //Modificador distancia motorY
      distancia_Y = newPos*10;
      lcd.setCursor(0, 1);
      lcd.cursor();
      lcd.setCursor(0, 1);
      lcd.print(String(distancia_Y) + " mm");
    }
    if (index == 8)
    {
      //Autohome de ambos ejes
      homi_X();
      delay(1000);
      homi_Y();
      check = 0;
      modificar = false;
      lcd.clear();
      menuDisplay(1);
    }
    if (index == 9)
    {
      modificar = false;
      submenu_count = true;
      submenu = true;
      check = 0;
      // modificar = false;
      lcd.clear();
    }
    if (index == 10)
    {
      //reiniciar todos los datos.
      velocidad_X = 0;
      distancia_X = 0;
      velocidad_Y = 0;
      distancia_Y = 0;
      lcd.setCursor(0, 1);
      lcd.print("Valores en 0");
      delay(1000);
      check = 0;
      modificar = false;
      lcd.clear();
      menuDisplay(1);
    }
  }
  else {
    lcd.clear();
    lcd.noCursor();
  }
}
void submenu_display(int sub_mode) {
  switch (sub_mode) {
    case 1:
      lcd.clear();
      lcd.noCursor();
      lcd.setCursor(0, 0);
      lcd.print("Probar Rutina");
      lcd.setCursor(0, 1);
      lcd.print("1/3");
      break;
    case 2:
      lcd.clear();
      lcd.noCursor();
      lcd.setCursor(0, 0);
      lcd.print("Rutina Ciclo");
      lcd.setCursor(0, 1);
      lcd.print("2/3");
      break;
    case 3:
      lcd.clear();
      lcd.noCursor();
      lcd.setCursor(0, 0);
      lcd.print("Volver");
      lcd.setCursor(0, 1);
      lcd.print("3/3");
      break;
  }
}
void sub_cambiarValores(bool sub_ok, int sub_index) {

  if (sub_ok == true) {

    if (sub_index == 1) {      
        Test_rutina();
    }
    if (sub_index == 2) {

      rutina_Task = true;
    digitalWrite(MOTOR_X_ENABLE,LOW);
    digitalWrite(MOTOR_Y_ENABLE,LOW);
      Rutina_move();

    }
    if (sub_index == 3) {
      submenu_modificar = false;
      submenu_count = false;
      submenu = false;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Volviendo");
      modificar = false;
      delay(1000);
      menuDisplay(1);
      Serial.println("newPos");

    }
  }
}
void Test_rutina() {
  digitalWrite(MOTOR_X_ENABLE,LOW);
  digitalWrite(MOTOR_Y_ENABLE,LOW);
  stepper_X.setSpeedInMillimetersPerSecond(velocidad_X);
  stepper_Y.setSpeedInMillimetersPerSecond(velocidad_Y);
  lcd.setCursor(0, 1);
  lcd.print("Movimiento test");
  Serial.println("Movimiento test");
  stepper_X.setTargetPositionInMillimeters(distancia_X);
  stepper_Y.setTargetPositionInMillimeters(distancia_Y);
  while ((!stepper_X.motionComplete()) || (!stepper_Y.motionComplete())) {
    stepper_X.processMovement();
    stepper_Y.processMovement();
  }
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.println("   Completado   ");
  delay(1000);
  check = 0;
  lcd.clear();
  digitalWrite(MOTOR_X_ENABLE,HIGH);
  digitalWrite(MOTOR_Y_ENABLE,HIGH);
  submenu_count = true;
  submenu_modificar = false;
  submenu_display(indexmenu2);
}
void Rutina_move() {
  if (rutina_Task == true) {

    

    lcd.setCursor(0, 1);
    lcd.print("Rutina Loop");
    lcd.setCursor(0, 0);
    lcd.print("Click  Salir");
    Serial.println("Rutina Click");
    stepper_X.setSpeedInMillimetersPerSecond(velocidad_X);
    stepper_Y.setSpeedInMillimetersPerSecond(velocidad_Y);

    stepper_X.setDecelerationInMillimetersPerSecondPerSecond(velocidad_X * 2);
    stepper_Y.setDecelerationInMillimetersPerSecondPerSecond(velocidad_Y * 2);

    stepper_X.setAccelerationInMillimetersPerSecondPerSecond(velocidad_X * 2);
    stepper_Y.setAccelerationInMillimetersPerSecondPerSecond(velocidad_Y * 2);

    stepper_Y.setTargetPositionInMillimeters(distancia_X);
    stepper_X.setTargetPositionInMillimeters(distancia_Y);
    while ((!stepper_X.motionComplete()) || (!stepper_Y.motionComplete()))
    {
      //Rutina_button();
      stepper_X.processMovement();
      stepper_Y.processMovement();
    }
    stepper_Y.setTargetPositionInMillimeters(-distancia_X);
    stepper_X.setTargetPositionInMillimeters(-distancia_Y);
    while ((!stepper_X.motionComplete()) || (!stepper_Y.motionComplete()))
    {
      //Rutina_button();
      stepper_X.processMovement();
      stepper_Y.processMovement();
    }
  }else{
  //  lcd.clear();
  //.  submenu_display(indexmenu2);
  }
}
/*
void Rutina_button() {
  int presionado = 0;
  if (digitalRead(ENCODER_SW) == LOW)
  {
    presionado = 1;
    delay(50);
    rutina_Task = false;
    submenu_count = true;
    submenu_modificar = false;
    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print("Saliendo");
    lcd.clear();
    submenu_display(indexmenu2);
  }
}
*/
void finaltask(){
 
   lcd.clear();
   submenu_display(indexmenu2);
}