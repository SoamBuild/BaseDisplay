<h1>Librerias utilizadas</h1>

* [AiEsp32RotaryEncoder](https://github.com/igorantolic/ai-esp32-rotary-encoder)
* [LiquidCrystal_I2C](https://github.com/fdebrabander/Arduino-LiquidCrystal-I2C-library)

<h1>Codigo</h1>

Este pequeño bloque se repite en casi la todos los switch, y con esas variables y lineas logro modificar los valores y guardarlos.

```C++
   	if (modificar==true){
          modificar=2;
          velocidad = encoderValue;
          lcd.setCursor(0,1);
          lcd.print(velocidad);
        }
```

<h2>Switch</h2>

Respecto a los switch...

* <h3>Case1 = para ver los dos valores desde el principio cuando estan en 0 y cuando los cambias, vuelve a esta pantalla para que revises los valores</h3> 
* <h3>Case2 = Modificar la velocidad y su variable</h3> 
* <h3>Case1 = Modificar la distancia y su variable</h3> 
* <h3>Case3 = iniciar la rutina de movimiento, aca deberia ir el codigo de movimiento de los motores.</h3> 