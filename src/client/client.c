#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>  // Para close()

#include "message_functions.h"
#include "message_types.h"
#include "message.h"
#include "proto.h"

#define LENGHT_NAME 20

//variables globales
int socketclient;
struct sockaddr_in server_addr;

int main() {
    
    //presentarse
    char nickname[50];
    printf("introduce tu nombre de usuario: ");
    scanf("%s",nickname);

    printf("'%s'\n",nickname);
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

    // Preparar el mensaje
    struct message i_am_mgs;
    strncpy(i_am_mgs.to, SERVER_IP, sizeof(i_am_mgs.to) - 1);
    i_am_mgs.to[sizeof(i_am_mgs.to) - 1] = '\0';
    i_am_mgs.type = I_AM;
    strncpy(i_am_mgs.payload,nickname, sizeof(i_am_mgs.payload) - 1);
    i_am_mgs.payload[sizeof(i_am_mgs.payload) - 1] = '\0';

    print_message(i_am_mgs);

    // Enviar el mensaje
    if (sendto(socketclient, &i_am_mgs, sizeof(i_am_mgs), 0, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Fail to send message");
        close(socketclient);
        exit(EXIT_FAILURE);
    }

    printf("Message sent to %s:%d\n", SERVER_IP, SERVER_PORT);

    // Cerrar el socket
    close(socketclient);
    return 0;
}
