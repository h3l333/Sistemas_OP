#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/wait.h>  
#include <sys/sem.h>
#include "PyV.h"

void P(int, int);
void V(int, int);

int main(void)
{
    union semun { int val; };
    union semun arg;

    int semid = semget(0xa, 2, IPC_CREAT | 0600);
    arg.val = 1;
    semctl(semid, 0, SETVAL, 1); //proceso padre permitido
    arg.val = 0;
    semctl(semid, 1, SETVAL, 0); //proceso hijo bloqueado
    int fd = open("abecedario", O_CREAT | O_WRONLY | O_TRUNC, 0600);
    pid_t pid, ter;
    int status = 0;
    char letra;
    pid = fork();
    if (pid > 0) //parent process
    {
        letra = 'a';
        while(letra <= 'z')
        {
            P(semid, 0);
                write(fd, &letra, 1);
            V(semid, 1);
            letra+=2;
        }
        ter = wait(&status);
        printf("Termino mi hijo %d\n", ter);
        printf("Produccion de mi hijo %d letras\n", status/256);
    }
    else //child process
    {
        int cantLetras = 0;
        letra = 'B';
        while(letra <= 'Z')
        {
            //write to file
            P(semid, 1);
                write(fd, &letra, 1);
            V(semid, 0);
            letra+=2;
            cantLetras++;
        }
        exit(cantLetras);
    }
    exit(0);
}
