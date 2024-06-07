#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>  // Para close()

#include "message_types.h"
#include "message.h"
#include "proto.h"

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 8000

int main() {
    int socketclient;
    struct sockaddr_in server_addr;

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
    struct message alive_msg;
    strncpy(alive_msg.to, SERVER_IP, sizeof(alive_msg.to) - 1);
    alive_msg.to[sizeof(alive_msg.to) - 1] = '\0';
    alive_msg.type = I_AM;
    strncpy(alive_msg.payload, "Hello World", sizeof(alive_msg.payload) - 1);
    alive_msg.payload[sizeof(alive_msg.payload) - 1] = '\0';

    // Enviar el mensaje
    if (sendto(socketclient, &alive_msg, sizeof(alive_msg), 0, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Fail to send message");
        close(socketclient);
        exit(EXIT_FAILURE);
    }

    printf("Message sent to %s:%d\n", SERVER_IP, SERVER_PORT);

    // Cerrar el socket
    close(socketclient);
    return 0;
}
