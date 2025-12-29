// En este ejemplo el padre y el hijo escriben de manera intercalada
// el abecedario dentro de un archivo compartido
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/sem.h>

void P(int, int);
void V(int, int);

int main(void)
{
    int semid = semget(0xa, 2, IPC_CREAT | 0600);
    semctl(semid, 0, SETVAL, 1);
    semctl(semid, 1, SETVAL, 0);
    int fd = open("abcd", O_CREAT | O_WRONLY | O_TRUNC, 0600);
    pid_t pid, ter;
    int status = 0;
    char letter;
    pid = fork();

    if (pid > 0) // padre
    {
        letter = 'a';
        while (letter <= 'z')
        {
            P(semid, 0);           // SECCION ENTRADA
            write(fd, &letter, 1); // SECCION CRITICA
            V(semid, 1);           // SECCION SALIDA
            letter += 2;
        }
        ter = wait(&status);
        printf("Termino mi hijo %d\n", ter);
        printf("Produccion de mi hijo %d letras\n", status / 256);
    }
    else // hijo
    {
        int cantLetras = 0;
        letter = 'B';
        while (letter <= 'Z')
        {
            P(semid, 1);           // SECCION ENTRADA
            write(fd, &letter, 1); // SECCION CRITICA
            V(semid, 0);           // SECCION SALIDA
            letter += 2;
            cantLetras++;
        }
        exit(cantLetras);
    }
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