# THE CODE 

Para entender el funcionamiento de cada funcion y de las pantallas del menu, se recomienda leer el siguiente contenido, donde se detallan las funciones utilizadas y que valores esperan, las pantallas que se muestran y cual funcion dentro del codigo, las rutinas que se realizan en cada pantalla operativa.

# Contenido
1. [Pantalla](#Pantallas_Funciones)
2. [Funciones](#Funciones_Utilizadas)
3. [Rutinas](#Rutinas_Menu)
4. [Fourth Example](#fourth-examplehttpwwwfourthexamplecom)

## Pantalla_Funciones

En esta seccion se explican que hace cada pantalla y cual de estas son realmente operativas.

### Pantallas
1. [Pantallas(16x2)](Display_16x2)
2. [Pantallas(20x4)](Display_20x4)

### Display_16x2


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

### Display_20x4


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

## Pantallas_Funciones
## Rutinas_Menu
## [Fourth Example](http://www.fourthexample.com) 