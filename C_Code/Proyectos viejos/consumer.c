#include "shared_memory.h"
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/mman.h>
#include <unistd.h>

int main()
{
    int fd = shm_open(SHARED_MEMORY, O_RDWR, 0666);
    if(fd < 0)
    {
        perror("shm_open");
        return 1;
    }

    sharedMemory *sh = (sharedMemory *) mmap(NULL, sizeof(sharedMemory), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if(sh == MAP_FAILED)
    {
        perror("mmap");
        return 1;
    }

    while(1)
    {
        // Peterson's entry section
        sh->flags[1] = 1;
        sh->turn = 0;
        while(sh->flags[0] == 1 && sh->turn == 0) {}

        // Wait until there is something to consume
        while(sh->buffer.free_index == sh->buffer.full_index) {}

        // Consume the value
        int value = sh->buffer.buffer[sh->buffer.full_index];
        printf("Consuming number %d from index %d\n", value, sh->buffer.full_index);
        sh->buffer.buffer[sh->buffer.full_index] = -1; // optional: clear slot
        sh->buffer.full_index = (sh->buffer.full_index + 1) % BUFFER_SLOTS;

        // Peterson's exit section
        sh->flags[1] = 0;

        sleep(1); // slow down for observation
    }

    return 0;
}