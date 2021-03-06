# THE CODE 

Para entender el funcionamiento de cada funcion y de las pantallas del menu, se recomienda leer el siguiente contenido, donde se detallan las funciones utilizadas y que valores esperan, las pantallas que se muestran y cual funcion dentro del codigo, las rutinas que se realizan en cada pantalla operativa.

# Contenido
1. [Funciones](#Funciones_Utilizadas)
2. [Pantallas](#Pantallas_Funciones)
3. [Rutinas](#Rutinas_Menu)
4. [Fourth Example](#fourth-examplehttpwwwfourthexamplecom)


## Funciones_Utilizadas

Para utilizar la pantalla y construir un menu se hizo uso de multiples funciones y variables que mantienen el control, ya se para detener o actualizar instancias del codigo.

Todas estas funciones las puedes encontrar en el codigo.

### void rotary(int ROTARYMIN,int ROTARYMAX)

```C++
void rotary(int ROTARYMIN,int ROTARYMAX){
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
  } 
   menuDisplay(indexmenu);
   lastPos = newPos;
  }
```

la funcion void rotary se ocupa para actualizar los valores del rotary encoder y para establecer y evaluar valores maximos y minimos.

1. establecer valores minimos y maximos.

Para establecer los valores minimos y maximos, se llama a la funcion y se le pasan dos valores tipos enteros el primero sera el valor minimo y el segundo tambien tipo entero sera el valor maximo de funcionamiento.

por ejemplo para establecer un rango de 0,7, se haria de la siguiente forma.

```C++
   	rotary(0,7);// Donde 0 es el minimo y 7 el maximo
```

luego la funcion va actualizando el valor del enconder y evaluando si esta en el minimo o en el maximo


2. Actualizar el valor y mostrar la pantalla.
```C++
  if (lastPos != newPos) {
  if(modificar==false)indexmenu=newPos;
  
   menuDisplay(indexmenu);
   lastPos = newPos;
  }
```
Primero comparamos la ultima posicion con la nueva y no son iguales actualizamos newPos(Variable que se recoge los valores del enconder), luego evaluamos si modificar es falso y si lo es indexmenu pasa a tener los valores del encoder y luego llama a la funcion [menudisplay](#menudisplay). y luego actualiza el ultimo estado.

## Pantallas_Funciones
## Rutinas_Menu
## [Fourth Example](http://www.fourthexample.com) 