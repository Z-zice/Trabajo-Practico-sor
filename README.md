# Sistemas Operativos y Redes

## Trabajo Práctico de hilos y semáforos
A continuacion muestro el pseudocodigo del programa:

cortar()
  v(s.mezclar)       //en la funcion a travez de un signal incremento el valor del semaforo

  p(s.mezclar)   //la funcion wait comprueba el valor del semaforo, en este caso esta en 1 , el hilo segue ejecutandose y se decrementa en una unidad el valor del semaforo.
 mezclar()
  v(s.salar)
  
  
  mutex()                    //si un hilo entra se bloquea el acceso hasta que termine
  p(s.salar)
salar()                      //en este caso los hilos accederas de manera concurrente, para solucionarlo utilizaremos mutex, el
  v(s.armar_medallones)      // cual proteje la seccion critica,
  mutex()
   
   
  p(armar_medallones)
armar_medallones()
  v(cocinar_medallones)
  
  mutex()
  p(cocinar_medallones)
cocinar_medallones()
  v(hornear_pan)
  mutex()
  
  mutex()
  p(hornear_pan)
hornear_pan()
  v(cortar_extras)
  mutex()
  
  p(cortar_extras)
cortar_extra()
  v(armar_hamburguesas)
  
  p(armar_hamburguesas)
armar_hamburguesas() 
  v(ganador)
   
  p(ganador)
ganador()
  printf("el ganador es: ")     //se crea otro semaforo para indicar el ganador. ya que el primer hilo que acceda a la funcion armar_hamburguesa sera el ganador.
  
  Problemas encontrados y soluciones:
  
  -Primer problema es leer de un archivo la recete, este problema aun no lo pude solucionar, a la momento de buscar informacion habias varias formas de leer un archivo, no logre volcarlo al TP
  -Indicar el ganador, pense primero hacerlo con una bandera pero no pude ver donde colocarlo, luego pense en crear un funcion ganador() en el cual, deduje que el primer hilo que llegue a armar_hamburguesa habilitara con un semaforo a la siguente funcion , y esta me imprimira por pantalla el ganador.
  -Declaracion de las variables semaforos, esto lo solucione declarandolos siempre antes del main, caso contratio nunca se declaraban
  -
