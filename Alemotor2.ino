/*
  Codigo de mesa vibratoria para conocer el funcionamiento de las funciones y del menu
  revisar readme.md ubicado en la rama main
  los pines del sistema se encuentran detallados en pin32.md en la rama main
*/


#include <LiquidCrystal_I2C.h>
#include "Arduino.h"
#include <RotaryEncoder.h>
#include <ESP_FlexyStepper.h>
#include <EasyButton.h>

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
EasyButton button(ENCODER_SW);

#define ROTARYSTEPS 1
int newPos;
int lastPos = -1;
//const int pulsador = 32;

//Debounce encodervariable
int check = 0;

//Navegacion de menu
bool modificar = false;
int indexmenu = 1;
//Navegacion de submenu_Encender
bool submenu = false;
bool submenu_count = false;
int indexmenu2;
bool submenu_modificar = false;
bool rutina_Task = false;
//Navegacion de submenu_multiplicador
bool submulti=false;
bool submulti_count = false;
int indexmenu3;
bool submulti_modificar=false;
int multiplicador= 1;
//Variables de velocidad y distancia de eje (X,Y)
int velocidad_X = 1500;
int distancia_X = 25;
int velocidad_Y = 1500;
int distancia_Y = 25;


void onPressed()
{
  if (indexmenu > 3 && submenu == false)
  {
    check++;
    switch (check) {
      case 1:
        in_menu_1();
        break;
      case 2:
        out_menu_1();
        break;
    }
  }
  if (submenu == true)
  {
    in_Menu_2_Modificar();
  }
  if (rutina_Task == true){
    Serial.println("Salir Rutina");
    rutina_Task = false;
    out_Menu_2_modificar();
    stepper_X.emergencyStop();
    stepper_Y.emergencyStop();
    digitalWrite(MOTOR_X_ENABLE, HIGH);
    digitalWrite(MOTOR_Y_ENABLE, HIGH);
  }
   if (submulti == true)
  {
    menu_multiplicar();
  }
  
}
void setup()
{

  Serial.begin(115200);
  stepper_X.connectToPins(MOTOR_X_STEP_PIN, MOTOR_X_DIRECTION_PIN);
  pinMode(MOTOR_X_ENABLE, OUTPUT);
  digitalWrite(MOTOR_X_ENABLE, HIGH);
  stepper_Y.connectToPins(MOTOR_Y_STEP_PIN, MOTOR_Y_DIRECTION_PIN);
  pinMode(MOTOR_Y_ENABLE, OUTPUT);
  digitalWrite(MOTOR_Y_ENABLE, HIGH);
  stepper_X.setStepsPerMillimeter(3.3 * 1);   // 1x microstepping
  stepper_Y.setStepsPerMillimeter(3.3 * 1);    // 1x microstepping
  pinMode(LIMIT_X_SWITCH_PIN, INPUT_PULLUP);
  pinMode(LIMIT_Y_SWITCH_PIN, INPUT_PULLUP);
  pinMode(DEBUG_LED, OUTPUT);
  button.begin();
  button.onPressed(onPressed);
  lcd.init();
  lcd.clear();
  lcd.backlight();
  //homi_X();
  delay(1000);
  //homi_Y();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("MotorControl 1.2");
  delay(2000);
  lcd.clear();
}
void loop()
{

  if (modificar == false && submenu == false) rotary(1, 11);

  if (modificar == true) {
    rotary(1, 100);
    cambiarValores(modificar, indexmenu);
  }

  if (submenu_count == true)rotary(1, 3);

  if (submenu_modificar == true) {
    rotary(1, 100);
    sub_cambiarValores(submenu_modificar, indexmenu2);
  }

  if (rutina_Task == false) button.read();

  if(submulti_count==true)rotary(1,4);

  if(submulti_modificar==true){
    menu_multiplicador_modificar(submulti_modificar,indexmenu3);
  }
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
    if(submulti_count==true){
      indexmenu3 = newPos;
      sub_menu_multiplicador(indexmenu3);
    }
    lastPos = newPos;
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
      lcd.print("Vel_Y");
      lcd.setCursor(6, 0);
      lcd.print(String(velocidad_Y) + " mm/s");
      lcd.setCursor(0, 1);
      lcd.print("Dis_Y");
      lcd.setCursor(6, 1);
      lcd.print(String(distancia_Y) + " mm");
      break;
    case 3:
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
    case 4:
      lcd.clear();
      lcd.noCursor();
      lcd.setCursor(0, 0);
      lcd.print("Velocidad X");
      break;
    case 5:
      lcd.clear();
      lcd.noCursor();
      lcd.setCursor(0, 0);
      lcd.print("Distancia X");
      break;
    case 6:
      lcd.clear();
      lcd.noCursor();
      lcd.setCursor(0, 0);
      lcd.print("Velocidad Y");
      break;
    case 7:
      lcd.clear();
      lcd.noCursor();
      lcd.setCursor(0, 0);
      lcd.print("Distancia Y");
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
    case 11:
      lcd.clear();
      lcd.noCursor();
      lcd.setCursor(0,0);
      lcd.print("Multiplicador");
  }
}
void homi_X() {

  digitalWrite(MOTOR_X_ENABLE, LOW);
  lcd.clear();
  Serial.println("AutoHomeX");
  lcd.setCursor(0, 0);
  lcd.print("AutoHome X");

  if (stepper_X.moveToHomeInMillimeters(-1, 100, 380, LIMIT_X_SWITCH_PIN) == true)
  {
    Serial.println("HOMING X OK");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("AutoHome X OK");
    delay(1000);
    digitalWrite(MOTOR_X_ENABLE, HIGH);

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
  digitalWrite(MOTOR_Y_ENABLE, LOW);

  lcd.clear();
  Serial.println("AutoHomeY");
  lcd.setCursor(0, 0);
  lcd.print("AutoHome Y");

  if (stepper_Y.moveToHomeInMillimeters(-1, 100, 380, LIMIT_Y_SWITCH_PIN) == true)
  {
    Serial.println("HOMING Y OK");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("AutoHome Y OK");
    delay(1000);
    digitalWrite(MOTOR_Y_ENABLE, HIGH);

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
    if (index == 4)
    {
      //Modificador Velocidad motorX
      velocidad_X = newPos * 10;
      lcd.setCursor(0, 1);
      lcd.cursor();
      lcd.setCursor(0, 1);
      lcd.print(String(velocidad_X) + " mm/s");
    }
    if (index == 5)
    {
      //Modificador distancia motorX
      distancia_X = newPos * 10;
      lcd.setCursor(0, 1);
      lcd.cursor();
      lcd.setCursor(0, 1);
      lcd.print(String(distancia_X) + " mm");
    }
    if (index == 6)
    {
      //Modificador Velocidad motorY
      velocidad_Y = newPos * 10;
      lcd.setCursor(0, 1);
      lcd.cursor();
      lcd.setCursor(0, 1);
      lcd.print(String(velocidad_Y) + " mm/s");
    }
    if (index == 7)
    {
      //Modificador distancia motorY
      distancia_Y = newPos * 10;
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
      in_menu_2();
      // modificar = false;
      // submenu_count = true;
      // submenu = true;
      // check = 0;
      // // modificar = false;
      // lcd.clear();
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
    if(index == 11){
      in_Menu2_in_multiplicador();
    }
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
      digitalWrite(MOTOR_X_ENABLE, LOW);
      digitalWrite(MOTOR_Y_ENABLE, LOW);
      Rutina_move();
    }
    if (sub_index == 3) {
      out_Menu_2();
    }
  }
}
void sub_menu_multiplicador(int display){
  switch (display) {
    case 1:
      lcd.clear();
      lcd.noCursor();
      lcd.setCursor(0, 0);
      lcd.print("X5");
      lcd.setCursor(0, 1);
      lcd.print("1/4");
      break;
    case 2:
      lcd.clear();
      lcd.noCursor();
      lcd.setCursor(0, 0);
      lcd.print("X50");
      lcd.setCursor(0, 1);
      lcd.print("2/4");
      break;
  case 3:
      lcd.clear();
      lcd.noCursor();
      lcd.setCursor(0, 0);
      lcd.print("X100");
      lcd.setCursor(0, 1);
      lcd.print("3/4");
      break;
  case 4:
      lcd.clear();
      lcd.noCursor();
      lcd.setCursor(0, 0);
      lcd.print("X200");
      lcd.setCursor(0, 1);
      lcd.print("4/4");
      break;
}}

void menu_multiplicador_modificar(bool state,int estado){
  if(state ==true){
    if(estado == 1){
      multiplicador = 5;
      lcd.setCursor(0, 1);
      lcd.print("Multi X5");
      out_menu2_multiplicador();
      
      
    }
  }
}

void Test_rutina() {
  digitalWrite(MOTOR_X_ENABLE, LOW);
  digitalWrite(MOTOR_Y_ENABLE, LOW);
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
  digitalWrite(MOTOR_X_ENABLE, HIGH);
  digitalWrite(MOTOR_Y_ENABLE, HIGH);
  out_Menu_2_modificar();

  //  submenu_count = true;
  //  submenu_modificar = false;
  //  submenu_display(indexmenu2);
}
void Rutina_move() {
  if (rutina_Task == true) {
    lcd.setCursor(0, 1);
    lcd.print("Rutina Loop");
    lcd.setCursor(0, 0);
    lcd.print("Click Salir");
    Serial.println("Rutina Click");
    stepper_X.setSpeedInMillimetersPerSecond(velocidad_X);
    stepper_Y.setSpeedInMillimetersPerSecond(velocidad_Y);

    stepper_X.setDecelerationInMillimetersPerSecondPerSecond(velocidad_X * 3);
    stepper_Y.setDecelerationInMillimetersPerSecondPerSecond(velocidad_Y * 3);

    stepper_X.setAccelerationInMillimetersPerSecondPerSecond(velocidad_X * 3);
    stepper_Y.setAccelerationInMillimetersPerSecondPerSecond(velocidad_Y * 3);

    stepper_Y.setTargetPositionInMillimeters(distancia_X);
    stepper_X.setTargetPositionInMillimeters(distancia_Y);
    while ((!stepper_X.motionComplete()) || (!stepper_Y.motionComplete()))
    {
      button.read();
      stepper_X.processMovement();
      stepper_Y.processMovement();
    }
    stepper_Y.setTargetPositionInMillimeters(0);
    stepper_X.setTargetPositionInMillimeters(0);

    while ((!stepper_X.motionComplete()) || (!stepper_Y.motionComplete()))
    {
      button.read();
      stepper_X.processMovement();
      stepper_Y.processMovement();
    }
  }
}
//IN && OUT Menu(Value,Home,Reset)
void in_menu_1() {
  modificar = true;
  Serial.println("Modificando valores Menu 1");
}
void out_menu_1() {
  lcd.clear();
  menuDisplay(indexmenu);
  modificar = false;
  Serial.println("Guardando los valores");
  check = 0;
}

//IN && OUT submenu (PROBAR,ENCENDER,VOLVER)
void in_menu_2() {
  Serial.println("IN_SUBMENU");
  //bloqueo contador menu principal;
  modificar = false;
  submenu = true;
  //activo el contador de menu 2
  submenu_count = true;
}
void out_Menu_2() {
  Serial.println("OUT_SUBMENU");
  modificar = false;
  check = 0;
  submenu = false;
  submenu_count = false;
  submenu_modificar = false;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Volviendo");
  delay(1000);
  menuDisplay(1);
}
void in_Menu_2_Modificar() {
  Serial.println("Entrando en rutina submenu");
  //desactivo el contador de menu2
  submenu_count = false;
  //Activo Modificador de menu2
  submenu_modificar = true;
}
void out_Menu_2_modificar() {
  Serial.println("Rutina finalizada");
  //desactivo Modificador de menu2
  submenu_modificar = false;
  //Activo el contador de menu2
  submenu_count = true;
  lcd.clear();
  submenu_display(indexmenu2);
}
void in_Menu2_in_multiplicador(){
  modificar=false;
  submulti_count=true;
  submulti=true;
}
void menu_multiplicar(){
  //submulti=false;
  submulti_count = false;
  submulti_modificar=true;
}
void out_menu2_multiplicador(){
  modificar=false;
  check=0;
  submulti_count=false;
  submulti=false;
  submulti_modificar=false;
  delay(1000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Volviendo");
  delay(1000);
  menuDisplay(1);
  }
