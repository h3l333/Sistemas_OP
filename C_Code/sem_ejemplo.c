#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/wait.h>  
#include <sys/sem.h>

void P(int, int);
void V(int, int);

int main(void)
{
    int semid = semget(0xa, 0, 0);
    int fd = fopen("abecedario", O_CREAT | O_WRONLY | O_TRUNC, 0600);
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
        printf("Termino mi hijo %d\n");
        printf("Produccion de mi hijo %d letras\n", status/256);
    }
    else //child process
    {
        int cantLetras;
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