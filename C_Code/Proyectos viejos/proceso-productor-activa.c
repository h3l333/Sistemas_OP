#include <fcntl.h> //enables file control through shm_open and open
#include <sys/stat.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/mman.h>
#include "shared.h"
#include <stdio.h>

void inicializar(shared_data *sh)
{
    sh->buffer.out = 0;
    sh->buffer.tail = 0;
    sh->buffer.buffer = {-1, -1, -1, -1, -1};
    sh->flags[0] = 0;
    sh->flags[1] = 0;
    sh->turn = 0;
}

int main()
{   
    int fd = shm_open(SHM_NAME, O_CREAT | O_EXCL | O_RDWR, 0666); //open shared memory region or create it
    shared_data *sh; //pointer to shared data

    //We create the space and a pointer to the space and assign that pointer's value to sh
    if(fd >= 0)
    {
        ftruncate(fd, sizeof(shared_data)); //give it a size if it's successfully been created
        void *ptr = mmap(NULL, sizeof(shared_data), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
        //The OS chooses the starting virtual address (NULL lets the kernel choose any suitable unused VA range),
        //size of structure in shared memory, what the 
        //process can do, writes to the region are visible to processes that map the same object
        //We identify what shared memory object to map and define the offset
        sh = (shared_data *) ptr;
        inicializar(sh); //initialise variables so that they don't store garbage
    }
    else
    {
        fd = shm_open(SHM_NAME, O_RDWR, 0666); //if it already exists, just open it
        void *ptr = mmap(NULL, sizeof(shared_data), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
        sh = (shared_data *) ptr;
    }
    
    srand(getpid());

    while(1)
    {
        sh->flags[0] = 1;
        sh->turn = 1;

        while(sh->flags[1] == 1 && sh->turn == 1);
        
        while((sh->buffer.tail + 1) % SLOTS_BUFFER == sh->buffer.out) { }
        
        //SC
        int randomNum = rand();
        sh->buffer.buffer[sh->buffer.tail] = randomNum;
        sh->buffer.tail = (sh->buffer.tail + 1) % SLOTS_BUFFER;
        printf("\nProduced %d!\n", randomNum);

        sh->flags[0] = 0;

        int seconds = rand();
        sleep(seconds);
    }

    munmap(sh, sizeof(shared_data));
    close(fd);
    return 0;
}