#define SHARED_MEMORY "/shared_memory_busy"
#define BUFFER_SLOTS 5

typedef struct bufferCompartido
{
    int free_index; //next free index
    int full_index; //first full index
    int buffer[BUFFER_SLOTS]; //shared buffer
}bufferCompartido;

typedef struct sharedMemory
{
    int flags[2]; //banderas del algoritmo de Peterson
    int turn; //variable "turn"
    bufferCompartido buffer;
}sharedMemory;
