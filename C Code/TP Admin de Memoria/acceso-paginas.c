#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define FRAMES_RAM 4
#define SIZE_STRING_REFERENCIAS 11

void inicializarTablaFramesAcceso(int tablaDeFrames[], int framesEnRAM) {
  for (int i = 0; i < framesEnRAM; i++)
    tablaDeFrames[i] = -1;
}

void imprimirTablaFrames(int tablaDeFrames[], int framesEnRAM) {
  for (int i = 0; i < framesEnRAM; i++)
    printf("| Frame %d: Pagina %d |", i, tablaDeFrames[i]);
  printf("\n");
}

int estaEnRAM(int nroPag, int tablaDeFrames[], int framesEnRAM) {
  for (int i = 0; i < framesEnRAM; i++) {
    if (tablaDeFrames[i] == nroPag)
      return 1;
  }
  return 0;
}

int hayFrameLibre(int tablaDeFrames[],
                  int framesEnRAM) // Devuelvo indice libre si si, -1 si no
{
  for (int i = 0; i < framesEnRAM; i++) {
    if (tablaDeFrames[i] == -1)
      return i;
  }
  return -1;
}

void inicializarTablaUltimoAcceso(int ultimoAcceso[]) {
  for (int i = 0; i < 8; i++)
    ultimoAcceso[i] = 0;
}

int ultimoAccesoPag(int ultimoAcceso[], int pagina) {
  for (int i = 0; i < 8; i++) {
    if (i == (pagina - 1)) {
      return ultimoAcceso[i];
    }
  }
  return -1;
}

int devolverFrameVictima(int ultimoAcceso[], int tablaDeFrames[],
                         int framesEnRAM) {
  int indiceFramePagVic = 0;
  int relojMasBajo = ultimoAccesoPag(ultimoAcceso, tablaDeFrames[0]);
  for (int i = 0; i < framesEnRAM; i++) {
    if (ultimoAccesoPag(ultimoAcceso, tablaDeFrames[i]) < relojMasBajo) {
      relojMasBajo = ultimoAccesoPag(ultimoAcceso, tablaDeFrames[i]);
      indiceFramePagVic = i;
    }
  }
  return indiceFramePagVic;
}

void accesoPaginas(int stringReferencias[], int framesEnRAM) {
  int tablaDeFrames[framesEnRAM];
  inicializarTablaFramesAcceso(tablaDeFrames, framesEnRAM);
  int pf_counter = 0;
  int reloj = 0;
  int cantReferencias = SIZE_STRING_REFERENCIAS;
  int ultimoAcceso[8];
  inicializarTablaUltimoAcceso(ultimoAcceso);

  for (int i = 0; i < cantReferencias; i++) {
    int paginaActual = stringReferencias[i];
    int indicePagActual = paginaActual - 1;
    // Me fijo si esta en RAM o no la pagina
    if (estaEnRAM(stringReferencias[i], tablaDeFrames, framesEnRAM) == 0) {
      // Debo aplicar LRU. Busco si hay un frame libre
      pf_counter++;
      if (hayFrameLibre(tablaDeFrames, framesEnRAM) != -1) {
        // Hay frame libre
        tablaDeFrames[hayFrameLibre(tablaDeFrames, framesEnRAM)] =
            stringReferencias[i];
      } else {
        int frameVictima =
            devolverFrameVictima(ultimoAcceso, tablaDeFrames, framesEnRAM);
        tablaDeFrames[frameVictima] = paginaActual;
      }
    }
    ultimoAcceso[indicePagActual] = reloj;
    reloj++;
    imprimirTablaFrames(tablaDeFrames, framesEnRAM);
  }
}