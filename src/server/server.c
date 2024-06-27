#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <time.h>

#include "message_types.h"
#include "message.h"
#include "user.h"
#include "user_table_functions.h"

#define IP_LENGTH 16

int main() {


    int socketserver;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    struct message recv_msg , send_msg;
    char client_ip[IP_LENGTH];
    int client_port;

    // Crear el socket
    socketserver = socket(AF_INET, SOCK_DGRAM, 0);
    if (socketserver == -1) {
        perror("FAIL TO CREATE SOCKET");
        exit(EXIT_FAILURE);
    }

    // Configurar la dirección del servidor
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(SERVER_PORT);

    // Vincular el socket con la dirección y el puerto
    if (bind(socketserver, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("FAIL TO BIND");
        close(socketserver);
        exit(EXIT_FAILURE);
    }

    printf("[-]Server listening on port: %d\n", SERVER_PORT);
    printf("[-]Server esperando por mensajes\n");

    while (1) {
        // Recibir el mensaje
        ssize_t recv_len = recvfrom(socketserver, &recv_msg, sizeof(recv_msg), 0, (struct sockaddr *)&client_addr, &client_addr_len);

        if (recv_len == -1) {
            perror("Fail to receive message");
            close(socketserver);
             exit(EXIT_FAILURE);
        }

        if(recv_msg.type == I_AM)
        {
            printf("[-]se recibio un mensaje del tipo (I AM) | registrando ('%s') \n",recv_msg.from);
            
            char client_ip[MAX_USER_IP_LENGTH];
            inet_ntop(AF_INET, &(client_addr.sin_addr), client_ip, INET_ADDRSTRLEN);

            client_port = ntohs(client_addr.sin_port);

            add_user(recv_msg.payload,client_ip,client_port,false,true,time(NULL));
            
            send_msg.type = NEW_USER;
            strcpy(send_msg.payload,"OK");
            ssize_t send_len = sendto(socketserver, &send_msg, sizeof(send_msg), 0, (struct sockaddr *)&client_addr, client_addr_len);

        }

        if(recv_msg.type == I_AM_LIVE)
        {
            printf("[-]se recibio un mensaje del tipo (I AM ALIVE) | actualizando estado \n");
            //printf("from: %s",recv_msg.from); //<---- aqui marca violacion del segmento ayuda aqui GTP
            update_user_lastconnection(recv_msg.from,true);          
        }



        // Convertir la dirección IP del cliente a formato legible
        inet_ntop(AF_INET, &client_addr.sin_addr, client_ip, IP_LENGTH);

        // Imprimir la información recibida
        printf("[-]recibido desde => %s:%d\n", client_ip, ntohs(client_addr.sin_port));


        //save user
        if(recv_msg.type == I_NEED_TALK){
            printf("[-]se recibio un mensaje del tipo (I NEED TALK) | preparando inicio de comunicacion");
            
              struct User *peer_user = get_user(recv_msg.payload);

            if (peer_user && peer_user->is_busy != true) {
                printf("[-]Encontrado usuario: %s | IP: %s | Puerto: %d\n", peer_user->username, peer_user->ip, peer_user->port);
                send_msg.type = PEER_INFO;
                snprintf(send_msg.payload, sizeof(send_msg.payload), "%s:%d", peer_user->ip, peer_user->port);
                sendto(socketserver, &send_msg, sizeof(send_msg), 0, (struct sockaddr *)&client_addr, client_addr_len);
            } else {
                printf("[-]Usuario no encontrado: %s\n", recv_msg.payload);
                send_msg.type = PEER_INFO;
                strcpy(send_msg.payload, "USER_NOT_FOUND");
                sendto(socketserver, &send_msg, sizeof(send_msg), 0, (struct sockaddr *)&client_addr, client_addr_len);
            }
        }

    }

    // Cerrar el socket
    close(socketserver);
    return 0;
}
