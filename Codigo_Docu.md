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

### void submenu_display(int sub_mode) 

- 6.1 mostrando las pantallas de los submenu.

el funcionamiento de esta rutina es exactamente igual que el de menudisplay.

esperamos un indice de 1 a 5 y segun ese valor que se manda cada vez que giras la perilla en encender puedes ver informacion distinta.

en este punto no tenemos que cambiar valores solamente llamar rutinas pero eso lo hacemos en otra funcion.

```C++
   switch (sub_mode) {
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
``` 
el encargado de evaluar es el switch case y evalua el valor de indice que le pasamos en este caso es submenu_encender_indexmenu2, como nos sobro espacio en estas pantallas agregamos en un extremo del display un indice para que te sea mas facil navegar.

### void sub_cambiarValores(bool sub_ok, int sub_index) 

- 7.1 como llamamos a las rutinas.

como no tenemos que modificar valores no llamamos a rotary aca solamente trabajamos con el boton del enconder. 

```C++
  void sub_cambiarValores(bool sub_ok, int sub_index) {

  if (sub_ok == true) {

    if (sub_index == 1) {
      Test_rutina();
    }
    if (sub_index == 2) {
      submenu_encender_rutinatask = true;
      digitalWrite(MOTOR_X_ENABLE, LOW);
      digitalWrite(MOTOR_Y_ENABLE, LOW);
      Rutina_move();
    }
    if (sub_index == 3) {
      submenu_encender_rutinatask = true;
      digitalWrite(MOTOR_X_ENABLE, LOW);
      Rutina_move_X();
    }
      if (sub_index == 4) {
      submenu_encender_rutinatask = true;
      digitalWrite(MOTOR_Y_ENABLE, LOW);
      Rutina_move_Y();
    }
    if (sub_index == 5) {
      out_Menu_2();
    }
  }
}
```

para valor del 1 al 5 hay una operacion y como puedes ver antes de llamar a la operacion rutina encendemos los motores y llamamos a la rutina, para apagarlos lo hacemos dejando en falso rutina task al principio del codigo en el callback button_press(). 

por ejemplo haces click en rutina en loop, llamamos a rutina_move que bloquea el codigo principal y solo funciona un while que se encarga de mover de forma sincronica los dos ejes y lee el boton. para salir de esa rutina haces click y rutina_task es falsa y vuelves al submenu.

esto es importante no te devolvemos al menu principal, te mandamos directo al submenu encender y si quieres volver giras hasta llegar al 5 "volver" y ahi llamamos a  la funcion out_Menu2() encargada de cambiar unos valores y devolverte al menu principal.

### sub_menu_multiplicador(int display)

- 8.1 mostrando los pantallas del submenu multiplicador

ya explicamos para que era multiplicador, y como se usaba, ahora toca explicar como se muestra la informacion del submenu multiplicador aca algunas cosas cambian pero la base se mantiene
```C++
  void sub_menu_multiplicador(int display){
  switch (display) {
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
```

aca esperamos un valor tipo entero que es el submenu_multiplicador_indexmenu3, pero que dentro del contexto local de la funcion lo llamaremos display, y el encargado de evaluar display es un switch case que va mostrando cada valor x5,x50 y asi.

En estas pantalla solo tenemos la informacion para seleccionar y el indice que se muestra en un extremo no hay nada mas, lo nuevo viene en como seleccionamos cada opcion del submenu modificar.

### void menu_multiplicador_modificar(bool state,int estado){

- 9.1 como funciona el click en el multiplicador.

Esta funcion espera dos valores un booleando y un entero, que llamaremos state y estado, pero que realmente son submenu_multiplicador_modificar y submenu_multiplicador_indexmenu3 

```C++
   if(state ==true){
    if(estado == 1){
      submenu_multiplicador_valuemultiplicador = 5;
      lcd.setCursor(0, 1);
      lcd.print("Multi X5");
      out_menu2_multiplicador();      
    }
    if(estado == 2){
      submenu_multiplicador_valuemultiplicador = 50;
      lcd.setCursor(0, 1);
      lcd.print("Multi X50");
      out_menu2_multiplicador();      
    }
     if(estado == 3){
      submenu_multiplicador_valuemultiplicador = 100;
      lcd.setCursor(0, 1);
      lcd.print("Multi X100");
      out_menu2_multiplicador();      
    }
    if(estado == 4){
      submenu_multiplicador_valuemultiplicador = 200;
      lcd.setCursor(0, 1);
      lcd.print("Multi X200");
      out_menu2_multiplicador();      
    }
  }
}
```

primero evualuamos el primer valor el booleano state, y si es verdadero revisamos el numero de la pantalla o el indexmenu3. Esta funcion de control es igual a la anteriores salvo que cuando haces click en alguna pantalla, te cambiamos el valor del multiplicador por tu seleccion y te mandamos de vuelta de inmediato al menu principal llamando out_menu2_multiplicador().

### entrar y salir del menu principal.

en el menu principal decimos que entramos y salimos cuando haces un click para cambiar un valor y el segundo click para guardar y salir.

de todo esto se encarga el callback del boton del encoder y en parte el loop que va bloqueando y activando ciertos menus y modificadores.

- 9.1 entrar a modificar un valor

```C++
   void in_menu_1() {
  menu_modificar = true;
  Serial.println("Modificando valores Menu 1");
}
```

al entrar a modificar hacemos verdadero el valor menu_modificar = true y con esto en el loop se bloquea el rotary 1 a 9, rotary pasa a ser 1 a 100 en este caso si debemos usar la perilla asi que la activamos, adicionalmente llamamos al modificador de menu principal con su indece para que puedes cambiar el valor que quieras.

- 9.2 guardar y salir del modificador.

```C++
  void out_menu_1() {
  lcd.clear();
  menuDisplay(menu_indexmenu);
  menu_modificar = false;
  Serial.println("Guardando los valores");
  check = 0;
}
```

para salir es algo mas complejo hay mas cosas, lo primero es que recuerda que para salir es un click mas.

lo primero limpiamos la pantalla, luego te mandamos al menu principal con el ultimo indece conocido, luego negamos el modificador con esto vuelves al rotary 1 a 9 y sales del modificador, y por ultimo reiniciamos el valor de check para decir que ya hiciste los dos click.

### entrar y salir del submenu encender.

aca hay 4 tareas que hacer, primero cuando haces click en la pantalla encender tenemos que bloquear el menudisplay principal y activar un segundo menudisplay para que veas la informacion del submenu encender, luego cuando hagas click en alguna pantalla del submenu encender quiere decir que ejecutas una rutina, y luego cuando haces otro click sales de la rutina, y por ultimo vuelves al menu principal.

para todas esas tareas hay valores cambiando gracias al boton tanto en el callback como en el loop

- 10.1 entrar al submenu encender

```C++
void in_menu_2() {
  Serial.println("IN_SUBMENU");
  //bloqueo contador menu principal;
  menu_modificar = false;
  submenu_encender = true;
  //activo el contador de menu 2
  submenu_encender_count = true;
}
```
cuando haces click en encender, lo primero es bloquear el rotary 1 a 11 y eso lo hacemos negando el menu_modificar, luego debemos decirle a rotary que queremos mostrar 5 pantallas entonces su rango es 1 a 5 eso lo hacemos con submenu_encender siendo verdadero y por ultimo queremos guardar los nuevos valores de rotary en indice nuevo indexmenu2, y eso lo hacemos con submenu_encender_count siendo verdadero.

este ultimo cambio o negacion permite que puedas ver la informacion del submenu encender.

- 10.2 Accionar una rutina 


```C++
 void in_Menu_2_Modificar() {
  Serial.println("Entrando en rutina submenu_encender");
  //desactivo el contador de menu2
  submenu_encender_count = false;
  //Activo Modificador de menu2
  submenu_encender_modificar = true;
}
}
```
al hacer click en una rutina, llamamos a esta funcion, lo primero es desactivar el indexmenu2 y con esto cerramos la informacion en pantalla y hacer verdadero a encender_modificar que se evalua en loop y si es verdadero llama al modificador y el entrega el indice que seleccionaste.


- 10.3 cerrar una rutina

lo primero que debes saber que para cerrar una rutina debes hacer click y se detiene toda la ejecucion del codigo 
```C++
 void out_Menu_2_modificar() {
  Serial.println("Rutina finalizada");
  //desactivo Modificador de menu2
  submenu_encender_modificar = false;
  //Activo el contador de menu2
  submenu_encender_count = true;
  lcd.clear();
  submenu_display(submenu_encender_indexmenu2);
}
}
```
cuando cierras la rutina, no te devuelves al menu principal si no que al submenu encender, es por esto que lo primero es negar modificar para salir del modificador y luego volver a encender el contador para que veas las pantallas de navegacion, limpiamos y te llevamos a la ultima pantalla que viste o al ultimo indice registrado.

- 10.3 salir del submenu 

para salir del submenu encender, debes ir a la pantalla 5 donde dice volver, y asi vuelves al menu principal. 
```C++
 void out_Menu_2() {
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
```
Lo primero es negar menu_modificar para que vuelva el rotary 1 a 9 del menu principal. luego reiniciamos check 0 para limpiar el numero click,  y negamos todos los estados del submenu encender, limpiamos la pantalla, imprimos que volveremos al menu principal, esperamos un segundo y te mandamos a la pantalla numero 1 informativa del menu principal.

## Pantallas_Funciones
## Rutinas_Menu
## [Fourth Example](http://www.fourthexample.com) 