#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/sem.h>

// ---------- MINUS ----------

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
    if (errno == EEXIST)
        fd = fopen("abecedario", O_WRONLY);
    int letra = 'b';
    while (letra <= 'z')
    {
        P(semid, 0); // Obtiene permiso de acceder la seccion critica
        lseek(fd, 0, 2);
        write(fd, &letra, 1);
        V(semid, 1);
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