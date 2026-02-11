#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

pthread_t *productores;
pthread_t *consumidores;
// punteros a variables pthread_t (thread IDs)

sem_t bufMutex, contVacio, contLleno; // sem_t es un typedef o estructura de
                                      // datos que representa a un semaforo.
// bufMutex representa a un semaforo que sirve el rol de asegurar que solo un
// hilo a la vez acceda a su seccion critica.
// contVacio mantiene registro de la cantidad de espacios vacios en el buffer
// compartido contLleno mantiene registro de la cantidad de espacios llenos en
// el buffer compartido

int *buf; // puntero al buffer compartido
int bufPos =
    -1; //-1 indica que el buffer esta vacio. 0 que hay un objeto en el primer
        //indice, 1 que hay objetos hasta el segundo indice, etc.
int bufLargo, contProd,
    contConsumo; // indican el largo del buffer, cantidad de productores y
                 // consumidores respectivamente

int produce(pthread_t id) // pasamos id del hilo para imprimir que hilo produce
                          // que numero
{
  int i =
      0; // inicializamos el indice antes de buscar en el array de productores
  int p = 1 + rand() % 100; // generamos un numero
  while (i < contProd && !pthread_equal(*(productores + i),
                                        id)) // escribiendo *(productores + i)
    // accedemos al valor en el indice i del array dinamico de productores y
    // comparamos con el id pasado como argumento
    i++;

  printf("Productor %d produjo %d\n", i + 1, p);
  return p; // retornamos numero generado
}

void consume(int p, pthread_t id) {
  int i = 0;
  while (i < contConsumo &&
         !pthread_equal(*(consumidores + i),
                        id)) // productores[i] == *(productores + i);
    i++; // buscamos indice correspondiente para imprimir que consumidor
         // consumio que

  printf("Buffer: ");
  for (i = 0; i <= bufPos; i++)
    printf("%d", *(buf + i)); // imprimimos buffer antes de consumo
  printf("\nConsumidor %d consumio %d\nLargo actual del buffer: %d\n", i + 1, p,
         bufPos);
  // Indice del consumidor, nro. producido devuelto originalmente por produce(),
  // posicion del buffer pre-consumo
}

void *productor(void *args) {
  while (1) {
    sem_wait(&contVacio);
    sem_wait(&bufMutex);
    // CS
    int p = produce(pthread_self());
    ++bufPos;
    *(buf + bufPos) = p; // Asignamos valor al indice del buffer apropiado
    sem_post(&bufMutex);
    sem_post(&contLleno);
    sleep(1 + rand() % 3);
  }
  return NULL;
}

void *consumidor(void *args) {
  int c;
  while (1) {
    sem_wait(&contLleno);
    sem_wait(&bufMutex);
    // CS
    c = *(buf + bufPos); // obten el valor del buffer que debe ser consumido
    consume(c, pthread_self());
    --bufPos;
    sem_post(&bufMutex);
    sem_post(&contVacio);
    sleep(1 + rand() % 5);
  }
  return NULL;
}

int main(void) {
  int i, err;
  srand(time(NULL)); // habilita que rand() haga su trabajo correctamente

  sem_init(&bufMutex, 0, 1);
  sem_init(&contLleno, 0, 0); // inicializo semaforos

  printf("Cuantos consumidores seran activos?\n");
  scanf("%d", &contConsumo);
  productores = (pthread_t *)malloc(contConsumo * sizeof(pthread_t));
  // reserva espacio para el array dinamico "productores"

  printf("Cuantos productores seran activos?\n");
  scanf("%d", &contProd);
  consumidores = (pthread_t *)malloc(contProd * sizeof(pthread_t));

  printf("Capacidad del buffer: ");
  scanf("%d", &bufLargo);
  buf = (int *)malloc(bufLargo * sizeof(int));

  sem_init(&contVacio, 0, bufLargo);

  for (i = 0; i < contProd; i++) {
    err = pthread_create(productores + i, NULL, &productor, NULL);
    // puntero a donde se guardara el puntero al ID del hilo
    // puntero al objeto de atributos del hilo. NULL = hilo default
    // puntero a la funcion que va a ejecutar
    // argumento pasado a la funcion del hilo. NULL si no se necesita argumento
    if (err != 0)
      printf("Error en crear el productor %d\n", i + 1);
    else
      printf("Se creo el productor %d\n", i + 1);
  }

  for (i = 0; i < contConsumo; i++) {
    err = pthread_create(consumidores + i, NULL, &consumidor, NULL);
    if (err != 0)
      printf("Error en crear el consumidor %d\n", i + 1);
    else
      printf("Se creo el consumidor %d\n", i + 1);
  }

  for (i = 0; i < contProd; i++)
    pthread_join(*(productores + i), NULL);
  // ID del hilo cuya por cuya ejecucion deseo esperar, valor de retorno de la
  // funcion del hilo Itero por los productores y espero a que cada uno termine

  for (i = 0; i < contConsumo; i++)
    pthread_join(*(consumidores + i), NULL);

  return 0;
}