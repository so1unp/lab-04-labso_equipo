#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <fcntl.h>

void usage(char *argv[])
{
    fprintf(stderr, "Uso: %s [OPCION] [PARAMETRO]\n", argv[0]);
    fprintf(stderr, "Opciones:\n");
    fprintf(stderr, "\t-c semaforo valor\tcrea semáforo con el nombre y valor inicial indicado.\n");
    fprintf(stderr, "\t-u semaforo\t\trealiza un UP en el semáforo indicado.\n");
    fprintf(stderr, "\t-d semaforo\t\trealiza un DOWN en el semaforo indicado.\n");
    fprintf(stderr, "\t-b semaforo\t\telimina el semáforo indicado.\n");
    fprintf(stderr, "\t-i semaforo\t\tinformación del semáforo indicado.\n");
    fprintf(stderr, "\t-h\t\t\timprime este mensaje.\n");
}

int main(int argc, char *argv[])
{
    if (argc < 2) {
        usage(argv);
        exit(EXIT_FAILURE);
    }

    if (argv[1][0] != '-') {
        usage(argv);
        exit(EXIT_FAILURE);
    }

    char option = argv[1][1];
    sem_t *semaforo;
    switch(option) {
        case 'c':
            if (argc < 4) {
                usage(argv);
                exit(EXIT_FAILURE);
            }
           if((semaforo=sem_open(argv[2], O_CREAT, 0644, atoi(argv[3])))==(sem_t*)-1) {
               perror("No se puede crear el semáforo"); exit(1); 
            }
            printf("Semáforo %s creado con valor %d\n", argv[2], argv[3]);
            sem_close(semaforo);
            break;
        case 'u':
            break;
        case 'd':
            if (argc < 3) {
                fprintf(stderr, "Falta el nombre del semáforo. Uso: %s -d nombre\n", argv[0]);
                exit(EXIT_FAILURE);
            }
    
            if((semaforo=sem_open(argv[2], 0, 0644, 1))==(sem_t *)-1) {
                printf("No se encuentra el semáforo %s",argv[1]); 
                exit(1);
            }
            // sem_close(semaforo);
            /* Remove named semaphore NAME. */
            if (sem_unlink(argv[2]) < 0) {
                printf("error al cerrar el semáforo %s",argv[1]); 
                exit(1);
            }
            break;
        case 'b':
            break;
        case 'i':
            break;
        case 'h':
            usage(argv);
            break;
        default:
            fprintf(stderr, "Opción desconocida: %s\n", argv[1]);
            exit(EXIT_FAILURE);
    }
    
    exit(EXIT_SUCCESS);
}
