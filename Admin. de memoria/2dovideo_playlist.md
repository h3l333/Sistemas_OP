# Admin. de memoria

## Tipos de administracion de memoria

![](/Imagenes/tipos-de-admin-mem.png)

>Internal fragmentation wastes space inside allocated memory blocks (fixed-size, like paging), while external fragmentation wastes total memory because free blocks are scattered and non-contiguous (variable-size, like segmentation).   Internal happens when a process gets more memory than needed within its block, creating unused space within that block; external happens when there's enough total free memory but no single contiguous chunk large enough for a new process. 

*Basicamente, la fragmentacion interna y externa se refieren a fenomenos que pueden ocurrir cuando en un sistema cuya administracion de memoria es contigua con particiones fijas suceden una de dos cosas:*

- En el caso de la fragmentacion interna, se da cuando se produce espacio "desperdiciado" de memoria en la particion de un proceso

- En el caso de la externa, se da cuando hay suficiente memoria libre como para ejecutar un proceso, pero este no se puede ejecutar al tratarse de un espacio no contiguo

![](/Imagenes/proteccion-memoria.png)

**To protect memory spaces within the system, assuming contiguous memory management through the use of partitions, the system analyses the address given by the CPU. If it is larger than or equal to the base address AND smaller than the base + limit size, it is valid. The limit address is a relative value and not an absolute value. Otherwise, it is not and the system will return an error**

---

Cuando hablamos de administración de memoria, paginación, segmentación o particiones, nos referimos principalmente a memoria física y a cómo el sistema operativo la gestiona.

- Memoria física: es la RAM real del sistema. Los esquemas como paginación, segmentación y particiones indican cómo se divide y asigna la memoria física a procesos.

- Memoria virtual: es un concepto más alto que combina memoria física + almacenamiento secundario (disco) para dar a los procesos la ilusión de un espacio de direcciones continuo.

>La paginación se usa tanto en memoria física como en virtual, porque las páginas pueden estar en RAM o en disco.

En resumen: los métodos de administración gestionan la memoria física, aunque algunos (como paginación con swapping) también se relacionan con la memoria virtual.