# Práctica 1

1. Que contiene una tabla de vectores de interrupcion? 
Una tabla de vectores de interrupcion contiene solamente direcciones de memoria en donde estan almacenados distintos programas (series de instrucciones) que responden a distintos interrupts que pueden o provocar programas en ejecución, sea a traves de syscalls o excepciones/errores, o que dispositivos de hardware externos al CPU pueden mandarle al CPU.

2. Cuál de las siguientes instrucciones máquina no debería ejecutarse en modo protegido? Razone su respuesta.

a) Inhibir interrupciones.

b) Instrucción TRAP.

c) Modificar el reloj del sistema.

d) Cambiar el mapa de memoria.

La unica instruccion de las listadas que no debe ejecutarse exclusivamente en modo protegido o kernel es la instruccion trap. Que los programas de usuario puedan realizar instrucciones trap es fundamental para su funcionamiento, ya que les permite solicitar al sistema operativo acceso indirecto a operaciones tales como escribir a memoria, obtener valores en memoria, acceder a dispositivos IO etc. No deben poder inhibir interrupciones porque en caso de que pudieran hacer eso, provocarian consecuencias adversas sobre la ejecucion de otros programas. Modificar el reloj del sistema podria provocar caos en el sistema al repercutir sobre la sincronizacion de ciertos procesos. El mapa de memoria se refiere a la manera en que el sistema operativo distribuye la memoria virtual correspondiente a cada proceso sobre la memoria fisica. Fuera a realizar esto, podria seriamente afectar la seguridad de los datos asociados a ciertos programas y ademas pisar los registros de otros programas segun su voluntad, incluyendo los registros del SO mismo, provocando lo que se llama a veces the blue screen of death 


3. Las principales funciones de un sistema operativo son

Administrar los recursos de hardware para la eficaz ejecucion de programas

Funcionar como intermediario entre el usuario y el hardware a traves del kernel. El kernel es el nucleo del sistema operativo y se encarga de administrar los recursos de hardware, NO de traducir comandos del usuario a instrucciones de maquina que componentes de hardware puedan entender, de eso se encargan programas como el shell. El sistema operativo ademas del kernel tiene componentes que permiten que el usuario y el hardware puedan interactuar, como system libraries, drivers e interfaces. 


4. Diferencia entre mandato y llamada al sistema: Un mandato es ingresado por un usuario a través de la command line interface y frecuentemente implica la ejecución de programas que incluyen system calls para su realización. Ejemplos incluyen, en Linux, whoami, ls, clear, mkdir (crea una nueva carpeta), cd (change directory), etc. Las llamadas al sistema por otro lado son implementadas, explicitamente, por programas en su código para la solicitar al kernel acceso indirecto a operaciones reservadas al modo núcleo, como abrir archivos de la memoria, acceder a recursos de hardware, escribir en memoria, etc. Algunos ejemplos incluyen open(), close(), write(), etc.

5. La multiprogramación es un mecanismo que habilita la aparante ejecución "concurrente" de procesos. La palabra "aparente" aplica a partir del hecho de que, por más que así parezca, los procesos en realidad no se están ejecutando concurrentemente sino que son asignados los recursos de la CPU y son desalojados de la CPU a velocidades extremadamente rápidas, para dar al usuario una experiencia responsiva y permitir que todos los procesos se ejecuten equitativamente acorde a los protocolos establecidos por el sistema operativo. Pero su ejecución es intercalada. Se pueden ejecutar por prioridad, órden, tiempo de "CPU burst" etc acorde al tipo de scheduling que utiliza el OS. Este concepto difiere del multiprocesamiento, concepto que aparece solamente en el contexto de CPUs con varios núcleos o máquinas con varios CPUs. 

6. Las instrucciones de la lista que deben ejecutarse exclusivamente en modo núcleo son: 
- Inhibir todas las interrupciones (SI) Esto se le debe permitir hacer únicamente al kernel dado que, fuera un programa de usuario a inhibir las interrupciones de otros procesos, esto podría implicar graves errores en la ejecución de ellos y llevar a fallas en el sistema. Llevaría a errores en la gestión de archivos, gestión de procesos y comunicación entre procesos. 
- El controlador DMA (direct memory access), dispositivo de hardware configurado por la CPU, se ocupa de quitarle carga a la CPU gestionando el acceso a la memoria por parte de dispostivos de hardware por él, reduciendo overhead y agilizando la realización de sus tareas. 
Un proceso de usuario NO DEBERÍA PODER acceder a este recurso directamente, menos sobreescribir sus registros de control, dado que así podría inhibir el acceso a memoria de componentes de hardware fundamentales y provocar errores de conectividad a la red, fallas en la escritura a discos de almacenamiento, provocar errores en las interfaces gráficas de la máquina, inhibir las entradas dadas por el usuario (keystrokes, clicks), etc.
- Procesos de usuario deberían poder acceder controladores periféricos solamente a través del kernel, dado que fuera a ser otro el mecanismo esto implicaría problemas de seguridad graves. Por ejemplo, un proceso podría tomar fotos de un usuario a través de su webcam, o grabar su voz a través del micrófono sin el consemiento de este. 
- Escribir en los registros de la MMU: No. Fuera a poder hacerlo, podría manipularlos de tal manera de acceder a los datos de procesos ajenos, poniendo en riesgo la seguridad del sistema y los datos dentro de la memoria. 
- Provocar un trap o interrupción de software: Esta es la única instrucción que deben poder ejecutar programas de usuario, dado que son mecanismos vitales para que estos puedan INDIRECTAMENTE acceder el kernel, la memoria y los dispositivos de hardware.  

7. Los manejadores de dispositivos (rutinas de software que habilitan la gestión de recursos de hardware) se introdujeron en la década de los 60. Anteriormente, las computadoras se gestionaban a través de una combinación de:
- Directas configuraciones físicas
- Operación manual por un humano, no por control dinámico durante la ejecución
- Software fuertemente acoplado que era ESPECÍFICO a la máquina dónde se instalaba
El primer prototipo de un sistema operativo con la característica de tiempo compartido fue demostrado en el año 1961. Anteriormente a los sistemas operativos de tiempo compartido, los programas se ejecutaban estrictamente uno a la vez y eran cargados secuencialmente por el operador. A la ejecución de programas secuencial por lotes sin interacción por parte del usuario se le llama "batch processing". 

8. Escribir un sistema operativo utilizando lenguajes de alto nivel tiene varias ventajas, como: 
- Mayor facilidad en su escritura y desarrollo
- Código más facil de mantener al ser más facil de leer por seres humanos y siendo típicamente más corto
- Portabilidad; permite enfocarse en las metodologías de planificación, el aspecto lógico del sistema etc sin preocuparse por la implementación a muy bajo nivel, habilitando su uso en distintos dispositivos con distintos componentes de hardware

9. El intérprete de mandatos es externo al kernel porque se trata de un proceso de usuario que, al igual que los otros procesos, por default cuenta con privilegios menores al kernel. No se ocupa de gestionar los recursos del sistema operativo o de hardware, sino de traducir los comandos ingresados por el usuario a operaciones que el kernel puede entender y ejecutar a través del uso de syscalls.

10. En un sistema operativo monolítico el kernel se ocupa de todas las operaciones básicas, mientras que en uno por capas las tareas esenciales pertenecen a distintos modulos organizados en capas que pueden ejecutar instrucciones privilegiadas, y el kernel se ocupa unicamente de gestionar la interacción entre las capas y la comunicación de los procesos de las capas con el HW.

11. La respuesta es a), la memoria virtual. Esto se debe a que, aunque el uso de memoria virtual resulta el mecanismo de hardware más utilizado en la actualidad para el aislamiento de procesos y protección de datos, no es el único.\
Uno de ellos es, por ejemplo, el mantenimiento de registros correspondientes a la dirección en memoria dónde "empieza" el espacio de memoria de un proceso y su tamaño máximo, es decir, hasta dónde se extiende ese espacio. El "relocation register" contiene la dirección más pequeña y el limit register contiene el tamaño de el espacio en memoria del proceso. Sistemas viejos de multi-tasking usaban este método.\
b) "Protección de memoria" hace referencia al concepto de reservar estrictamente espacio para cada proceso en la memoria genéricamente, más los mecanismos de chequeo implementados para gestionar acceso. Este método de hardware es fundamental para asegurar la apta ejecución de procesos y no puede faltar, debido a que fuera a no implementarse procesos podrían mutuamente e indiscriminadamente "pisarse" la memoria unos a otros, incluyendo aquellos espacios de memoria reservados al sistema operativo.\ 
c) Si un proceso de usuario tiene acceso directo al hardware, esto implicaría que usuario A tiene acceso directo a los datos de usuario B y viceversa, planteando un problema de seguridad enorme dentro del contexto del sistema operativo. Protección de acceso a dispositivos de hardware E/S es primordial para asegurar un sistema que pueda aptamente manejar múltiples usuarios.\
d) Idem con el punto anterior.

12. Se solicita una llamada al sistema operativo con la utilización de los syscalls. El proceso de usuario provee los parametros necesarios para que se lleve a cabo, como el nro. de syscall y los parametros correspondientes y provoca un interrupt, transfiriendo control al sistema operativo, haciendo que la CPU pase de modo usuario a modo kernel en donde tiene elevados permisos. En este modo ejecuta las instrucciones correspondientes a la llamada realizada, almacenadas en el espacio de memoria reservada al sistema operativo. 
Una vez ejecutadas las instrucciones, el sistema operativo devuelve control al proceso de usuario y la CPU retorna a modo usuario. Al devolver control al proceso de usuario, se le retornan los resultados de la operacion realizada (success, failure o data)

13. Usualmente el mantenimiento se un S.O. por capas resulta mas sencillo debido a su estructura modular, permitiendo que cambios en una capa no modifiquen las caracteristicas de una capa ajena. Aunque un sistema monolitico habilite un desarrollo inicial mas sencillo y veloz y permita mayor velocidad de ejecucion, que sus componentes esten fuertemente acoplados resulta una desventaja considerable a la hora de mantenerlo y gestionarlo. 

14. Si, un sistema multitarea debe si o si ser tambien de tiempo compartido si suponemos que hablamos de un sistema con un unico nucleo de procesamiento. Esto se debe a que todo sistema operativo multitarea monoprocesador se basa en los principios de time sharing, en donde los recursos de la CPU se alojan a distintos procesos de manera extremadamente rapida, de tal modo de optimizar su uso y dar la ilusion de concurrencia. Fueran los procesos a ejecutarse secuencialmente sin la utilizacion de los principios de time sharing, ya no seria un sistema multitarea, sino que nos estariamos refiriendo a sistemas como aquellos basados en el batch processing.
Si un sistema es de tiempo compartido, podemos suponer que justamente las unidades de trabajo que comparten el tiempo son tareas (hilos o procesos); asi que si, todo sistema de tiempo compartido es a su vez multitarea.