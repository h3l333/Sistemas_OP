#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>

//Use -lm at the end of gcc command

#define CANT_FRAMES 128
#define CANT_PAGINAS 128
#define CANT_PROCESOS 8

typedef struct
{
    int frameNum;  // Indica el nro. de frame dentro de la memoria fisica
    bool ocupado;  // Describe si esta libre o no
    char tipo;     // O es del S.O. o corresponde al espacio de usuario, o esta libre
    int pidDuenio; // Describe el proceso ocupando este espacio de memoria fisica en un instante
} Frame;           // Espacio en memoria fisica

typedef struct
{
    int pid;       // ID del proceso
    int tamanioKB; // Tamanio total del proceso
    int cantPag;   // Determinado por el tamanio del proceso y el tamanio de una pagina
    char tipo;
    int frameDePagina[CANT_PAGINAS]; // Cada indice representa una pagina del proceso y cada valor en un indice indica el frame de la pagina correspondiente
    int paginaEnRAM[CANT_PAGINAS];   // Nos dice si la pagina representada por el indice dado esta en RAM o no (0 si no esta, 1 si esta)
} Proceso;

Frame frames[CANT_FRAMES]; // Tabla de frames, estructura que registra el estado de cada frame de la memoria física
int tamanioFrame = 0;      // Tamanio del frame en KB, determina tamanio memoria real
int framesDelSO = 0;       // Cuantos frames en memoria fisica pertenecen al sistema operativo
int framesPorProceso = 0;  // Cuantos frames se usan para alojar a un proceso en memoria

Proceso procesos[CANT_PROCESOS]; // Vector de structs 'Proceso'; registro de los procesos cargados
int procesosCargados = 0;        // Contador de los procesos en el vector

int parametrosInvalidos = 0; // Contador de parametros invalidos ingresados por el usuario
// Ejemplo: Proceso cuyo tamanio supera framesPorProceso * tamanioFrame

// Funcion que recibe parametros del sistema
// Paso por referencia para que la funcion modifique su valor original
void recibirParametrosSist(int *tamanioFrame, int *framesDelSO, int *framesPorProceso)
{
    int tamanioFrameInput, framesDelSOInput, framesXProcInput;
    printf("\nTamanio de un frame en KB:\n");
    scanf("%d", &tamanioFrameInput);
    *tamanioFrame = tamanioFrameInput; // Accedo al valor directamente en la direccion pasada como arg.
    printf("\nFrames pertenecientes al SO:\n");
    scanf("%d", &framesDelSOInput);
    *framesDelSO = framesDelSOInput;
    printf("\nFrames por proceso:\n");
    scanf("%d", &framesXProcInput);
    *framesPorProceso = framesXProcInput;
}

void inicializarFrames(Frame tablaFrames[CANT_FRAMES], int framesDelSO)
{
    for (int i = 0; i < CANT_FRAMES; i++)
    {
        tablaFrames[i].frameNum = i;
        if (i < framesDelSO)
        {
            tablaFrames[i].tipo = 'S'; // Frame del SO
            tablaFrames[i].ocupado = true;
        }
        else
        {
            tablaFrames[i].tipo = 'L'; // Frame libre
            tablaFrames[i].ocupado = false;
        }
        tablaFrames[i].pidDuenio = 0;
    }
}

void mostrarTablaFrames(Frame tablaFrames[CANT_FRAMES])
{
    printf("Frame | Ocupado | Tipo | PID Duenio\n");
    printf("-------------------------------\n");
    for (int i = 0; i < CANT_FRAMES; i++)
    {
        printf("%5d | %7s |  %c  | %8d\n",
               tablaFrames[i].frameNum,
               tablaFrames[i].ocupado ? "Si" : "No",
               tablaFrames[i].tipo,
               tablaFrames[i].pidDuenio);
    }
}

void mapearProceso(Frame frame[CANT_FRAMES], Proceso *proceso)
{
    // Tengo que actualizar la tabla de frames y asi cargar el proceso [i] en memoria
    int i = 0;
    while (i < CANT_FRAMES && frame[i].ocupado != false)
    {
        i++;
    }
    for (int j = i; j < i + proceso->cantPag; j++)
    {
        frame[j].ocupado = true;
        frame[j].tipo = 'U';
        frame[j].pidDuenio = proceso->pid;
        proceso->frameDePagina[j - i] = j;
    }
}

void ingresarDatosProceso(Proceso *procesos, int *procesosCargados, int tamanioFrame, Frame frames[CANT_FRAMES])
{
    // Voy al indice del vector en donde tengo que escribir los datos
    int i = *procesosCargados;
    int pidInput, tamanioProcInput;
    char tipoProcInput;
    printf("PID del proceso:\n");
    scanf("%d", &pidInput);
    procesos[i].pid = pidInput;
    printf("Tamanio del proceso en KB:\n");
    scanf("%d", &tamanioProcInput);
    procesos[i].tamanioKB = tamanioProcInput;
    procesos[i].cantPag = ceil((tamanioProcInput + tamanioFrame - 1) / tamanioFrame);
    while (procesos[i].cantPag > framesPorProceso)
    {
        printf("Ingreso equivocado, intente nuevamente:\n");
        scanf("%d", &tamanioProcInput);
        procesos[i].tamanioKB = tamanioProcInput;
        procesos[i].cantPag = ceil((tamanioProcInput + tamanioFrame - 1) / tamanioFrame);
    }
    printf("Tipo de proceso:\n");
    scanf(" %c", &tipoProcInput);
    procesos[i].tipo = tipoProcInput;
    (*procesosCargados)++;
    // Ingreso su PID, tamanio, cantidad de paginas y el tipo de proceso que es
    mapearProceso(frames, &procesos[i]);
    // En la funcion llamada actualizo de manera acorde su tabla de paginas y los bits de validez de cada pagina del proceso
}

void imprimirTablaDePags(Proceso proceso)
{
    printf("Tabla de paginas:\n");
    for (int i = 0; i < proceso.cantPag; i++)
    {
        printf("Pag %d: %d\n", i, proceso.frameDePagina[i]);
    }
}

void simularGrillaLRU()
{
    int arrayPaginas[11] = {1, 2, 3, 4, 2, 5, 6, 3, 7, 5, 8};
    int cantPags = sizeof(arrayPaginas) / sizeof(arrayPaginas[0]);

    int framesProceso[framesPorProceso];
    int tiempoUsado[framesPorProceso];
    int fallosPag[cantPags];

    for (int i = 0; i < framesPorProceso; i++)
    {
        framesProceso[i] = -1;
        tiempoUsado[i] = -1;
    }

    for (int i = 0; i < cantPags; i++)
        fallosPag[i] = 0;

    int instante = 0;

    for (int j = 0; j < cantPags; j++) // Itero por las instancias de accesos a paginas determinadas
    {
        int pagina = arrayPaginas[j]; // Uso la variable para despues encontrar un acierto o asignarsela a un frame
        instante++;

        int existe = -1;
        for (int i = 0; i < framesPorProceso; i++) // Busco en la tabla de frames una coincidencia con 'pagina'
        {
            if (framesProceso[i] == pagina)
            {
                existe = i; // Guardo el 'frame' correspondiente para asi luego referenciarlo al actualizar los tiempos
                break;
            }
        }
        // Actualizo el ultimo instante en que fue utilizada la pagina si encuentro una coincidencia
        if (existe != -1)
            tiempoUsado[existe] = instante;
        else // Hay una falla
        {
            fallosPag[j] = 1;

            int hayFrameLibre = -1;
            // Busco un frame libre. Si lo hay, asigno a ese la pagina actual. En caso contrario, aplicamos LRU
            for (int i = 0; i < framesPorProceso; i++)
            {
                if (framesProceso[i] == -1)
                {
                    hayFrameLibre = i; // Le asigno el nro. de indice para referenciarlo luego
                    break;
                }
            }

            if (hayFrameLibre != -1)
            {
                framesProceso[hayFrameLibre] = pagina; // Le asigno la pagina al frame
                tiempoUsado[hayFrameLibre] = instante;
            }
            else // Aplico LRU quitando la pagina que este en el indice del menor valor del array tiempoUsado
            {
                int menorTiempo = tiempoUsado[0];
                int menorIndice = 0;
                for (int i = 0; i < framesPorProceso; i++)
                {
                    if (tiempoUsado[i] < menorTiempo)
                    {
                        menorTiempo = tiempoUsado[i];
                        menorIndice = i;
                    }
                }
                framesProceso[menorIndice] = pagina;
                tiempoUsado[menorIndice] = instante;
            }
        }
        for (int i = 0; i < framesPorProceso; i++)
        {
            printf(" Frame %d: %d", i, framesProceso[i]);
        }
        printf("\n----------\n");
    }
}

//void returnDirFisica()

void menu()
{
    int opcion;

    while (true)
    {
        printf("Seleccione la accion que desea realizar:\n");
        printf("1) Ingresar parametros del sistema\n");
        printf("2) Mostrar la tabla de frames\n");
        printf("3) Ingresar datos proceso\n");
        printf("4) Mostrar una tabla de paginas\n");
        printf("5) Simular algoritmo LRU (ejecutar 1) anteriormente)\n");
        printf("6) Dada una direccion logica mostrar su direccion fisica\n");
        printf("7) Salir\n");

        scanf("%d", &opcion);

        switch (opcion)
        {
        case 1:
        {
            recibirParametrosSist(&tamanioFrame, &framesDelSO, &framesPorProceso);
            inicializarFrames(frames, framesDelSO);
            break;
        }
        case 2:
        {
            mostrarTablaFrames(frames);
            break;
        }
        case 3:
        {
            // Actualizo procesosCargados con cada carga y cada vez que quiero agregar un proceso, me desplazo dentro del vector procesosCargados espacios
            ingresarDatosProceso(procesos, &procesosCargados, tamanioFrame, frames);
            break;
        }
        case 4:
        {
            int procID, i;
            i = 0;
            Proceso procesoInput;
            printf("Que proceso desea consultar?\n");
            scanf("%d", &procID);
            while (procesos[i].pid != procID)
            {
                i++;
            }
            if (procesos[i].pid == procID)
            {
                procesoInput = procesos[i];
            }
            imprimirTablaDePags(procesoInput);
            break;
        }
        case 5:
        {
            simularGrillaLRU();
            break;
        }
        case 6:
        {
            int procPID, hexOBin, dirLogica, cantDir, cantDigitosBin, cantDigitosHex;
            cantDir = CANT_FRAMES * tamanioFrame * 8;
            cantDigitosBin = (int)ceil(log2(cantDir));
            cantDigitosHex = (int)ceil(log2(cantDir) / 4.0);
            char dirBin[cantDigitosBin + 1];
            char dirHex[cantDigitosHex + 1];
            printf("Ingrese el PID del proceso:\n");
            scanf("%d", &procPID);
            printf("Desea ingresar la direccion correspondiente en hexadecimal o binario (1: hex; 2: binario)\n");
            scanf("%d", &hexOBin);
            int ch;
            while ((ch = getchar()) != '\n' && ch != EOF);
            printf("Direccion logica a consultar:\n");
            if (hexOBin == 1)
            {
                printf("Ejemplos de formatos validos: 1A3F, 0x1A3F, 0x1a3f\n");
                fgets(dirHex, sizeof dirHex, stdin);
                dirHex[strcspn(dirHex, "\n")] = '\0';
                dirLogica = (int)strtol(dirHex, NULL, 16);
                printf("Direccion hexadecimal: %d\n", dirLogica);
            }
            else if (hexOBin == 2)
            {
                scanf("%*c");
                fgets(dirBin, sizeof dirBin, stdin);
                dirBin[strcspn(dirBin, "\n")] = '\0';
                dirLogica = (int)strtol(dirBin, NULL, 2);
                printf("Direccion binaria: %d\n", dirLogica);
            }

            //returnDirFisica(procPID, dirLogica);
        }
        case 7:
        {
            printf("Chau!\n");
            exit(0);
            break;
        }
        default:
            printf("Opcion no valida, vuelva a intentarlo por favor\n");
        }
    }
}

int main()
{
    menu();
    return 0;
}