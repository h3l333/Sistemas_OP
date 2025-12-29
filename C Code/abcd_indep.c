// Como se trata de procesos independientes y no de un proceso padre y
// uno hijo, no usaremos ni wait() ni fork()
//
// Vamos a usar lseek(), que reposiciona el puntero de lectura/escritura
// dentro de un archivo abierto
//  off_t lseek(int fildes, off_t offset, int whence)
//
//  lseek(fd, desplazamiento, a partir de donde)
//  si desplazamiento > 0, hacia eof; en caso contrario, hacia bof
//  si el 3er argumento es 0, se hace desde el bof; 1, posición actual
//  y si 3er argumento = 2, eof

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/sem.h>

// ---------- MAYUS ----------

void P(int, int);
void V(int, int);

int main(void)
{
    int semid = semget(0xa, 2, IPC_CREAT | IPC_EXCL | 0600);
    if (errno == EEXIST)
    {
        int semid = semget(0xa, 2, 0);
    }
    int fd = open("abecedario", O_CREAT | O_EXCL | O_TRUNC | O_WRONLY, 0600);
    // Los "flags" que usamos dicen lo siguiente:
    // O_CREAT: "Si el path no existe, crealo como archivo"
    // O_EXCL: Si es usado con O_CREAT, devuelve error en caso de que ya
    //   exista el archivo
    // O_TRUNC: Si el archivo existe y el modo de acceso permite escritura,
    //   lo trunca a tamanio 0 (modo de acceso = WR_ONLY)
    if (errno == EEXIST)
        fd = fopen("abecedario", O_WRONLY);
    int letra = 'B';
    while (letra <= 'Z')
    {
        P(semid, 1); // Obtiene permiso de acceder la seccion critica
        lseek(fd, 0, 2);
        write(fd, &letra, 1);
        V(semid, 0);
        letra += 2;
    }
    fclose(fd);
    exit(0);
}

void P(int semid, int semnum)
{
    struct sembuf op;
    op.sem_num = semnum;
    op.sem_op = -1;
    op.sem_flg = 0;
    semop(semid, &op, 1);
}

void V(int semid, int semnum)
{
    struct sembuf op;
    op.sem_num = semnum;
    op.sem_op = 1;
    op.sem_flg = 0;
    semop(semid, &op, 1);
}