#include <stdio.h>
#include <stdlib.h>

#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <mqueue.h>

#define QUEUE_PERMISSIONS 0664
#define MAX_MSG_SIZE 1024
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
    mqd_t cola;
    unsigned int prio;
    char buff[MAX_MSG_SIZE];

    struct mq_attr attr;
    attr.mq_flags = 0;  
    attr.mq_maxmsg = 10;  
    attr.mq_msgsize = 33;  
    attr.mq_curmsgs = 0;  

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
            if ((cola = mq_open (argv[2],  O_RDWR )) < 0) { 
                perror("No se puede acceder a la cola de mensajes"); 
                exit(1); 
            }
            sprintf (buff, "%s", argv[3]);
            prio = (argc > 4) ? atoi(argv[5]): 0;
            if (mq_send(cola, buff, strlen (buff), prio) < 0) {
                perror("Error al enviar el mensaje"); 
                exit(1); 
            }
                printf("Se envió el mensaje a la cola %s \n",argv[2]);
                  mq_close(cola);
            break;
        case 'r':
            printf("Recibe el primer mensaje en %s\n", argv[2]);
            if ((cola = mq_open (argv[2],  O_RDONLY )) < 0) { 
                perror("No se puede acceder a la cola de mensajes"); 
                exit(EXIT_FAILURE);
            }
            if (mq_getattr(cola, &attr) < 0) {
                mq_close(cola);
                perror("No se pudieron obtener los atributos de la cola");
                exit(EXIT_FAILURE);
            }
            prio = (argc > 3) ? atoi(argv[4]): 0;
            ssize_t leidos;
            if ((leidos = mq_receive(cola, buff, sizeof(buff)+10,prio)) < 0) {
                mq_close(cola);
                perror("Error al enviar el mensaje"); 
                exit(1); 
            }
            // buff[leidos] = '\0';
            printf("Se recibió el mensaje [%s] \n",buff);    
            mq_close(cola);
            break;
        case 'a':
            printf("Imprimer todos los mensajes en %s\n", argv[2]);
            break;
        case 'l':
            printf("Escucha indefinidamente por mensajes\n");
            break;
        case 'c':
            printf ("Va a crear la cola %s\n",argv[2]);
            if ((cola = mq_open(argv[2], O_CREAT, QUEUE_PERMISSIONS,&attr)) < 0){
                perror("no se pudo crear la cola de mensajes.\n");
                exit(1);
            }
            printf("Crea la cola de mensajes %s\n", argv[2]);
            mq_close(cola);
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
