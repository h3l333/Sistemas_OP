#include <stdbool.h>
#include <stdatomic.h>

#define SHM_NAME "/peterson_shm"
#define SLOTS_BUFFER 5

typedef struct bufferCompartido {
    int out;
    int tail;
    int buffer[SLOTS_BUFFER];
} bufferCompartido;

typedef struct shared_data {
    atomic_int flags[2];
    atomic_int turn;
    bufferCompartido buffer;
} shared_data;