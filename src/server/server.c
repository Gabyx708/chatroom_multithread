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
    struct message recv_msg;
    char client_ip[IP_LENGTH];

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

    printf("Server listening on port %d\n", SERVER_PORT);

    while (1) {
        // Recibir el mensaje
        ssize_t recv_len = recvfrom(socketserver, &recv_msg, sizeof(recv_msg), 0, (struct sockaddr *)&client_addr, &client_addr_len);
        if (recv_len == -1) {
            perror("Fail to receive message");
            close(socketserver);
            exit(EXIT_FAILURE);
        }

        // Convertir la dirección IP del cliente a formato legible
        inet_ntop(AF_INET, &client_addr.sin_addr, client_ip, IP_LENGTH);

        // Imprimir la información recibida
        printf("Received message from %s:%d\n", client_ip, ntohs(client_addr.sin_port));
        printf("Message payload: %s\n", recv_msg.payload);

        //save user
        if(recv_msg.type == I_NEED_TALK){
         add_user(recv_msg.payload, client_ip, false, true, time(NULL));
         print_user_table();
        }

         //print table
    }

    // Cerrar el socket
    close(socketserver);
    return 0;
}
