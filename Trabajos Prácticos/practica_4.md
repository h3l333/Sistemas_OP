# Práctica 4

1. La espera ocupada trata de un estado que se da cuándo un proceso que está esperando para ejecutar la sección crítica de su programa, que contiene algún recurso compartido con otro proceso, chequea continuamente la condición de ingreso a dicha sección para monitorear cuándo debe comenzar la ejecución.\
Al chequear condiciones, el proceso en espera consume recursos de la CPU que podrían, en un escenario alternativo, estar empleandosé para ejecutar instrucciones útiles
Esta característica provoca overhead y la postergación de procesos.\
Vale la pena aclarar que en el caso de que entre un proceso de alta prioridad que quiere acceder una sección a mitad de ejecución por un proceso anterior, no se liberará el lock que posee este proceso 'A', sino que si o si el proceso B tendrá que esperar. Esto provoca que un proceso de alta prioridad espere a uno de baja, contradiciendo el protocolo del planificador dado.
Para prevenir esto, muchas veces ocurren modificaciones en las prioridades de los procesos

2.  El clock como componente de hardware en si no se vería afectado dado que sin importar si el sistema operativo responde a sus pulsos o no seguirá funcionando igual a nivel mecánico: seguirá girando a la velocidad angular correspondiente a su frecuencia característica aun si el SO ignora sus señales. Sin embargo, a nivel sistema operativo las consecuencias serán mayores:\
Existen los "timer interrupts", interrupciones de procesos que ocurren cuándo el timer del sistema, que responde al reloj de la computadora, señaliza que ya finalizó un plazo de tiempo dado, lo que implica alguna operación por parte del CPU, cómo un cambio de contexto por ejemplo. Suponiendo que hablamos de un sistema operativo de tiempo compartido que implementa un protocolo de planificación como Red Robin, las consecuencias negativas se vuelven aún más aparentes. Al no contar ya con timer interrupts, no se puede desalojar los recursos de la CPU despues de un plazo de tiempo dado, rompiendo con las reglas dictadas por el protocolo. Idem con "SJF". 

3. Que wait() y signal() sean atómicos significa que el semáforo S no puede ser modificado simultáneamente por más de un wait() o signal(). Cada operación ocurre de manera indivisible, garantizando consistencia y exclusión mutua sobre el contador del semáforo.\
Si no se ejecutan atomicamente y dos operaciones acceden a un mismo semaforo a la vez, la incosistencia en el valor de este provocaría anomalias en el acceso a la sección crítica de un programa y dos procesos podrian acceder a recursos compartidos a la vez, provocando la corrupción de datos y posibles problemas de seguridad en el sistema. 

4. 
```
escritor()
{
    escribir(file); // Acceso recurso compartido
    signal(lector);
}

// ---

lector()
{
    wait(lector);
    leer(file); // Acceso recurso compartido
}
```

Lo que se puede dar en la implementación del modelo planteado es la "pérdida de señales" por parte del proceso lector: esto se debe a que, cómo nada inhibe que el proceso escritor vuelva a correr luego de dar una indicación al proceso lector para que este lea del recurso compartido, este puede señalizar al proceso lector aún si este no se encuentra en un estado en espera.\
Al no acumular notificaciones pendientes, el proceso lector podría quedar en un estado indefinido de espera (bloqueado; deadlock)\
Otro problema que se puede apreciar es la falta de exclusión mutua. 

5. 
```
while(turno != i)
{
    // SECCION CRITICA
    turno = j;
}
```

|       | Proceso 0 | PROCESO 1 |
| :---: | :-------: | :-------: |
|   i   |     0     |     1     |
|   j   |     1     |     0     |

Recordamos los 3 criterios del problema de la sección crítica: la exclusión mutua, el progreso y la espera limitada o "bounded waiting"\
Aunque si asegura la exclusión mutua, presenta dos limitaciones:\
- En cuanto al progreso, depende completamente de que el proceso actual salga y actualice la variable "turno". Si el proceso se interrumpe o no quiere entrar, el otro proceso queda bloqueado indefinidamente aun si este si quiere acceder a la S.C. libre.
- Por lo que planteamos anteriormente, la espera limitada no es una garantía en este algoritmo; esto cómo consecuencia puede llevar al "process starvation" 

6. 

![](/Imagenes/TP4.png)