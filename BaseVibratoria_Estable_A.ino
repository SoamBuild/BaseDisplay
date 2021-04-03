/*
  Codigo de mesa vibratoria para conocer el funcionamiento de las funciones y del menu
  revisar readme.md ubicado en la rama main
  los pines del sistema se encuentran detallados en pin32.md en la rama main
  
  *****************************
  CODIGO PARA PANTALLA 16X2 I2C
  *****************************
*/
//Librerias
#include <LiquidCrystal_I2C.h>
#include "Arduino.h"
#include <RotaryEncoder.h>
#include <ESP_FlexyStepper.h>
#include <EasyButton.h>

//PINOUT MOTOR X
const int MOTOR_X_STEP_PIN = 16;
const int MOTOR_X_DIRECTION_PIN = 17;
const int MOTOR_X_ENABLE = 18;
const int LIMIT_X_SWITCH_PIN = 4;
//PINOUT MOTOR Y
const int MOTOR_Y_STEP_PIN = 33;
const int MOTOR_Y_DIRECTION_PIN = 25;
const int MOTOR_Y_ENABLE = 26;
const int LIMIT_Y_SWITCH_PIN = 19;
//Opcional LED
const int DEBUG_LED = 23;

//Objeto FlexyStepper X&Y
ESP_FlexyStepper stepper_X;
ESP_FlexyStepper stepper_Y;

//Display lcd 16x2 con interfaz I2C
//SCL 22 Pin esp32
//SDA 21 Pin esp32
LiquidCrystal_I2C lcd(0x27, 16, 2);

//Configuracion de rotaryEncoder
const int ENCODER_CLK = 13;
const int ENCODER_DT = 27;
const int ENCODER_SW = 32;
//Objeto rotary Encoder Libreria
RotaryEncoder encoder(ENCODER_CLK, ENCODER_DT);
//Objeto easy button Libreria
EasyButton button(ENCODER_SW);
//Variables para Encoder
#define ROTARYSTEPS 1
int newPos;
int lastPos = -1;
int check = 0;////Debounce encodervariable

//Navegacion de menu principal
bool modificar = false;
int indexmenu = 1;
//Navegacion de submenu_Encender
bool submenu = false;
bool submenu_count = false;
int indexmenu2;
bool submenu_modificar = false;
bool rutina_Task = false;
//Navegacion de submenu_multiplicador
bool submulti = false;
bool submulti_count = false;
int indexmenu3;
bool submulti_modificar = false;
int multiplicador = 1;
//Variables de velocidad y distancia de eje (X,Y)
int velocidad_X = 100;
int distancia_X = 100;
int velocidad_Y = 100;
int distancia_Y = 100;

//EasyButton callback
void onPressed()
{
  //Enter & Out Modificador de valores
  if (indexmenu > 3 && submenu == false)
  {
    check++;
    switch (check)
    {
      case 1:
        in_menu_1();
        break;
      case 2:
        out_menu_1();
        break;
    }
  }
  //Modificador de submenu encender
  if (submenu == true)
  {
    in_Menu_2_Modificar();
  }
  //Stop Rutina loop
  if (rutina_Task == true)
  {
    Serial.println("Salir Rutina");
    rutina_Task = false;
    out_Menu_2_modificar();
    stepper_X.emergencyStop();
    stepper_Y.emergencyStop();
    digitalWrite(MOTOR_X_ENABLE, HIGH);
    digitalWrite(MOTOR_Y_ENABLE, HIGH);
  }
  //Modificador de submenu multiplicador
  if (submulti == true)
  {
    menu_multiplicar();
  }
}

void setup()
{
  Serial.begin(115200);
  //Stepper Motor X, pin & Enable pin
  stepper_X.connectToPins(MOTOR_X_STEP_PIN, MOTOR_X_DIRECTION_PIN);
  pinMode(MOTOR_X_ENABLE, OUTPUT);
  digitalWrite(MOTOR_X_ENABLE, HIGH);
  //Stepper Motor Y, pin & Enable pin
  stepper_Y.connectToPins(MOTOR_Y_STEP_PIN, MOTOR_Y_DIRECTION_PIN);
  pinMode(MOTOR_Y_ENABLE, OUTPUT);
  digitalWrite(MOTOR_Y_ENABLE, HIGH);
  //PASOS POR MILIMETROS X & Y
  stepper_X.setStepsPerMillimeter(3.3 * 1);
  stepper_Y.setStepsPerMillimeter(3.3 * 1);
  //Finales de carrera
  pinMode(LIMIT_X_SWITCH_PIN, INPUT_PULLUP);
  pinMode(LIMIT_Y_SWITCH_PIN, INPUT_PULLUP);
  //Optional Led
  pinMode(DEBUG_LED, OUTPUT);
  //EasyButton begin && Callback
  button.begin();
  button.onPressed(onPressed);
  //Rutina de inicio.
  lcd.init();
  lcd.clear();
  lcd.backlight();
  //Opcional AutoHome X
  //homi_X();
  delay(1000);
  //Opcional AutoHome Y
  //homi_Y();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("MotorControl 1.3");
  delay(2000);
  lcd.clear();
}
void loop()
{
  //Navegar por las distintas pantallas(Menu principal)
  if (modificar == false && submenu == false)
    rotary(1, 11);
  //Modificar valores de Menu Principal
  if (modificar == true)
  {
    rotary(1, 100);
    cambiarValores(modificar, indexmenu);
  }
  //Mostrar pantallas de menu encender
  if (submenu_count == true)
    rotary(1, 5);
  //Seleccionar pantallas del menu encender 
  if (submenu_modificar == true)
  {
    sub_cambiarValores(submenu_modificar, indexmenu2);
  }
  //rutina_Task detiene boton y boton se ejecuta en el while de al rutina_Task
  if (rutina_Task == false)
    button.read();
  //Mostrar pantallas de multiplicador
  if (submulti_count == true)
    rotary(1, 4);
  //Seleccionar submenu pantalla
  if (submulti_modificar == true)
  {
    menu_multiplicador_modificar(submulti_modificar, indexmenu3);
  }
}
//Rotary espera un valor minimo y maximo 
void rotary(int ROTARYMIN, int ROTARYMAX)
{
  //Check valor minimo
  encoder.tick();
  newPos = encoder.getPosition() * ROTARYSTEPS;
   //Revisa el valor minimo
  if (newPos < ROTARYMIN)
  {
    encoder.setPosition(ROTARYMIN / ROTARYSTEPS);
    newPos = ROTARYMIN;
  }
  //Revisa el valor maximo
  else if (newPos > ROTARYMAX)
  {
    encoder.setPosition(ROTARYMAX / ROTARYSTEPS);
    newPos = ROTARYMAX;
  }
  //actualiza el valor comparando la ultima posicion con la nueva
  if (lastPos != newPos)
  {
    //llama a la pantalla del menu principal con menudisplay
    if (modificar == false && submenu == false)
    {
      indexmenu = newPos;
      menuDisplay(indexmenu);
    }
    //llama a las pantallas del submenu encender con submenu_display()
    if (submenu_count == true)
    {
      indexmenu2 = newPos;
      submenu_display(indexmenu2);
    }
    //llama a las pantallas del submenu multiplicar con sub_menu_multiplar()
    if (submulti_count == true)
    {
      indexmenu3 = newPos;
      sub_menu_multiplicador(indexmenu3);
    }
    lastPos = newPos;
  }
}

//Pantallas de menu principal
void menuDisplay(int mode)
{

  switch (mode)
  {
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
      lcd.print("Multiplicador");
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
      lcd.setCursor(0, 0);
      lcd.print("Home");
      break;
  }
}

//Acciones menu principal espera un valor verdadero y un numero mayor a 3
void cambiarValores(bool ok, int index)
{

  if (ok == true)
  {
    if (index == 4)
    {
      //Modificador Velocidad motorX
      velocidad_X = newPos * multiplicador;
      lcd.setCursor(0, 1);
      lcd.cursor();
      lcd.setCursor(0, 1);
      lcd.print(String(velocidad_X) + " mm/s");
    }
    if (index == 5)
    {
      //Modificador distancia motorX
      distancia_X = newPos * multiplicador;
      lcd.setCursor(0, 1);
      lcd.cursor();
      lcd.setCursor(0, 1);
      lcd.print(String(distancia_X) + " mm");
    }
    if (index == 6)
    {
      //Modificador Velocidad motorY
      velocidad_Y = newPos * multiplicador;
      lcd.setCursor(0, 1);
      lcd.cursor();
      lcd.setCursor(0, 1);
      lcd.print(String(velocidad_Y) + " mm/s");
    }
    if (index == 7)
    {
      //Modificador distancia motorY
      distancia_Y = newPos * multiplicador;
      lcd.setCursor(0, 1);
      lcd.cursor();
      lcd.setCursor(0, 1);
      lcd.print(String(distancia_Y) + " mm");
    }
    if (index == 8)
    {
      //Establecer multiplicador
      in_Menu2_in_multiplicador();
    }

    if (index == 9)
    {
      //Entrar al submenu
      in_menu_2();
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
    if (index == 11)
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
  }
}
//pantallas para submenu encender
void submenu_display(int sub_mode)
{
  switch (sub_mode)
  {
    case 1:
      lcd.clear();
      lcd.noCursor();
      lcd.setCursor(0, 0);
      lcd.print("Probar Rutina");
      lcd.setCursor(0, 1);
      lcd.print("1/5");
      break;
    case 2:
      lcd.clear();
      lcd.noCursor();
      lcd.setCursor(0, 0);
      lcd.print("Rutina Ciclo");
      lcd.setCursor(0, 1);
      lcd.print("2/5");
      break;
    case 3:
      lcd.clear();
      lcd.noCursor();
      lcd.setCursor(0, 0);
      lcd.print("Rutina en X");
      lcd.setCursor(0, 1);
      lcd.print("3/5");
      break;
    case 4:
      lcd.clear();
      lcd.noCursor();
      lcd.setCursor(0, 0);
      lcd.print("Rutina en Y");
      lcd.setCursor(0, 1);
      lcd.print("4/5");
      break;
    case 5:
      lcd.clear();
      lcd.noCursor();
      lcd.setCursor(0, 0);
      lcd.print("Volver");
      lcd.setCursor(0, 1);
      lcd.print("5/5");
      break;
  }
}

//acciones para sub menu encender espera un ok y un numero de pantalla
void sub_cambiarValores(bool sub_ok, int sub_index)
{

  if (sub_ok == true)
  {

    if (sub_index == 1)
    {
      //Probar la rutina
      Test_rutina();
    }
    if (sub_index == 2)
    {
      //LLama a la rutina en loop
      rutina_Task = true;
      digitalWrite(MOTOR_X_ENABLE, LOW);
      digitalWrite(MOTOR_Y_ENABLE, LOW);
      Rutina_move();
    }
    if (sub_index == 3)
    {
      //Llama a la rutina en X
      rutina_Task = true;
      digitalWrite(MOTOR_X_ENABLE, LOW);
      Rutina_move_X();
    }
    if (sub_index == 4)
    {
      //LLama a la rutina en Y
      rutina_Task = true;
      digitalWrite(MOTOR_Y_ENABLE, LOW);
      Rutina_move_Y();
    }
    if (sub_index == 5)
    {
      //Sale del submenu encender
      out_Menu_2();
    }
  }
}
//pantallas de submenu para multiplicador
void sub_menu_multiplicador(int display)
{
  switch (display)
  {
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
  }
}
//acciones para multiplicador espera un valor verdadero y un numero de pantalla 
void menu_multiplicador_modificar(bool state, int Display_accion)
{
  if (state == true)
  {
    if (Display_accion == 1)
    {
      //Multiplicas x5
      multiplicador = 5;
      lcd.setCursor(0, 1);
      lcd.print("Multi X5");
      out_menu2_multiplicador();
    }
    if (Display_accion == 2)
    {
      //Multiplicas x50
      multiplicador = 50;
      lcd.setCursor(0, 1);
      lcd.print("Multi X50");
      out_menu2_multiplicador();
    }
    if (Display_accion == 3)
    {
      //Multiplicas x100
      multiplicador = 100;
      lcd.setCursor(0, 1);
      lcd.print("Multi X100");
      out_menu2_multiplicador();
    }
    if (Display_accion == 4)
    {
      //Multiplicas x200
      multiplicador = 200;
      lcd.setCursor(0, 1);
      lcd.print("Multi X200");
      out_menu2_multiplicador();
    }
  }
}
//Entrar a modificar un valor (Vel x,y/Dis x,y/reinicar los valores)
void in_menu_1()
{
  modificar = true;
  Serial.println("Modificando valores Menu 1");
}
//Salir de modificar un valor
void out_menu_1()
{
  lcd.clear();
  menuDisplay(indexmenu);
  modificar = false;
  Serial.println("Guardando los valores");
  check = 0;
}
//Entrar al submenu encender 
void in_menu_2()
{
  Serial.println("IN_SUBMENU");
  //bloqueo contador menu principal;
  modificar = false;
  submenu = true;
  //activo el contador de menu 2
  submenu_count = true;
}
//salir del menu encender 
void out_Menu_2()
{
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
//Ejecutar una accion en submenu encender
void in_Menu_2_Modificar()
{
  Serial.println("Entrando en rutina submenu");
  //desactivo el contador de menu2
  submenu_count = false;
  //Activo Modificador de menu2
  submenu_modificar = true;
}
//Detener la accion y volver al submenu encender
void out_Menu_2_modificar()
{
  Serial.println("Rutina finalizada");
  //desactivo Modificador de menu2
  submenu_modificar = false;
  //Activo el contador de menu2
  submenu_count = true;
  lcd.clear();
  submenu_display(indexmenu2);
}
//Entrar al submenu multiplicador
void in_Menu2_in_multiplicador()
{
  modificar = false;
  submulti_count = true;
  submulti = true;
}
//Ejecutar accion en submenu multiplicador
void menu_multiplicar()
{
  submulti_count = false;
  submulti_modificar = true;
}
//Salir del submenu multiplicador
void out_menu2_multiplicador()
{
  modificar = false;
  check = 0;
  submulti_count = false;
  submulti = false;
  submulti_modificar = false;
  delay(1000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Volviendo");
  delay(1000);
  menuDisplay(1);
}
//Movimiento de prueba no loop
void Test_rutina()
{
  digitalWrite(MOTOR_X_ENABLE, LOW);
  digitalWrite(MOTOR_Y_ENABLE, LOW);
  stepper_X.setSpeedInMillimetersPerSecond(velocidad_X);
  stepper_Y.setSpeedInMillimetersPerSecond(velocidad_Y);
  lcd.setCursor(0, 1);
  lcd.print("Movimiento test");
  Serial.println("Movimiento test");
  stepper_X.setTargetPositionInMillimeters(distancia_X);
  stepper_Y.setTargetPositionInMillimeters(distancia_Y);
  while ((!stepper_X.motionComplete()) || (!stepper_Y.motionComplete()))
  {
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
}

//movimiento en loop X&Y necesita dos valores y se ejecuta en loop
void Rutina_move()
{
  if (rutina_Task == true)
  {
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

    stepper_Y.setTargetPositionInMillimeters(distancia_Y);
    stepper_X.setTargetPositionInMillimeters(distancia_X);
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
//movimiento en loop X necesita un valores y se ejecuta en loop
void Rutina_move_X()
{
  if (rutina_Task == true)
  {
    lcd.setCursor(0, 1);
    lcd.print("Rutina en  X");
    lcd.setCursor(0, 0);
    lcd.print("Click Salir");
    Serial.println("Rutina Click");
    stepper_X.setSpeedInMillimetersPerSecond(velocidad_X);

    stepper_X.setDecelerationInMillimetersPerSecondPerSecond(velocidad_X * 3);

    stepper_X.setAccelerationInMillimetersPerSecondPerSecond(velocidad_X * 3);

    stepper_X.setTargetPositionInMillimeters(distancia_X);
    while (!stepper_X.motionComplete())
    {
      button.read();
      stepper_X.processMovement();
    }
    stepper_X.setTargetPositionInMillimeters(0);

    while (!stepper_X.motionComplete())
    {
      button.read();
      stepper_X.processMovement();
    }
  }
}
//movimiento en loop Y necesita un valores y se ejecuta en loop
void Rutina_move_Y()
{
  if (rutina_Task == true)
  {
    lcd.setCursor(0, 1);
    lcd.print("Rutina en  Y");
    lcd.setCursor(0, 0);
    lcd.print("Click Salir");
    Serial.println("Rutina Click");
    stepper_Y.setSpeedInMillimetersPerSecond(velocidad_Y);
    stepper_Y.setDecelerationInMillimetersPerSecondPerSecond(velocidad_Y * 3);
    stepper_Y.setAccelerationInMillimetersPerSecondPerSecond(velocidad_Y * 3);
    stepper_Y.setTargetPositionInMillimeters(distancia_Y);
    while (!stepper_Y.motionComplete())
    {
      button.read();
      stepper_Y.processMovement();
    }
    stepper_Y.setTargetPositionInMillimeters(0);
    while (!stepper_Y.motionComplete())
    {
      button.read();
      stepper_Y.processMovement();
    }
  }
}

//AutoHome X
void homi_X()
{

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
  else
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("AutoHome X Error");
    Serial.println("Error");
    while (true)
    {
      digitalWrite(DEBUG_LED, HIGH);
      delay(50);
      digitalWrite(DEBUG_LED, LOW);
      delay(50);
    }
    Serial.println("Error eje X");
  }
}

//AutoHome Y
void homi_Y()
{
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
  else
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("AutoHome Y Error");
    Serial.println("Error");
    while (true)
    {
      digitalWrite(DEBUG_LED, HIGH);
      delay(50);
      digitalWrite(DEBUG_LED, LOW);
      delay(50);
    }
    Serial.println("Error eje Y");
  }
}