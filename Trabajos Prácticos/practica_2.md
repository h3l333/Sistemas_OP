# Práctica 2

1. En cuánto a la administración de procesos, el sistema operativo principalmente:

- Se ocupa de planificar. Coordina cuándo y por cuánto tiempo los distintos procesos activos (en memoria secundaria o primaria) tienen acceso a la CPU y sus recursos. Esta tarea es fundamental para asegurar la eficaz ejecución de programas y la responsividad del sistema.
- Sincronización. Asegura que los procesos accedan a recursos compartidos de manera coordinada sin la ocurrencia de condiciones de carrera. La planificación se ocupa de manejar los recursos de la CPU mientras que la sincronización gestiona el acceso a recursos compartidos.
- Comunicación entre procesos: la asegura a través de mecanismos cómo la memoria compartida o a través del uso de colas ubicadas en el espacio de memoria del kernel, por ejemplo.
- Transiciones de estado. Cuándo un proceso pasa de un estado a otro (por ejemplo, pasa de la cola de "listos" a un estado "running", o de "running" a "waiting" a causa de un evento I/O, por ejemplo, etc) el procesador se ocupa de ejecutar las operaciones del kernel que producen esos cambios.
- Creación y eliminación de procesos: Cuándo un proceso realiza las syscalls exit() o fork(), el que se ocupa de crear un nuevo proceso en memoria y cederle a este todos los recursos que necesita para ejecutarse, es el sistema operativo. 

2. 
|               |                                                   Proceso                                                   |                   Hilo                   |
| :-----------: | :---------------------------------------------------------------------------------------------------------: | :--------------------------------------: |
|  Definición   |              Programa en ejecución con direcciones, recursos y un estado característico propio              | Unidad de ejecución dentro de un proceso |
| Ciclo de vida |                                                Independiente                                                |      Dependiente del proceso padre       |
|   Creación    |                                          Implica mayor "overhead"                                           |                  Ligero                  |
|   Recursos    | Cada proceso tiene su propio espacio en memoria, salvo que se especifique lo contrario explícitamente (IPC) |            Comparten memoria             |

3. No. Uno o varios archivos ejecutables conforman parte de los aspectos que caracterizan a un proceso, pero no lo integran de manera completa. Un proceso es un programa en ejecución, un contexto de ejecución en el que se puede encontrar la CPU. Además de contener uno o más de un archivo ejecutable, un proceso también posee:
- Espacio de memoria propio
- Un estado propio
- Su propio PCB (process control block)
- Hilos propios
- Archivos abiertos
- Su propia pila y puntero hacia dicha pila
- Espacio heap
- Variables globales
- Etc.

4. El sistema operativo asigna un Process Control Block a cada proceso para mantener registro de las características fundamentales de las distintas unidades de trabajo en la máquina. Contiene descriptores de archivos abiertos, el estado, contador de programa, información de memoria, registros de la CPU, PID, etc.

5. El multithreading se refiere a la ejecución concurrente a través del uso de tiempo compartido (suponiendo un sistema monoprocesador) de varios hilos de un proceso dado, mientras que la multiprogramación hace referencia a un concepto similar sólo que aplicado a procesos, unidades de trabajo relativamente más "pesadas" y lentas que los hilos.

6. 
![](/Imagenes/process_states.png)

- New -> Ready (Admitted)
- Ready -> Running (Despachado por el despachador integrado dentro del planificador del sistema)
- Running -> Waiting (syscall o evento E/S, cómo leer de un disco o la recepción de un paquete)
- Waiting -> Ready (se realiza el evento E/S o syscall)
- Running -> Ready (ocurre unicamente en sistemas preventivos que usan planificadores que lo habilitan (como Round Robin, por ejemplo))
- Running -> End (o finalizó el programa su ejecución o fue terminado por alguna causa externa)

7. 
![](/Imagenes/six_state_process_graph.png)

- Waiting -> Suspend (sucede cuándo el sistema operativo "muda" el proceso a memoria secundaria antes que dejarlo en memoria principal; suele ocurrir en procesos con largos tiempos de espera cuándo el espacio en memoria principal es limitado)
- Suspend -> Ready-suspended (se completó el evento por el cual el proceso fue bloqueado en primer lugar)
- Ready-suspended -> Blocked-suspended (puede llegar a darse si ya se satisfizo la condición original que lo bloqueó pero luego surge otra)

8. 
![](/Imagenes/states_7.png)

9. El "activador" se ocupa de ceder la CPU a un proceso seleccionado por el planificador del sistema, realizar el cambio de contexto. La respuesta es d)\
Restaurar la imágen de memoria de un proceso implica restaurar segmento de código, segmentos de datos (variables globales), la memoria heap, su stack y estado de paginación. El dispatcher solo se ocupa de los valores inmediatos que se le asignan a la CPU cuándo sucede un cambio de contexto.

10. Si, ya que justamente un proceso se puede definir cómo "el contexto de ejecución en el que se encuentra el procesador en un instante dado", que este contexto difiera de proceso a proceso es algo que se puede obviar.

11. Los hilos comparten los recursos de memoria del proceso padre. Sin embargo, para diferenciarse entre sí cuenta cada uno con un propio contador de programa, sus propios registros y pila propia. 

