# Práctica 3

3.1. Los planificadores se ocupan de gestionar cómo se distrubuyen los recursos de la CPU en el tiempo entre varios procesos y de decidir en qué órden se ejecutarán los procesos en memoria.\
Existen 3 tipos:

- Long-term scheduler: Carga procesos del disco (HDD o SSD) a memoria principal (RAM) asi pueden comenzar su ejecución. Cuándo esto sucede, un proceso pasa al estado "ready". Controla el grado de "multiprogramación", ya que controla cuántos procesos están en el estado "ready" en un instante dado

- Short-term scheduler: Es responsable de seleccionar procesos de la cola de listos y asignarles la CPU. Debe asegurar que ningún proceso sufra "starvation" y el mantener ocupado al procesador, cuan más posible. Llama al despachador, que realiza los cambios de contexto.

- Medium-term scheduler: Se ocupa de gestionar lo que se llama "swapping", que implica mover procesos entre la memoria principal y el disco. Esto se da, por ejemplo, cuándo están en un estado de espera por un evento E/S.\
Ayuda a reducir el grado de multiprogramación y gestionar el equilibrio de procesos I/O bound y CPU bound en memoria principal para maximizar rendimiento. 


3.2. Un cambio de contexto es el proceso mediante el cual el sistema operativo guarda el estado de un proceso y restaura el de otro para cederle la CPU. (pág. 159)
En resumidas cuentas, si porque existe el preemptive SFJ scheduling, en dónde si llega un proceso con un CPU burst con una duración de tiempo menor al tiempo restante del CPU burst del proceso actual, el proceso actual puede ser interrumpido y se puede realizar un cambio de contexto al nuevo proceso. Pero, si el planificador opera con un protocolo de SJF sin desalojo, ahí no.  

3.3. Si se trata de un sistema en dónde la interacción de usuario no debe ser tomada bajo consideración, entonces que sea responsivo no es un problema a tener en cuenta. Por ende, SJF resulta el scheduling más optimo, dado que provee el tiempo de espera promedio mínimo para los procesos a ser ejecutados por la CPU. Se debe tener en cuenta que para su apropiada implementación, se deben hacer los calculos adecuados para predecir con relativa cercanía la duración del próximo CPU burst.

3.4. La respuesta es b), Round-Robin. La planificación Round Robin asigna la CPU a cada proceso listo por turnos y durante un intervalo de tiempo fijo (quantum), garantizando igualdad y evitando que un proceso monopolice el procesador. a) NO. Aunque en sistemas en dónde no hay interacción con el usuario puede resultar óptimo, en aquellos que involucran interacción del usuario puede ocurrir el problema en dónde procesos con "CPU bursts" más largos son completamente privados de los recursos de la CPU, resultando en lo que se llama "process starvation". c) Idem al punto anterior. d) En ningún tipo de sistema resulta óptimo dado que, suponiendo que tenemos un proceso que debe esperar una operación de E/S (como una rpta. de la red o que se devuelva información de un disco), ese tiempo de espera la CPU estará "idle", desperdiciando tiempo y recursos que en otro protocolo hubieran sido utilizados.

3.5. En un sistema de tiempo compartido, el criterio de planificación más relevante es el tiempo de respuesta, no la optimización en el uso del procesador.

Razón:\
Estos sistemas están orientados a la interacción con el usuario, por lo que es fundamental que las respuestas a las acciones sean rápidas y predecibles. Un planificador que prioriza el tiempo de respuesta garantiza equidad y evita que un proceso monopolice la CPU, aun cuando ello implique una utilización del procesador ligeramente menor debido a cambios de contexto. En cambio, un sistema que maximiza la utilización de la CPU pero responde lentamente no cumple el objetivo de un sistema de tiempo compartido.

3.6. Los 7 estados de un proceso en un sistema operativo describen su ciclo de vida, añadiendo los estados de suspensión a los 5 estados comunes (Nuevo, Listo, En ejecución, Esperando/Bloqueado, Terminado) para incluir Listo Suspendido y Esperando Suspendido, que representan procesos trasladados a almacenamiento secundario debido a presión de memoria, permitiendo una gestión del sistema más compleja y una mayor capacidad de respuesta.

a) Pasa de bloqueado a listo una vez que finaliza su evento, es decir, una vez que se realiza la E/S esperada o se recibe el recurso necesario que indujo el bloqueo en primer lugar\
c) Ejecutando a bloqueado. Es esperable que pase de este estado a bloqueado en el caso de que se espere una E/S o se espere un recurso. Es necesario para alojar los recursos de la CPU a otro proceso para maximizar su uso\
d) Listo a ejecutando. Ya con el recurso recibido o evento completado y la CPU libre, el proceso que se bloqueo incialmente podrá renaudar ejecución\

b) Un proceso en un sistema non-preemptive jamás sera desalojado los recursos de la CPU sin su propia voluntad, es decir, no cederá la CPU a no ser que se bloquee (o en otras palabras, pase primero por el estado bloqueado). Por ende, no puede pasar directamente de ejecutando -> listo, debe realizar el camino de ejecutando -> blocked/waiting -> listo

3.7. Considerando que en RR los procesos dependientes de I/O, es decir, que esperan E/S o las finalizaciones de eventos en el transcurso de su ejecución, liberan los recursos de la CPU al bloquearse para que sean cedidos a procesos que si necesitan la CPU en aquel instante, el uso de la CPU no se verá afectado por la característica de P2 de ser I/O bound Entonces, el uso de la CPU será del 100% siempre y cuándo P1 no haya finalizado 

3.8.

- P2 termina en t = 11

- P3 termina en t = 13

- P4 termina en t = 14

- P1 termina en t = 19 

3.9. La interrupción del disco que indica el fin de una operación sobre el mismo (puede ser lectura o escritura) afecta únicamente al proceso que produjo la interrupción; este estado pasará de "waiting" a "ready". Aun en un sistema monoprocesador de núcleo único varios procesos pueden cambiar de estado entre dos pulsos de reloj, ya que el cambio simultáneo no necesariamente implica "ejecución simultánea" o paralela.