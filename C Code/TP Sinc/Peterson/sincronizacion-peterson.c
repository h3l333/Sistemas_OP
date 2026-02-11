#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

pthread_t hiloProd;
pthread_t hiloConsumo; // punteros a los identificadores de los dos hilos: el
                       // productor y consumidor

int *buf; // puntero al buffer compartido de tamanio dinamico
int bufPos = -1;
int bufLargo;

typedef struct Variables {
  int flags[2];
  int turno;
} Variables;

int producir() {
  int p = 1 + rand() % 100;
  printf("El hilo productor produjo %d\n", p);
  return p;
}

void consumir(int p) {
  printf("Buffer: ");
  for (int i = 0; i <= bufPos; i++)
    printf("%d", *(buf + i));
  printf("\nConsumidor consumio %d\nLargo actual del buffer: %d\n", p, bufPos);
}

void *hiloProductor0(void *arg) {
  while (1) {
    while (bufPos == (bufLargo - 1))
      ;
    Variables *var = (Variables *)arg;
    var->flags[0] = 1;
    var->turno = 1;

    while (var->flags[1] == 1 && var->turno == 1)
      ;
    // CS
    for (int i = 0; i < 1 + rand() % bufLargo; i++) {
      if (bufPos < bufLargo - 1) {
        int p = producir();
        bufPos++;
        *(buf + bufPos) = p;
      } else
        break;
    }

    sleep(rand() % 3);
    var->flags[0] = 0;
  }
}

void *hiloConsumidor1(void *arg) {
  while (1) {
    while (bufPos == -1)
      ;
    Variables *var = (Variables *)arg;
    var->flags[1] = 1;
    var->turno = 0;

    while (var->flags[0] == 1 && var->turno == 0)
      ;
    // CS
    for (int i = 0; i < 1 + rand() % bufLargo; i++) {
      if (bufPos > -1) {
        int c = *(buf + bufPos);
        consumir(c);
        --bufPos;
      } else
        break;
    }

    sleep(1 + rand() % 3);
    var->flags[1] = 0;
  }
}

int main() {
  srand(time(NULL));
  Variables var;
  var.flags[0] = 0;
  var.flags[1] = 0;
  var.turno = 0;

  printf("Capacidad del buffer: ");
  scanf("%d", &bufLargo);
  buf = (int *)malloc(bufLargo * sizeof(int));

  int err;

  // creo productor
  err = pthread_create(&hiloProd, NULL, hiloProductor0, &var);
  if (err != 0)
    printf("Hubo un error al crear al productor\n");

  err = pthread_create(&hiloConsumo, NULL, hiloConsumidor1, &var);
  if (err != 0)
    printf("Hubo un error al crear al consumidor\n");

  pthread_join(hiloProd, NULL);
  pthread_join(hiloConsumo, NULL);

  return 0;
}