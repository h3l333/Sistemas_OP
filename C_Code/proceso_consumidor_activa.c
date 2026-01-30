#include <fcntl.h> //enables file control through shm_open and open
#include <sys/stat.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/mman.h>
#include "shared.h"
#include <stdio.h>

int main()
{   
    
    shared_data *sh; //pointer to shared data
    int fd = shm_open(SHM_NAME, O_RDWR, 0666); //open shared memory region
    void *ptr = mmap(NULL, sizeof(shared_data), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    sh = (shared_data *) ptr;
    
    while(1)
    {
        sh->flags[1] = 1;
        sh->turn = 0;

        while(sh->flags[0] == 1 && sh->turn == 0);
        
        while(sh->buffer.tail == sh->buffer.out) { }
        
        //SC
        printf("\nConsuming the random number %d yum\n", sh->buffer.buffer[sh->buffer.out]);
        sh->buffer.out = (sh->buffer.out + 1) % SLOTS_BUFFER;
        
        sh->flags[1] = 0;

        int seconds = rand();
        sleep(seconds);
    }

    munmap(sh, sizeof(shared_data));
    close(fd);
    return 0;
}