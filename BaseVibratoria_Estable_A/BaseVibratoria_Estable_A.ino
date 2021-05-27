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
#include "WiFi.h"
#include "ESPAsyncWebServer.h"
#include "SPIFFS.h"

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

//Debounce encodervariable
int check = 0;

//Navegacion de menu
bool menu_modificar = false;
int menu_indexmenu = 1;
//Navegacion de submenu_Encender
bool submenu_encender = false;
bool submenu_encender_count = false;
int submenu_encender_indexmenu2;
bool submenu_encender_modificar = false;
bool submenu_encender_rutinatask = false;
//Navegacion de submenu_multiplicador
bool submenu_multiplicador = false;
bool submenu_multiplicador_count = false;
int submenu_multiplicador_indexmenu3;
bool submenu_multiplicador_modificar = false;
int submenu_multiplicador_valuemultiplicador = 1;
//Variables de velocidad y distancia de eje (X,Y)
int velocidad_X = 100;
int distancia_X = 100;
int velocidad_Y = 100;
int distancia_Y = 100;

//WebServer control
const char *PARAM_INPUT = "value";
const char *ssid = "MotorControl";
const char *password = "MotorControlV1";
AsyncWebServer server(80);
const char separator = ',';
bool handleweb = false;

void onPressed()
{
  if (menu_indexmenu > 3 && submenu_encender == false)
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
  if (submenu_encender == true)
  {
    in_Menu_2_Modificar();
  }
  if (submenu_encender_rutinatask == true)
  {
    rutina_out();
  }
  if (submenu_multiplicador == true)
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
  stepper_X.setStepsPerMillimeter(3.3 * 1); // 1x microstepping
  stepper_Y.setStepsPerMillimeter(3.3 * 1); // 1x microstepping
  pinMode(LIMIT_X_SWITCH_PIN, INPUT_PULLUP);
  pinMode(LIMIT_Y_SWITCH_PIN, INPUT_PULLUP);
  pinMode(DEBUG_LED, OUTPUT);
  button.begin();
  button.onPressed(onPressed);

  if (!SPIFFS.begin(true))
  {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }
  WiFi.softAP(ssid, password);
  Serial.println();
  Serial.print("IP address: ");
  Serial.println(WiFi.softAPIP());

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(SPIFFS, "/index.html", String(), false);
  });
  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(SPIFFS, "/style.css", "text/css");
  });
  server.on("/test.js", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(SPIFFS, "/test.js", "text/javascript");
  });
  server.on("/Move", HTTP_GET, [](AsyncWebServerRequest *request) {
    String inputMessage;
    if (request->hasParam(PARAM_INPUT))
    {
      inputMessage = request->getParam(PARAM_INPUT)->value();
    }
    handleweb = true;
    Serial.println(inputMessage);
    fixdata(1, inputMessage, 4);
    request->send(200, "text/plain", "OK");
  });
  server.on("/MoveX", HTTP_GET, [](AsyncWebServerRequest *request) {
    String inputMessage;

    if (request->hasParam(PARAM_INPUT))
    {
      inputMessage = request->getParam(PARAM_INPUT)->value();
    }
    fixdata(2, inputMessage, 2);
    request->send(200, "text/plain", "OK");
  });
  server.on("/MoveY", HTTP_GET, [](AsyncWebServerRequest *request) {
    String inputMessage;
    if (request->hasParam(PARAM_INPUT))
    {
      inputMessage = request->getParam(PARAM_INPUT)->value();
    }
    Serial.println(inputMessage);
    fixdata(3, inputMessage, 2);
    request->send(200, "text/plain", "OK");
  });
  server.on("/Stop", HTTP_GET, [](AsyncWebServerRequest *request) {
    String inputMessage;

    if (request->hasParam(PARAM_INPUT))
    {
      inputMessage = request->getParam(PARAM_INPUT)->value();
    }
    Serial.println(inputMessage);
    rutina_out();
    request->send(200, "text/plain", "OK");
  });

  lcd.init();
  lcd.clear();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("MotorControl 1.3");
  lcd.setCursor(0, 1);
  lcd.print("Server On");
  delay(2000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Red " + String(ssid));
  lcd.setCursor(0, 1);
  lcd.print(WiFi.softAPIP());
  delay(2000);
  lcd.clear();
  server.begin();
}
void fixdata(int tag, String values, int ndatos)
{
  int data[ndatos];

  for (int i = 0; i < ndatos; i++)
  {
    int index = values.indexOf(separator);
    data[i] = values.substring(0, index).toInt();
    values = values.substring(index + 1);
    Serial.println(data[i]);
  }
  if (tag == 1)
  {
    velocidad_X = data[0];
    distancia_X = data[1];
    velocidad_Y = data[2];
    distancia_Y = data[3];
    in_menu_2();
    submenu_encender_modificar = true;
    submenu_encender_indexmenu2 = 2;
  }
  if (tag == 2)
  {
    velocidad_X = data[0];
    distancia_X = data[1];
    in_menu_2();
    submenu_encender_modificar = true;
    submenu_encender_indexmenu2 = 3;
  }
  if (tag == 3)
  {
    velocidad_Y = data[0];
    distancia_Y = data[1];
    in_menu_2();
    submenu_encender_modificar = true;
    submenu_encender_indexmenu2 = 4;
  }
}
void loop()
{

  if (menu_modificar == false && submenu_encender == false)
    rotary(1, 11);

  if (menu_modificar == true)
  {
    rotary(1, 100);
    cambiarValores(menu_modificar, menu_indexmenu);
  }

  if (submenu_encender_count == true)
    rotary(1, 5);

  if (submenu_encender_modificar == true)
  {
    //rotary(1, 100);
    sub_cambiarValores(submenu_encender_modificar, submenu_encender_indexmenu2);
  }

  if (submenu_encender_rutinatask == false)
    button.read();

  if (submenu_multiplicador_count == true)
    rotary(1, 4);

  if (submenu_multiplicador_modificar == true)
  {
    menu_multiplicador_modificar(submenu_multiplicador_modificar, submenu_multiplicador_indexmenu3);
  }
}
void rotary(int ROTARYMIN, int ROTARYMAX)
{
  encoder.tick();
  newPos = encoder.getPosition() * ROTARYSTEPS;
  if (newPos < ROTARYMIN)
  {
    encoder.setPosition(ROTARYMIN / ROTARYSTEPS);
    newPos = ROTARYMIN;
  }
  else if (newPos > ROTARYMAX)
  {
    encoder.setPosition(ROTARYMAX / ROTARYSTEPS);
    newPos = ROTARYMAX;
  }
  if (lastPos != newPos)
  {
    if (menu_modificar == false && submenu_encender == false)
    {
      menu_indexmenu = newPos;
      menuDisplay(menu_indexmenu);
    }
    if (submenu_encender_count == true)
    {
      submenu_encender_indexmenu2 = newPos;
      submenu_display(submenu_encender_indexmenu2);
    }
    if (submenu_multiplicador_count == true)
    {
      submenu_multiplicador_indexmenu3 = newPos;
      sub_menu_multiplicador(submenu_multiplicador_indexmenu3);
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
    lcd.setCursor(0, 0);
    lcd.print("Velocidad X");
    break;
  case 5:
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Distancia X");
    break;
  case 6:
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Velocidad Y");
    break;
  case 7:
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Distancia Y");
    break;
  case 8:
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("submenu_multiplicador_valuemultiplicador");
    break;

  case 9:
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Encender");
    break;
  case 10:
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Reset valores");
    break;
  case 11:
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Home");
    break;
  }
}

//Acciones menu principal
void cambiarValores(bool ok, int index)
{

  if (ok == true)
  {
    if (index == 4)
    {
      //Modificador Velocidad motorX
      velocidad_X = newPos * submenu_multiplicador_valuemultiplicador;
      lcd.setCursor(0, 1);
      lcd.cursor();
      lcd.setCursor(0, 1);
      lcd.print(String(velocidad_X) + " mm/s");
    }
    if (index == 5)
    {
      //Modificador distancia motorX
      distancia_X = newPos * submenu_multiplicador_valuemultiplicador;
      lcd.setCursor(0, 1);
      lcd.cursor();
      lcd.setCursor(0, 1);
      lcd.print(String(distancia_X) + " mm");
    }
    if (index == 6)
    {
      //Modificador Velocidad motorY
      velocidad_Y = newPos * submenu_multiplicador_valuemultiplicador;
      lcd.setCursor(0, 1);
      lcd.cursor();
      lcd.setCursor(0, 1);
      lcd.print(String(velocidad_Y) + " mm/s");
    }
    if (index == 7)
    {
      //Modificador distancia motorY
      distancia_Y = newPos * submenu_multiplicador_valuemultiplicador;
      lcd.setCursor(0, 1);
      lcd.cursor();
      lcd.setCursor(0, 1);
      lcd.print(String(distancia_Y) + " mm");
    }
    if (index == 8)
    {
      in_Menu2_in_multiplicador();
    }

    if (index == 9)
    {
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
      menu_modificar = false;
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
      menu_modificar = false;
      lcd.clear();
      menuDisplay(1);
    }
  }

  //pantallas para submenu_encender encender
}
void submenu_display(int sub_mode)
{
  switch (sub_mode)
  {
  case 1:
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Probar Rutina");
    lcd.setCursor(0, 1);
    lcd.print("1/5");
    break;
  case 2:
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Rutina Ciclo");
    lcd.setCursor(0, 1);
    lcd.print("2/5");
    break;
  case 3:
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Rutina en X");
    lcd.setCursor(0, 1);
    lcd.print("3/5");
    break;
  case 4:
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Rutina en Y");
    lcd.setCursor(0, 1);
    lcd.print("4/5");
    break;
  case 5:
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Volver");
    lcd.setCursor(0, 1);
    lcd.print("5/5");
    break;
  }
}

//acciones para sub menu encender
void sub_cambiarValores(bool sub_ok, int sub_index)
{

  if (sub_ok == true)
  {

    if (sub_index == 1)
    {
      Test_rutina();
    }
    if (sub_index == 2)
    {
      submenu_encender_rutinatask = true;
      digitalWrite(MOTOR_X_ENABLE, LOW);
      digitalWrite(MOTOR_Y_ENABLE, LOW);
      Rutina_move();
    }
    if (sub_index == 3)
    {
      submenu_encender_rutinatask = true;
      digitalWrite(MOTOR_X_ENABLE, LOW);
      Rutina_move_X();
    }
    if (sub_index == 4)
    {
      submenu_encender_rutinatask = true;
      digitalWrite(MOTOR_Y_ENABLE, LOW);
      Rutina_move_Y();
    }
    if (sub_index == 5)
    {
      out_Menu_2();
    }
  }
}
//pantallas de submenu_encender para submenu_multiplicador_valuemultiplicador
void sub_menu_multiplicador(int display)
{
  switch (display)
  {
  case 1:
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("X5");
    lcd.setCursor(0, 1);
    lcd.print("1/4");
    break;
  case 2:
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("X50");
    lcd.setCursor(0, 1);
    lcd.print("2/4");
    break;
  case 3:
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("X100");
    lcd.setCursor(0, 1);
    lcd.print("3/4");
    break;
  case 4:
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("X200");
    lcd.setCursor(0, 1);
    lcd.print("4/4");
    break;
  }
}
//acciones para submenu_multiplicador_valuemultiplicador
void menu_multiplicador_modificar(bool state, int estado)
{
  if (state == true)
  {
    if (estado == 1)
    {
      submenu_multiplicador_valuemultiplicador = 5;
      lcd.setCursor(0, 1);
      lcd.print("Multi X5");
      out_menu2_multiplicador();
    }
    if (estado == 2)
    {
      submenu_multiplicador_valuemultiplicador = 50;
      lcd.setCursor(0, 1);
      lcd.print("Multi X50");
      out_menu2_multiplicador();
    }
    if (estado == 3)
    {
      submenu_multiplicador_valuemultiplicador = 100;
      lcd.setCursor(0, 1);
      lcd.print("Multi X100");
      out_menu2_multiplicador();
    }
    if (estado == 4)
    {
      submenu_multiplicador_valuemultiplicador = 200;
      lcd.setCursor(0, 1);
      lcd.print("Multi X200");
      out_menu2_multiplicador();
    }
  }
}
//IN && OUT Menu(Value,Home,Reset)
void in_menu_1()
{
  menu_modificar = true;
  Serial.println("Modificando valores Menu 1");
}
void out_menu_1()
{
  lcd.clear();
  menuDisplay(menu_indexmenu);
  menu_modificar = false;
  Serial.println("Guardando los valores");
  check = 0;
}

//IN && OUT submenu_encender (PROBAR,ENCENDER,VOLVER)
void in_menu_2()
{
  Serial.println("IN_SUBMENU");
  //bloqueo contador menu principal;
  menu_modificar = false;
  submenu_encender = true;
  //activo el contador de menu 2
  submenu_encender_count = true;
}

void out_Menu_2()
{
  Serial.println("OUT_SUBMENU");
  menu_modificar = false;
  check = 0;
  submenu_encender = false;
  submenu_encender_count = false;
  submenu_encender_modificar = false;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Volviendo");
  delay(1000);
  menuDisplay(1);
}
void in_Menu_2_Modificar()
{
  Serial.println("Entrando en rutina submenu_encender");
  //desactivo el contador de menu2
  submenu_encender_count = false;
  //Activo Modificador de menu2
  submenu_encender_modificar = true;
}
void out_Menu_2_modificar()
{
  Serial.println("Rutina finalizada");
  //desactivo Modificador de menu2
  submenu_encender_modificar = false;
  //Activo el contador de menu2
  submenu_encender_count = true;
  lcd.clear();
  submenu_display(submenu_encender_indexmenu2);
}
void in_Menu2_in_multiplicador()
{
  menu_modificar = true;
  submenu_multiplicador_count = true;
  submenu_multiplicador = true;
}
void menu_multiplicar()
{
  //submenu_multiplicador=false;
  submenu_multiplicador_count = false;
  submenu_multiplicador_modificar = true;
}
void out_menu2_multiplicador()
{
  menu_modificar = false;
  check = 0;
  submenu_multiplicador_count = false;
  submenu_multiplicador = false;
  submenu_multiplicador_modificar = false;
  delay(1000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Volviendo");
  delay(1000);
  menuDisplay(1);
}
void rutina_out()
{
  lcd.clear();
  Serial.println("Salir Rutina");
  submenu_encender_rutinatask = false;
  out_Menu_2_modificar();
  stepper_X.emergencyStop();
  stepper_Y.emergencyStop();
  digitalWrite(MOTOR_X_ENABLE, HIGH);
  digitalWrite(MOTOR_Y_ENABLE, HIGH);
}

//Movimiento de prueba
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

//movimiento en loop
void Rutina_move()
{
  lcd.clear();
  if (submenu_encender_rutinatask == true)
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
void Rutina_move_X()
{
   lcd.clear();
  if (submenu_encender_rutinatask == true)
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
void Rutina_move_Y()
{
  lcd.clear();
  if (submenu_encender_rutinatask == true)
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

  if (stepper_X.moveToHomeInMillimeters(-1, 100, 500, LIMIT_X_SWITCH_PIN) == true)
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

  if (stepper_Y.moveToHomeInMillimeters(-1, 100, 500, LIMIT_Y_SWITCH_PIN) == true)
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