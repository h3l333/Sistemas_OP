# 1er Módulo

>Los recursos de la máquina son fuertemente interdependientes y comparten los recursos de la máquina entre sí. La CPU no es excepción a esta regla.

Los procesos que están "ready" son aquellos que están esperando acceso a la CPU y sus recursos

Metas de la planificación:
- Aprovechar al máximo la CPU a través de la multiprogramación
- Distribuir justamente sus recursos entre los distintos procesos
  
Dentro de este tema, observaremos a los procesos cómo ciclos de ejecución en el CPU y espera de E/S

![](/Imagenes/cpu_burst_io.png)

---
Recordamos que el cambio de contexto ocurre con una llamada al sistema:\
Cuándo un programa solicita leer de un archivo, por ejemplo, el sistema operativo le quita la CPU para ejecutar las instrucciones privilegiadas solicitadas a través de la API del kernel para programas de usuario

>Algunos ejemplos de syscalls ya vistos son fopen(), que devuelve un puntero a un archivo *FILE; fork(), que devuelve el id de un proceso hijo (dentro del padre; devuelve 0 dentro del hijo) o un -1 en caso de error; read(), que devuelve la cantidad de bytes leidos de un archivo; close(), que devuelve 0 en caso de éxito, etc.

Mientras un proceso espera a que finalice un evento de entrada/salida, se encuentra en un llamado "I/O burst".\
***NO se encuentra en este estado si está esperando a que el kernel termine de ejecutar instrucciones privilegiadas solicitadas por él: ya que pese a que el proceso de usuario no se está ejecutando en modo usuario en aquel instante en la CPU, eso no quiere decir que no esté "usando la CPU"***

---
## -------------------- Planificador de la CPU --------------------

Qué hace: Selecciona entre los procesos en memoria ready y asigna el CPU a uno de ellos. 

|                                                                                                Preemptive Scheduling                                                                                                |                                                                                                    Non-preemptive scheduling                                                                                                    |
| :-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------: | :-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------: |
| El scheduler puede interrumpir a un proceso en ejecución sin que este lo solicite, para asignar la CPU a otro proceso (por ejemplo, por llegada de un proceso de mayor prioridad o por expiración del time quantum) | Los procesos no son interrumpidos por el scheduler mientras usan la CPU. Liberan la CPU solo por decisión propia: al finalizar su ejecución o al realizar una syscall bloqueante (por ejemplo, lectura de un archivo o sleep()) |

\
![](/Imagenes/process_states.png)

>El planificador no "causa" los cambios en estado de los procesos, sino que interviene una vez que se produce un cambio en el estado de un proceso para gestionar qué proceso se ejecutará a continuación\
\
Interviene cuándo se dan los siguientes cambios de estado, por ejemplo:\
_ running -> waiting (se da en ambos sistemas)\
_ running -> ready (solo apropiativos)\
_ waiting -> ready (ambos)\
_ running -> end (ambos sistemas)

*Planificador apropiativo = preemptive scheduler*

Por qué puede intervenir un planificador apropiativo en el cambio de estado de waiting a ready? Interviene en el caso que la finalización de un evento E/S implique la llegada de un proceso de mayor prioridad a aquel actualmente siendo ejecutado en la CPU a la cola de "ready". Si es de mayor prioridad, la CPU expulsa al proceso actual de la CPU y da sus recursos al recién llegado a la cola, el de alta prioridad.

>En un sistema no apropiativo puede pasar un proceso de waiting a ready en caso de que PREVIAMENTE haya voluntariamente liberado la CPU por, por ejemplo, esperar un evento o invocar la llamada sleep() y luego se haya completado el evento o finalizado la operación sleep()

---
## El despachador da el control de la CPU al proceso seleccionado por el planificador

Se ocupa de cambiar el contexto de la CPU y restaurar el estado previo del proceso a ejecutar, en caso de que este no esté siendo ejecutado por primera vez (actualiza el registro de la próxima instrucción además de los demás registros de la CPU, como el puntero a la pila y flags, etc). Tengamos en cuenta que el despachador y el planificador ambos son componentes del kernel

La latencia de despacho se refiere al tiempo que tarda el despachador en detener un proceso e iniciar el otro

>El planificador decide, el despachador cede el control. Se asemeja esta relación a la diferencia entre un jefe de despacho y un camionero/transportista

