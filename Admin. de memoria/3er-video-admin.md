El compialdor no sabe donde esta el registro base perteneciente al archivo ejecutable- pero al administrador de memoria si

El compilador genera un proceso con direcciones logicas- Deben ser logicas dado que el que se ocupa de mapearlas sobre la memoria fisica no es nignun otro componente salvo el administrador de memoria

A la direccion base le sumamos el espacio limite para alojar al espacio en un espacio de memoria virtual

MMU = Memory Management unit

>Mapea direcciones virtuales -> direcciones fisicas

![](/Imagenes/mmu.png)

![](/Imagenes/relocal-via-mmu.png)

## Que es el swapping?

Trata del proceso en el cual un proceso es trasladado de memoria principal a memoria secundaria o viceversa para manejar la multiprogramacion y optimizar el rendimiento del dispositivo

- Definiciones asociadas:
    - Almacenamiento secundario: Es un sistema secundario y persistente, distinto de la memoria RAM, que conserva los datos aunque el equipo se apague
    - Roll out, roll in: variante del swapping en donde se prioriza la ubicacion de procesos de alta prioridad en memoria principal y aquellos de menor prioridad residen generalmente en la memoria secundaria

>La mayor parte del swapping es tiempo de transferencia, que es directamente proporcional a la cantidad de memoria que se este moviendo por el manejador de memoria en el kernel (que difere de la MMU, componente de hardware responsable por el mapeo de direcciones virtuales sobre direcciones fisicas de memoria) 

>El componente que actualiza en memoria secundaria únicamente las partes del proceso que se modificaron durante su ejecución es el administrador de memoria del sistema operativo
---

NOTA: El sistema maneja una cola de listas reservada para procesos guardados a traves de paginas en memoria secundaria que estan competiendo por entrar en memoria principal para renaudar su ejecucion

>La imagen de un proceso es el conjunto completo de información que el sistema operativo mantiene en memoria para representar y ejecutar un proceso.

![](/Imagenes/diagrama-swapping.jpg)

## Asignacion contigua

El código y los datos críticos del sistema operativo residen en direcciones de memoria conocidas y protegidas. En arquitecturas tradicionales, usualmente reside en las direcciones de memoria mas bajas, comenzando en la nro. 0.

En esas mismas direcciones (o en una región fija bien definida) se encuentra el vector de interrupciones, que es la tabla que indica qué rutina del sistema operativo debe ejecutarse cuando ocurre una interrupción o excepción.

>El mapeo de las direcciones virtuales sobre memoria fisica es dinamico,  
no es fijo y cada proceso puede poseer una direccion fisica diferente acorde a lo que indiuque la MMU 

![](/Imagenes/asign-contigua.png)
El esquema muestra un sistema con particion de memoria contigua de tamano variable

- En varios algoritmos de manejo de memoria contiguos, el sistema operativo es capaz de reubicar procesos de tal manera de eliminar la fragmentacion externa y asi optimizar el uso de la cantidad de memoria libre en RAM

### Existen las politicas de ajuste

![](/Imagenes/asign-politicas-contigua.png)

*Best-fit genera la menor fragmentacion interna*

*Worst-fit es un algoritmo de asignación de memoria con particiones variables que selecciona el bloque libre más grande para ubicar un proceso, con el objetivo de dejar espacios libres grandes y reducir la fragmentación externa.*

---    
    - La fragmentacion externa se refiere al fenomeno en donde hay suficiente espacio en memoria para satisfacer una solicitud, pero dicha solicitud no puede ser realizada porque el espacio disponible no es contiguo
    
    - La interna se refiere al fenomeno en donde el espacio asignado a un proceso es mayor del que este necesita y se produce un espacio interno de memoria reservado para ese proceso que es desperdiciado y no utilizado

![](/Imagenes/manejo-contiguo-variable-c-fragmen.png)

>Si existe compactación de memoria, surge un overhead debido al costo de reubicar procesos en memoria principal, ya que implica copiar grandes bloques de memoria y suspender temporalmente la ejecución de los procesos.