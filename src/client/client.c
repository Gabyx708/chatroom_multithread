#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <omp.h>

#include "message_functions.h"
#include "message_types.h"
#include "message.h"
#include "proto.h"

#define LENGHT_NAME 20

//variables globales
int socketclient;
struct sockaddr_in server_addr;
struct sockaddr_in client_addr;

int main() {
    
    //creacion socket
    socketclient = socket(AF_INET, SOCK_DGRAM, 0);
    if (socketclient == -1) {
        perror("Fail to create socket");
        exit(EXIT_FAILURE);
    }

    // Configurar la dirección del servidor
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);

    if (inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr) <= 0) {
        perror("Invalid address/ Address not supported");
        close(socketclient);
        exit(EXIT_FAILURE);
    }

    //presentarse
    char nickname[50];
    printf("[*] introduce tu nombre de usuario: ");
    scanf("%s",nickname);

    printf("[*] registrando usuario:('%s') | realizando llamada al servidor \n",nickname);
    printf("[*]aguarde un instante \n");

    struct message i_am_msg;
    i_am_msg.type = I_AM;
    strcpy(i_am_msg.payload,nickname);
    strcpy(i_am_msg.from,nickname);

    ssize_t send = sendto(socketclient,&i_am_msg,sizeof(i_am_msg),0,(struct sockaddr *)&server_addr,sizeof(server_addr));

    if(send == -1){
        printf("ERROR");
        close(socketclient);
        exit(EXIT_FAILURE);
    }
    
    //esperando respuesta del servidor
    struct message recv_msg;

    ssize_t recv = recvfrom(socketclient, &recv_msg, sizeof(recv_msg), 0, (struct sockaddr *)NULL,NULL);

    if (recv < 0) {
        perror("Error al recibir el mensaje");
        exit(EXIT_FAILURE);
    }

    if(recv_msg.type == NEW_USER)
    {
        printf("[*]el servidor registro como nuevo usuario | respuesta (%s)\n",recv_msg.payload);
        printf("[*]\n");
    }

    
    #pragma omp parallel sections
    {
        #pragma omp section
        {   
            //este hilo debe notificar al servidor que el cliente esta vivo
            int id_thread = omp_get_thread_num();
            struct message i_am_alive;
            i_am_alive.type = I_AM_LIVE;
            strcpy(i_am_alive.from,nickname);

            FILE *file;

            file = fopen("log.txt","a");

            while (1)
            {

            // Enviar el mensaje
            if (sendto(socketclient, &i_am_alive, sizeof(i_am_alive), 0, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
            {
                fprintf(file,"(ERROR)============================== | thread (%d)\n",id_thread);
                perror("Fail to send message");
                close(socketclient);
                exit(EXIT_FAILURE);
            }

            fprintf(file,"(ALIVE) message sent to %s:%d | thread (%d)\n", SERVER_IP, SERVER_PORT,id_thread);
            fflush(file); //escribe en el archivo
            sleep(1); //descansa por 2 segundos
            }
            
        }

        #pragma omp section
        {
            // consulta para obtener la IP del peer
            char peer_nickname[30];
            printf("[*]introduce el nombre de usuario con quien deseas hablar:");
            scanf("%s", peer_nickname);
            printf("[*]enviando consulta de usuario al servidor | consulta: (%s)", peer_nickname);

            struct message get_peer_msg;
            get_peer_msg.type = I_NEED_TALK;
            strcpy(get_peer_msg.to, nickname);
            strcpy(get_peer_msg.payload, peer_nickname);
            sendto(socketclient, &get_peer_msg, sizeof(get_peer_msg), 0, (struct sockaddr *)&server_addr, sizeof(server_addr));

            printf("[*]se recibio %s",recv_msg.payload);
        }
    }


    // Cerrar el socket
    close(socketclient);
    return 0;
}
