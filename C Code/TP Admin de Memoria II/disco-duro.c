#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int tamanioBloqueFisico;
int bloquesRestantes;

typedef struct Bloque {
  int idArchivoAlojado;
  int ocupado; // 1 si esta ocupado, 0 si esta libre
} Bloque;

typedef struct DiscoRigido {
  int tamanioDiscRigido;
  int cantBloquesDisco;
  Bloque *puntBloquesDisco;
} DiscoRigido;

typedef struct Archivo {
  int fileID;
  char nombre[50];
  int tamanioKBFile;
  int tamanioRegLogico; // Tamanio de unidad de informacion significativa
  float factorBloq;     // Cuantos registros logicos entran en un bloque-
                        // tamanioBloqueFisico/tamanioRegLogico
  int *directorio;      // Array dinamico, cada valor en un indice representa un
                        // bloque en el HD
} Archivo;

// Array que guarda archivos en el orden en que se cargaron. Luego de que se
// ingresen parametros se calcula un maximo posible realizando el calculo
// tamanioDiscRigido/bloquesRestantes
Archivo *arrayArchivos;
int cantArchivos = 0;

DiscoRigido discoRigido;

void inicializarDirectorio(Archivo *nuevoArchivo, Bloque *arrayDisco) {
  int cantidadBloquesFisicos =
      nuevoArchivo->tamanioKBFile / tamanioBloqueFisico;
  nuevoArchivo->directorio =
      (int *)malloc(cantidadBloquesFisicos * sizeof(int));
  int j = 0;
  while ((arrayDisco + j)->ocupado != 0)
    j++;
  for (int i = 0; i < cantidadBloquesFisicos; i++) {
    (arrayDisco + i + j)->idArchivoAlojado = nuevoArchivo->fileID;
    (arrayDisco + i + j)->ocupado = 1;
    nuevoArchivo->directorio[i] = i + j;
  }
  printf("Directorio del archivo %d:\n", nuevoArchivo->fileID);
  for (int i = 0; i < cantidadBloquesFisicos; i++) {
    printf("Bloque %d: Bloque fisico %d\n", i, nuevoArchivo->directorio[i]);
  }
}

void ingresarDatosArchivo(int tamanioBloqueFisico, Bloque *arrayDisco) {
  Archivo *nuevoArchivo = malloc(sizeof(Archivo));
  printf("ID del archivo:\n");
  scanf("%d", &nuevoArchivo->fileID);
  printf("Tamanio en KB:\n");
  scanf("%d", &nuevoArchivo->tamanioKBFile);
  while (nuevoArchivo->tamanioKBFile >
         (bloquesRestantes * tamanioBloqueFisico)) {
    printf("No hay suficiente espacio en el disco para guardar su archivo.\n");
    printf("Tamanio en KB:\n");
    scanf("%d", &nuevoArchivo->tamanioKBFile);
  }
  while (getchar() != '\n')
    ;
  printf("Nombre del archivo:\n");
  scanf("%s", nuevoArchivo->nombre);
  printf("Tamanio del registro logico:\n");
  scanf("%d", &nuevoArchivo->tamanioRegLogico);
  nuevoArchivo->factorBloq =
      (float)(tamanioBloqueFisico / nuevoArchivo->tamanioRegLogico);
  inicializarDirectorio(nuevoArchivo, arrayDisco);
  *(arrayArchivos + cantArchivos) = *nuevoArchivo;
  cantArchivos++;
  printf("Archivos en el disco: ");
  for (int i = 0; i < cantArchivos; i++) {
    printf("%d, ", (*(arrayArchivos + i)).fileID);
  }
  printf("\n");
}

void inicializarDiscoRigido(DiscoRigido *discoRigido, int cantBloquesDisco) {
  discoRigido->puntBloquesDisco =
      (Bloque *)malloc(cantBloquesDisco * sizeof(Bloque));
  for (int i = 0; i < cantBloquesDisco; i++) {
    (discoRigido->puntBloquesDisco + i)->idArchivoAlojado = -1;
    (discoRigido->puntBloquesDisco + i)->ocupado = 0;
  }
  for (int i = 0; i < cantBloquesDisco; i++) {
    printf("Bloque Fisico %d\n", i);
    printf("    Id del archivo alojado: %d\n",
           (discoRigido->puntBloquesDisco + i)->idArchivoAlojado);
    printf("    Esta ocupado: %d\n",
           (discoRigido->puntBloquesDisco + i)->ocupado);
    printf("---------------\n");
  }
}

void ingresarTamanios(DiscoRigido *discoRigido, int *tamanioBloqueFisico,
                      int *bloquesRestantes) {
  printf("Tamanio del disco rigido en KB:\n");
  scanf("%d", &(discoRigido->tamanioDiscRigido));
  printf("Tamanio del bloque fisico en KB:\n");
  scanf("%d", tamanioBloqueFisico);
  discoRigido->cantBloquesDisco =
      discoRigido->tamanioDiscRigido / (*tamanioBloqueFisico);
  *bloquesRestantes = discoRigido->cantBloquesDisco;
  inicializarDiscoRigido(discoRigido, discoRigido->cantBloquesDisco);
  arrayArchivos = (Archivo *)malloc(*bloquesRestantes * sizeof(Archivo));
}

void consultarRegistroLogico() {
  int archivoId;
  printf("A que archivo pertenece el registro logico que quiere consultar?\n");
  scanf("%d", &archivoId);
  int i = 0;
  while (i < cantArchivos && (arrayArchivos + i)->fileID != archivoId) {
    i++;
  }
  if (i == cantArchivos) {
    printf("No existe en el disco un archivo con ese ID\n");
    return;
  }

  int nroRegistroLogico;
  printf("Nro. del registro logico a consultar:\n");
  scanf("%d", &nroRegistroLogico);
  while (nroRegistroLogico > (arrayArchivos + i)->tamanioKBFile /
                                 (arrayArchivos + i)->tamanioRegLogico) {
    printf("Ingreso invalido. Vuelva a intentar\n");
    scanf("%d", &nroRegistroLogico);
  }
  // Recorrer array "directorio"
  printf("El registro logico %d esta en el bloque fisico %d\n",
         nroRegistroLogico,
         (*((arrayArchivos + i)->directorio) +
          (int)(nroRegistroLogico / (arrayArchivos + i)->factorBloq)));
}

void menu() {
  int opcion;
  do {
    printf("Que operacion desea realizar?\n");
    printf("1) Ingresar el tamanio del disco rigido y bloque fisico\n");
    printf("2) Imprimir la cantidad de bloques en el disco rigido\n");
    printf("3) Ingresar los datos de un archivo\n");
    printf("4) Consultar el bloque fisico de un registro logico\n");
    printf("0) Salir\n");
    scanf("%d", &opcion);

    switch (opcion) {
    case 1: {
      ingresarTamanios(&discoRigido, &tamanioBloqueFisico, &bloquesRestantes);
      printf("Tamanio disco rigido: %d KB\nTamanio bloque fisico: %d KB\n",
             discoRigido.tamanioDiscRigido, tamanioBloqueFisico);
      break;
    }
    case 2: {
      printf("Cantidad de bloques en el disco: %d\n",
             discoRigido.cantBloquesDisco);
      break;
    }
    case 3: {
      ingresarDatosArchivo(tamanioBloqueFisico, discoRigido.puntBloquesDisco);
      break;
    }
    case 4: {
      consultarRegistroLogico();
      break;
    }
    case 0: {
      printf("Chau!\n");
      exit(0);
      break;
    }
    default:
      printf("Opcion invalida. Vuelva a intentar.\n");
    }
  } while (opcion != 0);
}

int main() {
  menu();
  return 0;
}