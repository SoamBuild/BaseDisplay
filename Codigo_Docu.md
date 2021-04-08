# THE CODE 

Para entender el funcionamiento de cada funcion y de las pantallas del menu, se recomienda leer el siguiente contenido, donde se detallan las funciones utilizadas y que valores esperan, las pantallas que se muestran y cual funcion dentro del codigo, las rutinas que se realizan en cada pantalla operativa.

# Contenido
1. [Pantalla](#Pantalla_Funciones)
2. [Funciones](#Funciones_Utilizadas)
3. [Rutinas](#Rutinas_Menu)
4. [Fourth Example](#fourth-examplehttpwwwfourthexamplecom)

## Pantalla_Funciones

En esta seccion se explican que hace cada pantalla y cual de estas son realmente operativas.

### Pantallas
1. [Pantallas(16x2)](#Display_16x2)
2. [Pantallas(20x4)](#Display_20x4)

### **Display16x2**


El display de 16x2 I2C tiene 11 pantallas de las cuales 3 son de informacion de la posicion y los valores actuales de los ejes.

Las 8 pantallas restantes son operativas es decir puedes modificar valores, hacer autohome, cambiar multiplicadores entre otras operaciones. 

Adicionalmente puedes encontrar dos submenu el primero se trata del menu encender que agrega 5 pantallas y el menu de multiplicador que agrega 4 pantallas.

- 1 Valores eje X (Informacion).

En esta pantalla puedes ver la velocidad y la distancia actual del eje Y

- 2 Valores eje Y (Informacion).

En esta pantallas puedes ver la velocidad y la distancia actual del eje Y

- 3 Posicion actual de ejes X | Y

Como la libreria trabaja con movimientos absolutos es decir considera la posicion actual de los ejes, en esta pantallas puedes conocer dicho valor para ambos ejes.

- 4 Velocidad de eje X

Haciendo un click entras y puedes modificar con el enconder el valor de la velocidad del eje X, y con un segundo click puedes volver al menu principal
- 5 Distancia de eje X

haciendo un click entras y modificas las distancia actual del eje X, y haciendo un segundo click guardas el valor actual y regresas al menu principal.

- 6 Velocidad de eje Y

Haciendo un click entras y puedes modificar con el enconder el valor de la velocidad del eje Y, y con un segundo click puedes volver al menu principal
- 7 Distancia de eje Y

haciendo un click entras y modificas las distancia actual del eje Y, y haciendo un segundo click guardas el valor actual y regresas al menu principal.

- 8 Multiplicador

una nueva funcion que hace mas facil modificar los valores, ya que con esto puedes modificar los valores x5 / x25 / x100 / x200. Por defecto multiplicador es x1.

cuando haces click en multiplicador puedes navegar con el encoder y seleccionar la opcion que quieres haciendo click, con esto ya vuelves al menu principal de inmediato y ya puedes modificar mas rapido los valores.

- 9 Encender 

encender se trata de un submenu, donde al hacer click entras a este submenu y puedes navegar con el enconder y para seleccionar una opcion vuelves hacer click en la opcion y entras en un loop y para salir tienes que hacer otro click.

cuando sales del loop haciendo click vuelves al submenu encender y para salir de este, tienes que ir a la pantalla volver hacer click y volveras al menu principal.

- 10 Reset valores

reset valores es un atajo para poner todos los valores en 0 es decir dejas la velocidad y distancia de ambos ejes en 0 y el multiplicador en 1.

- 11 AutoHome

como lo dice su nombre en este punto puedes hacer un autohome, esta rutina se hace automaticamente en ambos ejes primero se realiza en X y luego en Y, La pantalla te mostrara el estado actual de la rutina y **en caso de error tendras que reiniciar todo el sistema.**

### **Display_20x4**

el display de 20x4 al ser mucho mas grande nos sirve para mostrar mucha mas informacion en cada pantalla, de esta forma todas las pantallas son operativas y se reducen solo a 1 las pantallas de informacion. 

Con este el codigo B tiene un numero de 9 pantallas, solo 1 de informacion y el resto todas operativas.

- 1  Posicion actual de ejes X | Y

Como la libreria trabaja con movimientos absolutos es decir considera la posicion actual de los ejes, en esta pantallas puedes conocer dicho valor para ambos ejes.

- 2 Velocidad de eje X

en esta pantallas puedes conocer el valor actual de la velocidad en X, y si haces click puedes modificarla.

cuando haces el click te dira nuevo valor * multiplicador y en la siguiente linea te dira el valor nuevo.

para guardar el valor debes hacer otro click y los valores ya se guardaran.

- 3 distancia de eje X

en esta pantallas puedes conocer el valor actual de la distancia en X, y si haces click puedes modificarla.

cuando haces el click te dira nuevo valor * multiplicador y en la siguiente linea te dira el valor nuevo.

para guardar el valor debes hacer otro click y los valores ya se guardaran.

- 4 Velocidad de eje Y

en esta pantallas puedes conocer el valor actual de la velocidad en Y, y si haces click puedes modificarla.

cuando haces el click te dira nuevo valor * multiplicador y en la siguiente linea te dira el valor nuevo.

para guardar el valor debes hacer otro click y los valores ya se guardaran.
- 4 distancia de eje Y

en esta pantallas puedes conocer el valor actual de la distancia en Y, y si haces click puedes modificarla.

cuando haces el click te dira nuevo valor * multiplicador y en la siguiente linea te dira el valor nuevo.

para guardar el valor debes hacer otro click y los valores ya se guardaran.
- 6 Multiplicador

una nueva funcion que hace mas facil modificar los valores, ya que con esto puedes modificar los valores x5 / x25 / x100 / x200. Por defecto multiplicador es x1.

cuando haces click en multiplicador puedes navegar con el encoder y seleccionar la opcion que quieres haciendo click, con esto ya vuelves al menu principal de inmediato y ya puedes modificar mas rapido los valores.

- 7 Encender 

encender se trata de un submenu, donde al hacer click entras a este submenu y puedes navegar con el enconder y para seleccionar una opcion vuelves hacer click en la opcion y entras en un loop y para salir tienes que hacer otro click.

cuando sales del loop haciendo click vuelves al submenu encender y para salir de este, tienes que ir a la pantalla volver hacer click y volveras al menu principal.

- 8 Reset valores

reset valores es un atajo para poner todos los valores en 0 es decir dejas la velocidad y distancia de ambos ejes en 0 y el multiplicador en 1.

- 9 AutoHome

como lo dice su nombre en este punto puedes hacer un autohome, esta rutina se hace automaticamente en ambos ejes primero se realiza en X y luego en Y, La pantalla te mostrara el estado actual de la rutina y **en caso de error tendras que reiniciar todo el sistema.**



## Funciones_Utilizadas

Para utilizar la pantalla y construir un menu se hizo uso de multiples funciones y variables que mantienen el control, ya sea para detener o actualizar instancias del codigo.

Todas estas funciones las puedes encontrar en el codigo.

### 1 void onPressed()

```C++
void onPressed()
{
  if (menu_indexmenu > 3 && submenu_encender == false)
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
  if (submenu_encender == true)
  {
    in_Menu_2_Modificar();
  }
  if (submenu_encender_rutinatask == true){
    Serial.println("Salir Rutina");
    submenu_encender_rutinatask = false;
    out_Menu_2_modificar();
    stepper_X.emergencyStop();
    stepper_Y.emergencyStop();
    digitalWrite(MOTOR_X_ENABLE, HIGH);
    digitalWrite(MOTOR_Y_ENABLE, HIGH);
  }
   if (submenu_multiplicador == true)
  {
    menu_multiplicar();
  }
  
}
```
La funcion onpressed es el callback de la libreria easybutton, con esta controlamos los click y el debounce del boton del enconder.

- 1.1 Entrar y salir menu principal

Para entrar y salir del menu principal, se necesitan dos estados es por esto que ocupamos la variable **check**, con un switch case podemos entrar a modificar un valor como la velocidad o la distancia de los ejes y con un segundo click podemos salir y almacenar estas variables.

En el caso de la pantalla de 16x2 existe la condicion adicional que **menu_indexmenu** sea mayor a 3, ya que las 3 primeras pantallas no son operativas.

En el caso de la pantalla de 20x4 no existe esta condicion ya que todas las pantallas son operativas.

```C++
   	if (menu_indexmenu > 3 && submenu_encender == false)
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
```

in_menu_1() tiene todos los cambios necesarios para entrar al menu y out_menu_1() tiene lo necesario para cambiar al menu principal.

- 1.2 Entrar al menu de encender 

En este caso cuando presionas la pantalla de encender entras a un submenu llamado encender donde puedes operar cada eje por separado o en conjunto y adicionalmente puedes probar la rutina configurada.

Cuando ya estas en el submenu y seleccionas una opcion se hace el llamado para modificar los valores en **in_Menu_2_Modificar()**.

```C++
   if (submenu_encender == true)
  {
    in_Menu_2_Modificar();
  }
  if (submenu_encender_rutinatask == true){
    Serial.println("Salir Rutina");
    submenu_encender_rutinatask = false;
    out_Menu_2_modificar();
    stepper_X.emergencyStop();
    stepper_Y.emergencyStop();
    digitalWrite(MOTOR_X_ENABLE, HIGH);
    digitalWrite(MOTOR_Y_ENABLE, HIGH);
  }
```

Una vez que se se inicia la rutina en algun eje, se activa la variable **submenu_encender_rutinatask**, y si se hace click se detiene la rutina y se detienen los motores de inmediato y adicionalmente se llama out_Menu_2_modificar() y se vuelve al submenu encender y para volver al menu principal hay que presionar la opcion volver en el submenu encender.

- 1.3 Submenu multiplicador.

la funcionalidad de multiplicador es nueva y sirve para cambiar los valores de velocidad o distancia mas rapido.

```C++
   if (submenu_multiplicador == true)
  {
    menu_multiplicar();
  }
```
cuando se presiona llama a menu_multiplicar() que bloquea y entra al menu de multiplicacion. 

### void loop ()

en el loop se manejan los limites y las etapas de cada menu o submenu

- 2.1 control de menu principal
```C++
  void loop()
{

  if (menu_modificar == false && submenu_encender == false) rotary(1, 11);

  if (menu_modificar == true) {
    rotary(1, 100);
    cambiarValores(menu_modificar, menu_indexmenu);
  }
 ```
en la primer condicion se evalua si modificar es verdadero es decir si aun no haces click en alguna pantalla y si submenu encender se encuentra falso o no activado

cuando esto se cumple se llama rotary con los valores 1,11 que son el minimo y el maximo valor que puede tener el encoder.

cuando haces click en alguna pantalla operativa, se activa la segunda condicional es decir quieres modificar, y modificar es true.
con este se llama a rotary con valor 1,100 minimo y maximo y se llama a la funcion cambiar valores con un valor primeramente booleano y un segundo valor entero.

con esto le pasamos un valor verdadero para modificar y la pantalla que tiene modificar, ya que cada pantalla tienes valores distintos para modificar.

- 2.2 entrar y salir del submenu encender 
```C++
 if (submenu_encender_count == true)rotary(1, 5);

  if (submenu_encender_modificar == true) {
    sub_cambiarValores(submenu_encender_modificar, submenu_encender_indexmenu2);
  }
  if (submenu_encender_rutinatask == false) button.read();
 ```
 cuando haces click en la pantalla encender entras al submenu y llamamos a la funcion rotary con 1,5 minimo y maximo, con esto puedes navegar por las 5 pantallas del submenu encender.

 y cuando haces click en una pantalla del submenu encender no llamamos a rotary ya que no es necesario modificar valores solo operamos el boton, y llamamos a la funcion sub_cambiarvalores con un valor booleano y su indice para accionar una rutina.

 por ultimo si presionas una rutina en loop bloqueamos la lectura del boton del encoder y esta lectura la hacemos directamente en el ciclo while de la rutina. Esto nos facilita la detencion inmediata de los motores.

 - 2.3 entrar y salir del submenu multiplicador
```C++
  if(submenu_multiplicador_count==true)rotary(1,4);

  if(submenu_multiplicador_modificar==true){
    menu_multiplicador_modificar(submenu_multiplicador_modificar,submenu_multiplicador_indexmenu3);
  }
  
 ```
cuando haces click en la pantalla multiplicador, llamamos a rotary con 1,4 minimo y maximo para que navegues por sus 4 opciones en este caso cuando haces click no te devolvemos al submenu si no que directamente al menu principal es por esto que en este submenu no esta la pantalla volver.

cuando seleccionas una opcion del submenu ej: x200, llamamos a submenu_multiplicador_modificar con un valor booleano y su indice para modificar. Se actualiza la variable multiplicador y se te devuelve al menu principal y ya puedes cambiar los valores de 100 en 100 o de 200 en 200.

### void rotary(int ROTARYMIN, int ROTARYMAX)

- 3.1 rotary min max 
```C++
  encoder.tick();
  newPos = encoder.getPosition() * ROTARYSTEPS;
  if (newPos < ROTARYMIN) {
    encoder.setPosition(ROTARYMIN / ROTARYSTEPS);
    newPos = ROTARYMIN;
  } else if (newPos > ROTARYMAX) {
    encoder.setPosition(ROTARYMAX / ROTARYSTEPS);
    newPos = ROTARYMAX;
  }
 ```
 para navegar por las distintas instacias del menu necesito limites del valor del encoder es por esto que la primera parte de la funcion rotary se encarga de evaluar los valores maximos y minimos que puede tener. Con esto nos aseguramos que se muestre la cantidad de pantallas exactas y necesarias para la navegacion. 

 - 3.2 cambiar indices y controlar nuevos valores
 ```C++
   if (lastPos != newPos) {
    if (menu_modificar == false && submenu_encender == false) {
      menu_indexmenu = newPos;
      menuDisplay(menu_indexmenu);
    }
    if (submenu_encender_count == true) {
      submenu_encender_indexmenu2 = newPos;
      submenu_display(submenu_encender_indexmenu2);
    }
    if(submenu_multiplicador_count==true){
      submenu_multiplicador_indexmenu3 = newPos;
      sub_menu_multiplicador(submenu_multiplicador_indexmenu3);
    }
    lastPos = newPos;
  }
}
 ```
 para tener distintas pantallas y distintos valore se ocupan indeces (indexmenu1,indexmenu2,indexmenu3), cada submenu se opera y se programa como si se tratase de un menu nuevo lo unico que se les agrega es una entrada y salida. 

En esta parte actualizamos el valor nuevo del encoder considerando el min y el max, este valor nuevo se almacena en newPos y se va entregando a este indice segun sea necesario.

adicionalmente aca llamamos a menudisplay() una funcion dedicada a mostrarte todos los datos en pantalla.

como puedes ver el codigo se va repitiendo y solamente cambian los indices segun se necesite por ejemplo estas en el menu principa llamamos a la funcion menudisplay le pasamos el indice y te muestra la pantalla segun este valor. y asi se repite con cada submenu. 

### menuDisplay(int mode)

- 4.1 como Funciona menu display

menu display espera un valor entero, este valor se lo entragamos en rotary y es el indice segun cada submenu, pero como manejamos distintos menus, existe distintos menudisplay

en este caso es el menu display del menu principal, y este espera el valor y luego lo evalua en el switch case, en este caso solo encontraremos informacion de cada pantalla no operaciones de las pantallas. 

```C++
 switch (mode) {
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
 ```
como puedes ver aca esta toda la informacion que puedes ver en cada pantalla, estas van del 1 al 11 en caso del display de 16x2 y del 1 al 9 en caso del display de 20x4

Cada numero muestra informacion distinta, cabe decir que este numero luego lo entramos al modificador para cambiar o entrar en la rutina.


### void cambiarValores(bool ok, int index) 

- 5.1 como se cambian los valores.

```C++
 switch (mode) {
   void cambiarValores(bool ok, int index) {

  if (ok == true) {
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
 ```
aca es donde cambiamos los valores cada vez que haces click en alguna pantalla del menu principal como puedes ver esperamos un valor booleano para saber que si debemos cambiar algo y luego esperamos el indice para evaluar donde haras el cambio, es decir esperamos un valor de 4 a 11 y segun sea ese valor modificamos las variables o disparamos alguna otra funcion.

aca tambien estan las entradas a los submenus.
```C++
   if(index == 8){
      in_Menu2_in_multiplicador();
    }
    
    if (index == 9)
    {
      in_menu_2();
    }
 ```
 por ejemplo si presionas en la pantalla 8 llamamos al  in_Menu2_in_multiplicador() y entras al submenu de multiplicador, cuando presionas una serie de variables cambian para detener el menu principal y activar otros limites en el encoder. como esto lo teniamos que hacer frecuentemente lo mejor fue meter todo dentro de una funcion y asi nos ahorramos mucho codigo y nos asegurabamos de que los valores que cambiemos siempre sean los mismos(Lo mismo aplica para el submenu 2 o el indice 9 de encender).

Con esto entras a los submenus, pero en esta funcion tambien puedes hacer un autohome.

```C++
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
``` 
respecto del autohome al inicio no lo dejamos activo, pero puedes descomentarlo y ya funcionaria.

cuando presionas en la pantalla numero 11 autohome, llamamos a homi_X y luego a homi_Y, cada rutina de estas contiene la informacion para hacer un autohome del eje y tambien recuerda que si este sale mal debes reiniciar el sistema.

luego si sale todo bien te devolvemos automaticamente al menu principal simulando el click del boton por eso dejamos check en 0 y modificar en falso, limpiamos la pantalla y te devolvemos a la primer pantalla con menudisplay(1).

## Pantallas_Funciones
## Rutinas_Menu
## [Fourth Example](http://www.fourthexample.com) 