# Práctica 3

3.1. https://www.geeksforgeeks.org/operating-systems/process-schedulers-in-operating-system/

3.2. https://medium.com/@drajput_14416/cpu-scheduling-shortest-job-first-sjf-priority-scheduling-and-round-robin-rr-c42de2469950
Un cambio de contexto es el proceso mediante el cual el sistema operativo guarda el estado de un proceso y restaura el de otro para cederle la CPU. (pág. 159)
En resumidas cuentas, si porque existe el preemptive SFJ scheduling, en dónde si llega un proceso con un CPU burst con una duración de tiempo menor al tiempo restante del CPU burst del proceso actual, el proceso actual puede ser interrumpido y se puede realizar un cambio de contexto al nuevo proceso 

3.3. Si se trata de un sistema en dónde la interacción de usuario no debe ser tomada bajo consideración, entonces que sea responsivo no es un problema a tener en cuenta. Por ende, SJF resulta el scheduling más optimo, dado que provee el tiempo de espera promedio mínimo para los procesos a ser ejecutados por la CPU. Se debe tener en cuenta que para su apropiada implementación, se deben hacer los calculos adecuados para predecir con relativa cercanía la duración del próximo CPU burst.

3.4. La respuesta es b), Round-Robin. La planificación Round Robin asigna la CPU a cada proceso listo por turnos y durante un intervalo de tiempo fijo (quantum), garantizando equidad y evitando que un proceso monopolice el procesador. a) NO. Aunque en sistemas en dónde no hay interacción del usuario puede resultar óptimo, en aquellos que involucran interacción del usuario puede ocurrir el problema en dónde procesos con "CPU bursts" más largos son completamente privados de los recursos de la CPU, resultando en lo que se llama "process starvation". c) Idem al punto anterior. d) En ningún tipo de sistema resulta óptimo dado que, suponiendo que tenemos un proceso que debe esperar una operación de E/S (como una rpta. de la red o que se devuelva información de un disco), ese tiempo de espera la CPU estará "idle", desperdiciando tiempo y recursos que en otro protocolo hubieran sido utilizados.

3.5. En un sistema de tiempo compartido, el criterio de planificación más relevante es el tiempo de respuesta, no la optimización en el uso del procesador.

Razón:
Estos sistemas están orientados a la interacción con el usuario, por lo que es fundamental que las respuestas a las acciones sean rápidas y predecibles. Un planificador que prioriza el tiempo de respuesta garantiza equidad y evita que un proceso monopolice la CPU, aun cuando ello implique una utilización del procesador ligeramente menor debido a cambios de contexto. En cambio, un sistema que maximiza la utilización de la CPU pero responde lentamente no cumple el objetivo de un sistema de tiempo compartido.

3.6. The 7 states of a process in an operating system describe its lifecycle, typically adding suspension to the common 5 states (New, Ready, Running, Waiting/Blocked, Terminated) to include Suspended Ready and Suspended Wait, representing processes moved to secondary storage due to memory pressure, allowing for more complex system management and responsiveness. 

a) Pasa de bloqueado a listo una vez que finaliza su evento, es decir, una vez que se realiza la E/S esperada o se recibe el recurso necesario que indujo el bloqueo en primer lugar
c) Ejecutando a bloqueado. Es esperable que pase de este estado a bloqueado en el caso de que se espere una E/S o se espere un recurso. Es necesario para alojar los recursos de la CPU a otro proceso para maximizar su uso
d) Listo a ejecutando. Ya con el recurso recibido o evento completado y la CPU libre, el proceso que se bloqueo incialmente podrá renaudar ejecución

b) Un proceso en un sistema non-preemptive jamás sera desalojado los recursos de la CPU sin su propia voluntad, es decir, no cederá la CPU a no ser que se bloquee (o en otras palabras, pase primero por el estado bloqueado). Por ende, no puede pasar directamente de ejecutando -> listo, debe realizar el camino de ejecutando -> blocked/waiting -> listo
https://www.geeksforgeeks.org/operating-systems/states-of-a-process-in-operating-systems/

3.7. Considerando que en RR los procesos dependientes de I/O, es decir, que esperan E/S o las finalizaciones de eventos en el transcurso de su ejecución, liberan los recursos de la CPU al bloquearse para que sean cedidos a procesos que si necesitan la CPU en aquel instante, el uso de la CPU no se verá afectado por la característica de P2 de ser I/O bound Entonces, el uso de la CPU será del 100% siempre y cuándo P1 no haya finalizado 