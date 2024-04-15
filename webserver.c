#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 8080

void handle_request(int client_socket) {
    // Construye la respuesta HTTP
    char response[] = "HTTP/1.1 200 OK\nContent-Type: text/html\n\n<html><head><title>Hello World</title></head><body><h1>Hello, World!</h1></body></html>";

    // Envía la respuesta al cliente
    send(client_socket, response, sizeof(response), 0);
}

int main() {
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);

    // Crea el socket
    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // Configura la dirección del servidor
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Enlaza el socket a la dirección y puerto
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    // Escucha por conexiones entrantes
    if (listen(server_socket, 5) == -1) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    printf("Servidor escuchando en el puerto %d...\n", PORT);

    while (1) {
        // Acepta la conexión entrante
        if ((client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_len)) == -1) {
            perror("accept");
            continue;
        }

        printf("Cliente conectado\n");

        // Maneja la solicitud del cliente
        handle_request(client_socket);

        // Cierra la conexión con el cliente
        close(client_socket);
    }

    // Cierra el socket del servidor
    close(server_socket);

    return 0;
}
