# Sincronización BACA...
Los procesos B, A y C ejecutan sus secciones críticas de manera consecutiva por un plazo de tiempo indefinido o hasta que se de una condición de terminación establecida

>Vamos a realizar la resolución con procesos independientes, usando semáforos en un entorno Linux

Vamos a necesitar 4 semáforos: Sa, Sb, Sc, Sx. Sa = 0, Sb = 1, Sc = 0, Sx = 1 (así se inicializarán)

A cada proceso le correspondrá una sección de entrada, crítica y de salida

|    Proceso B    |    Proceso A    |    Proceso C    |
| :-------------: | :-------------: | :-------------: |
|    wait(Sb);    |    wait(Sa);    |    wait(Sc);    |
|    wait(Sx);    |                 |    wait(Sx);    |
| SECCIÓN CRÍTICA | SECCIÓN CRÍTICA | SECCIÓN CRÍTICA |
|   signal(Sa);   |                 |   signal(Sa);   |
|   signal(Sc);   |   signal(Sx);   |   signal(Sb);   |

[Explicación del flujo de procesos](https://www.youtube.com/watch?v=UZLaeemleRk&list=PLkEdSyoEjf3AiBD3suHMIYsBYGcHtdVR5&index=7)

### Valores que toman los semáforos en la ejecución:

Cuándo el proceso B ejecuta wait(Sb), el valor de Sb pasa de 1 a 0

Recuerdo que un sem. binario en 1 señaliza que un recurso está disponible y que 0 indica lo contrario

|    Semáforo A     |    Semáforo B     |    Semáforo C     | Semáforo X |
| :---------------: | :---------------: | :---------------: | :--------: |
|         0         |         1         |         0         |     1      |
|                   | SECCIÓN ENTRADA B |                   |            |
|         0         |         0         |         0         |     0      |
|                   | SECCIÓN SALIDA B  |                   |            |
|         1         |         0         |         1         |     0      |
| SECCIÓN ENTRADA A |                   |                   |            |
|         0         |         0         |         1         |     0      |
| SECCIÓN SALIDA A  |                   |                   |            |
|         0         |         0         |         1         |     1      |
|                   |                   | SECCIÓN ENTRADA C |            |
|         0         |         0         |         0         |     0      |
|                   |                   | SECCIÓN SALIDA C  |            |
|         1         |         1         |         0         |     0      |
| SECCIÓN ENTRADA A |                   |                   |            |
|         0         |         1         |         0         |     0      |
| SECCIÓN SALIDA A  |                   |                   |            |
|         0         |         1         |         0         |     1      |

>B -> A -> C -> A -> B -> A -> C

---
## Qué son las "matrices semáforo"?

ipcs = IPC status

Al ejecutar este comando, podemos ver secciones de memoria compartida, matrices de semáforo y colas de mensajes

- Secciones de memoria: *permisos, tamaño, propietario, ID (shmid, shared memory ID), nro. de procesos adjuntos al segmento en un instante (nnatch, number of attached processes) y estado (recién creado, bloqueado en memoria física (permanece en RAM para conservar acceso rápido))*
- Matrices: *ID del conjunto, propietario, nro. de semáforos en el conjunto, permisos, valores actuales de los semáforos*
- Colas de msjs.: *ID de la cola, propietario, permisos, cantidad de mensajes actuales, tamaño máximo*

>Conceptualmente, las colas de mensajes (message queues) son buffers en el kernel que permiten a procesos enviar y recibir mensajes de manera ordenada.\
\
Funcionan como en el problema productor–consumidor:\
_ Productor escribe mensajes\
_ Consumidor los lee\
\
El kernel garantiza sincronización, orden FIFO y persistencia temporal hasta que se lean.
---
### Para este ejercicio hay que crear una matriz de semáforos con 4 semáforos

```
#include<stdio.h>
#include<sys/sem.h>
#include<sys/ipc.h>
#include<unistd.h>

int main(void)
{
    int semid = semget(0xa, 1, IPC_CREAT | IPC_EXCL | 0660); //crea recurso semaforo
    if(semid != -1)
    {
        //inicializa para la sincronizacion BACA
        semctl(semid, 0, SETVAL, 0); //Sa = 0
        semctl(semid, 1, SETVAL, 1); //Sb = 1
        semctl(semid, 2, SETVAL, 0); //Sc = 0
        semctl(semid, 3, SETVAL, 1); //Sx = 1
    }
    else
    {
        printf("Error de creacion de recursos\n");
    }
}
```

A su vez, el profesor en el video escribió un programa que permite ver y verificar la inicialización de los 4 semáforos 

![](/Imagenes/programa_verif_sem.PNG)

>The command clear clears the visible display of the terminal window

Además, escribió programas para cada proceso de tal modo de dar un ejemplo práctico de la sincronización de los procesos en Linux.

### Proceso B:

```
#include<stdio.h>
#include<sys/sem.h>
#include<sys/ipc.h>
#include<unistd.h>

#define hele 1

void P(int, int); //wait(Sem)
void V(int, int); //signal(Sem)

int main(void)
{
    int semid = semget(0xa, 0, 0);
    while(hele)
    {
        P(semid, 1); //wait(Sb);
        P(semid, 3); //wait(Sx);
            printf("SECCIÓN CRÍTICA PB\n");
            sleep(1);
        V(semid, 0); //signal(Sa);
        V(semid, 2); //signal(Sc);
    }
    return 0;
}

//definicion de las funciones P y V

void P(int semid, int nsem)
{
    struct sembuf buf;
    buf.sem_num = nsem;
    buf.sem_op = -1;
    buf.sem_flg = 0;
    semop(semid, &buf, 1); //wait
}

void V(int semid, int nsem)
{
    struct sembuf buf;
    buf.sem_num = nsem;
    buf.sem_op = 1; //<---
    buf.sem_flg = 0;
    semop(semid, &buf, 1); //signal
}
```

Visualizó la ejecución de cada uno de los tres procesos (A, B y C) en ventanas separadas. Voy al minuto 45.47 aprox. para apreciarlo 