# THE CODE 

Para entender el funcionamiento de cada funcion y de las pantallas del menu, se recomienda leer el siguiente contenido, donde se detallan las funciones utilizadas y que valores esperan, las pantallas que se muestran y cual funcion dentro del codigo, las rutinas que se realizan en cada pantalla operativa.

# Contenido
1. [Funciones](#Funciones_Utilizadas)
2. [Pantallas](#Pantallas_Funciones)
3. [Rutinas](#Rutinas_Menu)
4. [Fourth Example](#fourth-examplehttpwwwfourthexamplecom)


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