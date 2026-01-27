#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

//Use -lm at the end of gcc command

#define MAX_RAM_KB 1024
#define MAX_PAGS 1024 //Si la cantidad maxima de KB que puede almacenar la RAM es 1024 y todo frame debe ser mayor o igual que 1KB, 1024 es un nro. apto
#define MAX_FRAMES 1024
#define MAX_PROC 1024

typedef struct Proceso
{
    int pid; 
    int tamanioKB;
    char tipo; 
    int cantPags; //tamanioKB/tamanioFrameKBytes
    int pageTable[MAX_PAGS]; //inicializada en -1 antes de que se carguen paginas en RAM
}Proceso;

typedef struct Frame
{
    int ocupado; //1 si esta ocupado y 0 si esta libre
    char procesoTipo; //s si esta asignado al SO y u si esta asignado a una pagina de un proceso de usuario
    int pid;
}Frame;

int tamanioRAMKBytes;
int tamanioSOKBytes;
int tamanioFrameKBytes;
int cantFramesXProc;
Frame tablaDeFrames[MAX_FRAMES];
Proceso procesosActivos[MAX_PROC];
int cantProcActivos; //Contador cumple propósito de guardar la posición dentro del vector de procesos activos para agregar nuevos sin pisar procesos precedentes
int frameActual;
int cantFramesRAM;

//Funcion provista por geeksforgeeks.org/c/c-binary-to-decimal/
int binaryToDecimal(int n) {
    int dec = 0;
    int base = 1;
    
    while (n) {
        int last_digit = n % 10;
        n = n / 10;

        dec += last_digit * base;

        base = base * 2;
    }

    return dec;
}

void actualizarFrames(Frame tablaDeFrames[], Proceso *procesoNuevo, int *frameActual)
{
    int i = 0;
    while(i<(*frameActual))
        i++;

    for(int j = i; j<(procesoNuevo->cantPags + i); j++)
    {
        tablaDeFrames[j].ocupado = 1;
        tablaDeFrames[j].procesoTipo = procesoNuevo->tipo;
        tablaDeFrames[j].pid = procesoNuevo->pid;
        *frameActual = (*frameActual) + 1;
    }
}

void actualizarTablaPags(Proceso *proceso, Frame tablaDeFrames[])
{
    //Recorro la tabla de frames hasta encontrar al proceso indicado
    int i = 0;
    while(i < cantFramesRAM && tablaDeFrames[i].pid != proceso->pid)
    {
        i++;
    }
    //No encontre el proceso
    if (i == cantFramesRAM)
    {
        printf("No hay un proceso asociado con ese PID cargado en memoria principal\n");
        return;
    }

    //Una vez ubicado el frame base, inserto los indices en el array 'pageTable' del proceso en cuestion uno por uno. j = indice de la tabla de pags. 
    for(int j = 0;j < proceso->cantPags && i < cantFramesRAM && tablaDeFrames[i].pid == proceso->pid; j++)
    {
        proceso->pageTable[j] = i;
        i++;
    }
}

void inicializarTablaPags(Proceso *proceso) //funcion llamada al inicializarse el vector que guarda los procesos activos
{
    for(int i = 0; i<MAX_PAGS; i++)
        proceso->pageTable[i] = -1;
}

void agregarProcesoAActivos(Proceso procesosActivos[MAX_PAGS], Proceso procesoNuevo, int *cantProcActivos, int *frameActual)
{
    int i = 0;
    while(i<*cantProcActivos)
        i++;

    procesosActivos[i] = procesoNuevo;
    inicializarTablaPags(&procesosActivos[i]);
    
    (*cantProcActivos)++;
    actualizarFrames(tablaDeFrames, &procesoNuevo, frameActual);
    actualizarTablaPags(&(procesosActivos[i]), tablaDeFrames);
}

void especificarParametros()
{
    int ingresar;
    do
    {
        printf("Desea ingresar los parametros que definen el sistema computacional? (1: SI | 0: NO)\n");
        scanf("%d", &ingresar);
        if(ingresar == 1)
        {
            printf("\nTamanio de la memoria real en KB:\n");
            scanf("%d", &tamanioRAMKBytes);
            while(tamanioRAMKBytes>MAX_RAM_KB)
            {
                printf("\nIngreso invalido, vuelva a ingresar el tamanio de la memoria principal en KB:\n");
                scanf("%d", &tamanioRAMKBytes);               
            }
            printf("\nTamanio del SO en KB:\n");
            scanf("%d", &tamanioSOKBytes);
            while(tamanioSOKBytes>tamanioRAMKBytes)
            {
                printf("\nIngreso invalido, vuelva a ingresar el tamanio del SO en KB:\n");
                scanf("%d", &tamanioSOKBytes);               
            }
            printf("\nTamanio de un frame en KB:\n");
            scanf("%d", &tamanioFrameKBytes);
            while(tamanioFrameKBytes>tamanioRAMKBytes || tamanioFrameKBytes <= 0)
            {
                printf("\nIngreso invalido, vuelva a ingresar el tamanio de un frame en KB:\n");
                scanf("%d", &tamanioFrameKBytes);              
            }
            printf("\nCantidad de frames que seran asignados a cada proceso:\n");
            scanf("%d", &cantFramesXProc);
            while(cantFramesXProc>(tamanioRAMKBytes/tamanioFrameKBytes))
            {
                printf("\nIngreso invalido, vuelva a ingresar la cantidad de frames por proceso:\n");
                scanf("%d", &cantFramesXProc);               
            }
            cantFramesRAM = tamanioRAMKBytes/tamanioFrameKBytes;
            
            Proceso procesoSO;
            procesoSO.pid = 1;
            procesoSO.tamanioKB = tamanioSOKBytes;
            procesoSO.tipo = 's';
            procesoSO.cantPags = tamanioSOKBytes/tamanioFrameKBytes;
            agregarProcesoAActivos(procesosActivos, procesoSO, &cantProcActivos, &frameActual);

            ingresar = 0;
        }
        else if(ingresar != 0)
        {
            printf("Opcion invalida. Vuelva a intentar\n");
            printf("Desea ingresar los parametros que definen el sistema computacional? (1: SI | 0: NO)\n");
            scanf("%d", &ingresar);           
        }
    } while (ingresar != 0);
}

void inicializarTablaFrames(Frame tablaDeFrames[MAX_FRAMES])
{
    for(int i = 0; i<MAX_FRAMES; i++)
    {
        tablaDeFrames[i].ocupado = 0;
        tablaDeFrames[i].procesoTipo = '0';
        tablaDeFrames[i].pid = -1;
    }
}

void inicializarProcesosActivos(Proceso procesosActivos[MAX_PAGS])
{
    for(int i = 0; i<MAX_PROC; i++)
    {
        procesosActivos[i].pid = -1;
        procesosActivos[i].tamanioKB = -1;
        procesosActivos[i].cantPags = -1;
        procesosActivos[i].tipo = '0';
        inicializarTablaPags(&procesosActivos[i]); 
    }
}

void mostrarTablaFrames(Frame tablaDeFrames[MAX_FRAMES])
{
    printf("\n");
    for(int i = 0; i<MAX_FRAMES; i++)
    {
        if(tablaDeFrames[i].ocupado == 1)
        {
            printf("\nFrame %d:\n", i);
            printf("PID del proceso propietario: %d\n", tablaDeFrames[i].pid);
            printf("Ocupado: %d\n", tablaDeFrames[i].ocupado);
            printf("Tipo de proceso: %c\n", tablaDeFrames[i].procesoTipo);           
        }
    }
}

void cargarProceso(int *cantProcActivos, Proceso procesosActivos[MAX_PAGS], Frame tablaDeFrames[], int *frameActual)
{
    int ingresar;
    do
    {
        printf("\nDesea ingresar los datos de un nuevo proceso? (1: SI | 0: NO)\n");
        scanf("%d", &ingresar);
        if(ingresar == 1)
        {
            Proceso procesoNuevo;
            printf("\nPID del proceso:\n");
            scanf("%d", &procesoNuevo.pid);
            printf("\nTamanio en KB del proceso:\n");
            scanf("%d", &procesoNuevo.tamanioKB);
            if((int)ceil(((double)procesoNuevo.tamanioKB/tamanioFrameKBytes))>cantFramesXProc)
            {
                printf("\nProceso demasiado grande. Vuelva a intentarlo. Tamanio en KB:\n");
                scanf("%d", &procesoNuevo.tamanioKB);
            }
            printf("\nTipo de proceso: (u: de usuario | s: del S.O.)\n");
            scanf(" %c", &procesoNuevo.tipo);
            procesoNuevo.cantPags = (int)ceil((double)procesoNuevo.tamanioKB/tamanioFrameKBytes);
            agregarProcesoAActivos(procesosActivos, procesoNuevo, cantProcActivos, frameActual);
        }
        else if (ingresar != 0)
        {
            printf("Opcion invalida. Vuelva a intentar\n");
            printf("Desea ingresar los datos de un nuevo proceso? (1: SI | 0: NO)\n");
            scanf("%d", &ingresar);           
        }
    }while(ingresar!=0);
}

void imprimirTablaPags(int *procID, Proceso procesosActivos[], int *cantProcActivos)
{
    //Busco el proceso correspondiente al PID en el array
    int i = 0;
    while(procesosActivos[i].pid != *procID && i<*cantProcActivos)
        i++;

    if(i == *cantProcActivos)
    {
        printf("No hay un proceso activo con ese PID en este momento\n");
        return;
    }

    if(procesosActivos[i].pid == *procID)
    {
        //j = indice de la tabla de paginas
        for(int j = 0; j<procesosActivos[i].cantPags; j++)
        {
            printf("\nPagina %d: Frame %d\n", j, procesosActivos[i].pageTable[j]);
        }
    }
}

Proceso * returnProcesoPointer(int *pid, Proceso procesosActivos[])
{
    int i = 0;
    while(i<cantProcActivos && procesosActivos[i].pid != *pid)
        i++;
    if(procesosActivos[i].pid == *pid)
    {
        return &procesosActivos[i];
    }
    return NULL;
}

int main()
{
    cantProcActivos = 0;
    frameActual = 0;
    inicializarTablaFrames(tablaDeFrames);
    especificarParametros();
    printf("\nTamanio RAM: %d, Tamanio SO: %d, Tamanio Frame: %d, Cant frames x proceso: %d\n", tamanioRAMKBytes, tamanioSOKBytes, tamanioFrameKBytes, cantFramesXProc);
    
    int operacion;
    do
    {
        printf("\nQue operacion desea realizar?\n");
        scanf("%d", &operacion);
        switch(operacion)
        {
            case 1:
            {
                cargarProceso(&cantProcActivos, procesosActivos, tablaDeFrames, &frameActual);
                break;
            }
            case 2:
            {
                mostrarTablaFrames(tablaDeFrames);
                break;
            }
            case 3:
            {
                int procID;
                printf("\nInserte el PID del proceso cuya tabla de paginas desea consultar:\n");
                scanf("%d", &procID);
                imprimirTablaPags(&procID, procesosActivos, &cantProcActivos);
                break;
            }
            case 5: 
            {
                int pid, formato, dirEnDecimal;
                printf("Indique el PID del proceso cuya direccion fisica quiere consultar: \n");
                scanf("%d", &pid);
                Proceso * procesoAConsultar = returnProcesoPointer(&pid, procesosActivos);
                if(!procesoAConsultar)
                {
                    printf("Ingreso erroneo. Vuelva a intentar:\n");
                    printf("Indique el PID del proceso cuya direccion fisica quiere consultar: \n");
                    scanf("%d", &pid);
                }
                printf("Indique la direccion logica del proceso anterior:\n");
                printf("La desea ingresar en formato binario o hexadecimal? (1: binario | 2: hex)\n");
                scanf("%d", &formato);
                if(formato!=1 && formato!=2)
                {
                    printf("Ingreso invalido. Vuelva a intentar: \n");
                    scanf("%d", &formato);
                }
                switch(formato)
                {
                    case 1:
                    {
                        int binaryInput;
                        printf("\nDir. en binario:\n");
                        scanf("%d", &binaryInput);
                        dirEnDecimal = binaryToDecimal(binaryInput);
                        break;
                    }
                    case 2:
                    {
                        int hexInput;
                        scanf("%x", &hexInput);
                        dirEnDecimal = hexInput;
                        break;
                    }
                }
                if(dirEnDecimal>((*procesoAConsultar).tamanioKB*1024 - 1))
                {
                    printf("Esta intentando acceder a una direccion invalida (SEGFAULT)\n");
                    return -1;
                }

                int frameBase, dirBase;
                (*procesoAConsultar).pageTable[0] = frameBase;
                dirBase = frameBase * tamanioFrameKBytes * 1024;
                printf("La dir fisica del proceso es: %d KB\n", (dirBase + dirEnDecimal));
                break;
            }
            case 0:
            {
                printf("Gracias y chau!\n");
                break;
            }
            default: printf("\nOpcion invalida\n");
        }
    } while(operacion!=0);
}