#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

#define TAMANIO_BUFFER 5

void p()
{
    while sem <= 0;
    sem--;
}

void v()
{
    sem++;
}

//

void productor()
{
    while(contador == TAMANIO_BUFFER);

    //aniadir elemento al buffer
    ++contador;
    buffer[in] = elemento;
    in = (in + 1) % TAMANIO_BUFFER;
    
}

void consumidor()
{
    while(contador == 0);
    
    //sacar un elemento del buffer
    --contador;
    elemento = buffer[];
    out = (out + 1) % TAMANIO_BUFFER;
}