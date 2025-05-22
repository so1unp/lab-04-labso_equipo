#include <stdio.h>
#include <stdlib.h>

#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <mqueue.h>

#define QUEUE_PERMISSIONS 0664
#define MSG_BUFFER_SIZE MAX_MSG_SIZE+10


#define USERNAME_MAXSIZE    15  // Máximo tamaño en caracteres del nombre del remitente.
#define TXT_SIZE            100 // Máximo tamaño del texto del mensaje.

/**
 * Estructura del mensaje:
 * - sender: nombre del usuario que envió el mensaje.
 * - text: texto del mensaje.
 */
struct msg {
    char sender[USERNAME_MAXSIZE];
    char text[TXT_SIZE];
};

typedef struct msg msg_t;

/**
 * Imprime información acerca del uso del programa.
 */
void usage(char *argv[])
{
    fprintf(stderr, "Uso: %s comando parametro\n", argv[0]);
    fprintf(stderr, "Comandos:\n");
    fprintf(stderr, "\t-s queue mensaje: escribe el mensaje en queue.\n");
    fprintf(stderr, "\t-r queue: imprime el primer mensaje en queue.\n");
    fprintf(stderr, "\t-a queue: imprime todos los mensaje en queue.\n");
    fprintf(stderr, "\t-l queue: vigila por mensajes en queue.\n");
    fprintf(stderr, "\t-c queue: crea una cola de mensaje queue.\n");
    fprintf(stderr, "\t-d queue: elimina la cola de mensajes queue.\n");
    fprintf(stderr, "\t-h imprime este mensaje.\n");
}

int main(int argc, char *argv[])
{
    mqd_t cola;                  //descriptor de la cola
    struct mq_attr attr;         //atributos de la cola
    int mqsize, msgsize;
 
    if (argc < 2) {
        usage(argv);
        exit(EXIT_FAILURE);
    }

    if (argv[1][0] != '-') {
        usage(argv);
        exit(EXIT_FAILURE);
    }

    char option = argv[1][1];

    switch(option) {
        case 's':
            printf("Enviar %s a la cola %s\n", argv[3], argv[2]);
            break;
        case 'r':
            printf("Recibe el primer mensaje en %s\n", argv[2]);
            break;
        case 'a':
            printf("Imprimer todos los mensajes en %s\n", argv[2]);
            break;
        case 'l':
            printf("Escucha indefinidamente por mensajes\n");
            break;
        case 'c':
            printf ("Va a crear la cola %s\n",argv[2]);
            if (cola = mq_open(argv[2], O_CREAT, QUEUE_PERMISSIONS) < 0){
                perror("no se pudo crear la cola de mensajes.\n");
                exit(1);
            }
            printf("Crea la cola de mensajes %s\n", argv[2]);
            break;
        case 'd':
            printf("Borra la cola de mensajes %s\n", argv[2]);
            mq_unlink(argv[2]);
            printf ("Se ha eliminado la cola %s\n",argv[2]);
            break;
        case 'h':
            usage(argv);
            break;
        default:
            fprintf(stderr, "Comando desconocido: %s\n", argv[1]);
            exit(EXIT_FAILURE);
    }
    
    exit(EXIT_SUCCESS);
}
