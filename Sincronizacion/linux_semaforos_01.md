# C Linux Semáforos System

\
The problem of the "critical section" appears between processes where there is at least one consumer process and one process which produces 

We will use three syscalls:
- semget(): Allows processes to either create a new semaphore set or access an existing semaphore set\
We can use it to create semaphores

- semctl(): Semaphore control operations\
We can use this syscall to access semaphore control op. specified by "cmd" and remove the semaphore set from the system if need be

- semop(): performs the actual decrement and increment operations that control access to the shared resource

[Useful explanation!](https://www.youtube.com/watch?v=CG2nS6h8-SY)

Considering they are syscalls, we could call them part of the Unix System V Semaphore API (V for "verhoog", meaning increment).

>Quizás valga la pena aclarar que un semáforo mutex asegura que solo un proceso a la vez pueda acceder a un recurso a la vez. Un semáforo, por el otro lado, usa un contador para gestionar el acceso a múltiples recursos, permitiendo que varios procesos los accedan hasta cierto límite.

- RTOS stands for "Real Time OS"

## Tabla comparativa Mutex + Semáforo 
---
|                | Mutex                                                                                                                         | Semáforo                                                                                |
| -------------- | ----------------------------------------------------------------------------------------------------------------------------- | --------------------------------------------------------------------------------------- |
| Definicion     | Mecanismo que asegura que un unico proceso acceda a un recurso a la vez                                                       | Mecanismo de señalización que controla acceso a varios recursos a través de un contador |
| Inicialización | Usualmente se inicializa en un estado "desbloqueado"                                                                          | Suele inicializarse en un nro. representando la cantidad de recursos disponibles        |
| Propiedad      | El proceso que lo "lockeó" en primer lugar                                                                                    | Caulquier unidad de ejecución puede liberarlo, no sólo el que lo adquirió               |
| Comportamiento | Si un mutex está bloqueado, otras unidades de ejecucion que intenten desbloquearlo no podrán hasta que el dueño lo desbloquee | Si el contador vale 0, procesos tratando de decrementarlo serán bloqueados              |

\
![](/Imagenes/mutex_graph.png)

![](/Imagenes/sem_graph.png)
---

## ipcs command

>Basically, ipcs shows the status of inter process communication facilities in the OS, such as shared memory segments and semaphores. Flags such as -m, -q and -s allow to filter for specific types: memory, queues and semaphores respectively

---
## More on semget()

![](/Imagenes/semget_drawing.png)

```
#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/sem.h>

int main(void)
{
    int semid = semget(0xa, 2, IPC_CREAT | IPC_EXCL | 0600);
    if(errno == EEXIST)
        printf("Ya existe un conjunto de semaforos para la clave 0xa\n");
    else
        printf("semid = %d\n", semid);
    exit(0);
}
```

>"|" allows me to combine multiple independent flags into one argument in the following manner (simplified example):\
>-\
>IPC_CREAT = 0101\
>IPC_EXCL = 0200\
>0600 = 0010\
>Result = 0311

ipcrm -s [semid] <--- Elimina el semáforo con el semid que ingresemos

---
Breve resumen de algunas definiciones vistas hasta ahora:
- Condición de concurso: When there is concurrent access to shared data and the final result depends on the order of execution
- Sección crítica: code segment where shared data is accessed 
- Sección de entrada: Code which requests permission to access the critical section
- Sección de salida: Code segment that is executed after a process exits the critical section

![](/Imagenes/locks_critical.PNG)

En este caso, el semáforo sería nuestro cerrojo/candado

Recordamos que un semáforo solo puede accederse a través de dos operaciones **atómicas**:

```
acquire()
{
    while value <= 0; //no-op
    value--;
}

release()
{
    value++;
}
```
---
## semctl():

Podemos usarlo, por ejemplo, para "setear" el valor del semáforo

Devolución: un entero. -1 en caso de error

Argumentos: 
1. Identificador del conjunto de semáforos (int)
2. Nro. del semáforo (int); índice del semáforo dentro del conjunto
3. (cmd), un entero que corresponde al código de operación, cómo GETVAL, SETVAL 
4. El valor inicial del semáforo; int. En un semáforo binario, si lo inicializamos en 0 la puerta "está cerrada". Si lo hacemos en uno, está abierta

## Example code: 
![](/Imagenes/example_code_semctl.PNG)
 
![](/Imagenes/example_code_semctl_2.PNG)

>cc file.c -o init invokes the system C compiler to compile and link a C source file and produce an executable named init

```
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/sem.h>

void P(int semid, int sem) //P performs the "wait" op.
{
    struct sembuf buf;
    buf.sem_num = sem;
    buf.sem_op = -1;
    buf.sem_flg = 0;
    semop(semid, &buf, 1);
    //"On semaphore set semid, apply ONE semaphore op. described by buf"
}

void V(int semid, int sem) //V performs the "signal" op.
{
    struct sembuf buf;
    buf.sem_num = sem;
    buf.sem_op = 1;
    buf.sem_flg = 0;
    semop(semid, &buf, 1);
    //"On semaphore set semid, apply ONE semaphore op. described by buf"
}
```

>cc PyV.c -Wall -g -c\
>Con este comando no creamos un ejecutable, sino un "archivo objeto"
---
## Realizar un breve ejercicio:

Proceso A: produce; Proceso B: consume

>Debemos lograr que se ejecuten ordenadamente suponiendo que están en un ciclo infinito de producción y consumo son independientes entre sí

>Hemos creado 2 semáforos; S0 está inicializado en 1 y S1 en 0

## LA EXPLICACIÓN COMPLETA ESTÁ EN EL MINUTO 36 DE [ESTE VIDEO](https://youtu.be/ub2YTpCq3Aw?si=pp-dEHcZfnIyyLBM&t=2160)

### Código de los procesos: 

```
//PA
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/sem.h>

void P(int, int);
void V(int, int);

int main(void)
{
    int semid = semget(0xa, 0, 0);
    while(1)
    {
        P(semid, 0);
        printf("Seccion critica del proceso pa\n");
        sleep(1);
        V(semid, 1);
    }
    exit(0);
}
```

```
//PB
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/sem.h>

void P(int, int);
void V(int, int);

int main(void)
{
    int semid = semget(0xa, 0, 0);
    while(1)
    {
        P(semid, 1);
        printf("Seccion critica del proceso pb\n");
        sleep(1);
        V(semid, 0);
    }
}
exit(0);
```