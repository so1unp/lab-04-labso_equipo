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
    int value;

    switch(option) {
        case 'c':
            if (argc < 4) {
                usage(argv);
                exit(EXIT_FAILURE);
            }
            value = atoi(argv[3]);
            if((semaforo=sem_open(argv[2], O_CREAT | O_EXCL, 0644, value))==SEM_FAILED) {
               perror("No se puede crear el semáforo"); 
               exit(1); 
            }
            printf("Semáforo %s creado con valor %i \n", argv[2], atoi(argv[3]));
            sem_close(semaforo);
            break;
        case 'u':
            if (argc < 3) {
                fprintf(stderr, "Falta el nombre del semáforo. Uso: %s -u nombre\n", argv[0]);
                exit(EXIT_FAILURE);
            }
            // abrir el semaforo
            if((semaforo=sem_open(argv[2], 0))==SEM_FAILED) {
                fprintf(stderr,"No se pudo abrir el semáforo: %s",argv[2]);
                exit(EXIT_FAILURE);
            }
            // logica de modificar
            printf("Hace UP del semáforo\n");
            sem_post(semaforo);
            printf("Cierra semaforos y termina\n");
            sem_close(semaforo);
            break;
        case 'd':
            if (argc < 3) {
                fprintf(stderr, "Falta el nombre del semáforo. Uso: %s -d nombre\n", argv[0]);
                exit(EXIT_FAILURE);
            }
            // abrir el semaforo
            if((semaforo=sem_open(argv[2],0))==SEM_FAILED) {
                fprintf(stderr,"No se pudo abrir el semáforo: %s",argv[2]);
                exit(EXIT_FAILURE);
            }
            // logica de modificar
            printf("Hace DOWN del semáforo\n");
            sem_wait(semaforo);
            printf("Cierra semaforos y termina\n");
            sem_close(semaforo);
            break;
        case 'b':
            if (argc < 3) {
                fprintf(stderr, "Falta el nombre del semáforo. Uso: %s -b nombre\n", argv[0]);
                exit(EXIT_FAILURE);
            }
            if (sem_unlink(argv[2]) < 0) {
                fprintf(stderr,"error al borrar el semáforo %s",argv[2]); 
                exit(EXIT_FAILURE);
            }
            printf("Semaforo %s eliminado\n",argv[2]);
            break;
        case 'i':
            // abrir el semaforo
            if((semaforo=sem_open(argv[2], 0))==SEM_FAILED) {
                fprintf(stderr,"No se pudo abrir el semáforo: %s",argv[2]);
                exit(EXIT_FAILURE);
            }
            // consulta estado
            sem_getvalue(semaforo,&value);
            printf("El valor del semáforo %s es %d \n",argv[2],value);
            sem_close(semaforo);         
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
