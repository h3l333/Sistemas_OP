#include "shared_memory.h"
#include <fcntl.h>
#include <stdlib.h>
#include <time.h>

void initialiseSharedMemory(sharedMemory * sh)
{
    sh->flags[0] = 0;
    sh->flags[1] = 0;
    sh->turn = 0;
    sh->buffer.full_index = 0;
    sh->buffer.free_index = 0;
    for(int i = 0; i<BUFFER_SLOTS; i++)
        sh->buffer.buffer[i] = -1;
}

void writeToSharedMem(sharedMemory * sh)
{
    int randomNum = rand() % 100;
    printf("\nProducing the number %d to index %d...\n", randomNum, sh->buffer.free_index);
    sh->buffer.buffer[sh->buffer.free_index] = randomNum;
    sh->buffer.free_index = (sh->buffer.free_index + 1)%BUFFER_SLOTS;
    sleep(1);
}

int main()
{
    //Create shared memory space, use O_EXCL to verify if it already exists
    int fd = shm_open(SHARED_MEMORY, O_CREAT | O_EXCL | O_RDWR, 0666);
    sharedMemory *sh;
    srand(time(NULL));
    if(fd<0) //Ya existia u ocurrio un error
    {
        void * ptr = mmap(NULL, sizeof(sharedMemory), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
        sh = (sharedMemory *) ptr;

        sh->flags[0] = 1;
        sh->turn = 1;
        while(sh->flags[1] == 1 && sh->turn == 1) {}

        while((sh->buffer.free_index + 1)%BUFFER_SLOTS == sh->buffer.full_index) {}
        writeToSharedMem(sh);
        
        sh->flags[0] = 0;
    }
    else //Primera iteracion del proceso
    {
        ftruncate(fd, sizeof(sharedMemory)); //give it a size if it's successfully been created
        //Create a pointer to the shared memory space
        void * ptr = mmap(NULL, sizeof(sharedMemory), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
        sh = (sharedMemory *) ptr;
        initialiseSharedMemory(sh);

        sh->flags[0] = 1;
        sh->turn = 1;
        while(sh->flags[1] == 1 && sh->turn == 1) {}

        while((sh->buffer.free_index + 1)%BUFFER_SLOTS == sh->buffer.full_index) {}
        writeToSharedMem(sh);
        
        sh->flags[0] == 0;
    }
}