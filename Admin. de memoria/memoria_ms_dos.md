## Microsoft Disk Operating System

Gestión de memoria del MS-DOS (Microsoft Disk Operating System)  

>MS-DOS fue un sistema operativo diseñado para ejecutarse en procesadores de 16 bits que operaban en modo real, capaces de direccionar hasta 1 MB de memoria RAM.

*Memoria convencional (0–640 KB):*  
Área principal de trabajo del sistema.
Contenía el kernel de MS-DOS, controladores de dispositivos residentes y, fundamentalmente, los programas de usuario.
Representaba la única memoria directamente accesible para las aplicaciones.

*Área de memoria superior (640 KB–1 MB):*  
Reservada para componentes del hardware.  
Incluía:
Memoria de vídeo,
ROMs de dispositivos,
Rutinas del BIOS, ubicadas generalmente en los últimos 64 KB.  
No estaba disponible para aplicaciones estándar en las primeras versiones de MS-DOS.
MS-DOS era un sistema monoprocesador y monotarea, sin mecanismos de protección de memoria ni ejecución simultánea de procesos.

---

### Que es el mapeo de memoria?

>El mapeo de memoria es el mecanismo mediante el cual el procesador asocia direcciones lógicas (las que usa un programa) con direcciones físicas reales en la memoria RAM o ROM.

El procesador 8086 tenía registros de 16 bits, lo que permitiría direccionar solo 64 KB si se usaran direcciones lineales simples.  
Para superar esa limitación sin usar registros de 32 bits, Intel introdujo el direccionamiento segmentado.

Dirección de memoria en MS-DOS: 

```
segmento : desplazamiento
```

Dirección física = (Segmento × 16) + Desplazamiento

El desplazamiento permite:
- Acceder a cualquier byte dentro de un segmento de 64 KB.
- Mantener los registros de dirección en 16 bits.

>Para cargar rutinas de interrupción, por ejemplo: si era necesario cargar la rutina de interrupción 10h...  
10h × 4 = 40h. Accedíamos al segmento 0000:0040 y se leían los valores guardados en el índice indicado, los cuales eran cargados automáticamente por la CPU en los registros Code Segment (CS) e Instruction Pointer (IP).