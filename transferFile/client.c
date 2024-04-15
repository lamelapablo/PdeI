#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <errno.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>

#define PORT 3535

int main(int argc, char *argv[]){

    int sockfd;

    struct hostent *he;
    struct sockaddr_in server_addr;

    if(argc != 2){
        fprintf(stderr, "usage: client hostname\n");
        exit(1);
    }

    if((he = gethostbyname(argv[1])) == NULL){
        perror("gethostbyname");
        exit(1);
    }

    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1){
        perror("socket");
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr = *((struct in_addr *)he->h_addr_list[0]);
    bzero(&(server_addr.sin_zero), 8);

    if(connect(sockfd, (struct sockaddr *)&server_addr, sizeof(struct sockaddr))==-1){
        perror("connect");
        exit(1);
    }

    // Recepción del nombre del archivo
    char filename[256];
    recv(sockfd, filename, sizeof(filename), 0);
    filename[strlen(filename)] = '\0';
    //strcat(filename, "_recibido\0");

    printf("Received filename: %s\n", filename);

    // Verificar si el archivo ya existe
    // if (access(filename, F_OK) != -1) {
    //     printf("El archivo ya existe. Cambiando el nombre.\n");
    //     generate_new_filename(filename);
    // }

    // Creación del archivo local
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        // Manejo del error si no se puede crear el archivo
    } else {
        // Recepción y escritura del contenido del archivo
        char buffer[1024];
        int bytes_received;
        while ((bytes_received = recv(sockfd, buffer, sizeof(buffer), 0)) > 0) {
            fwrite(buffer, 1, bytes_received, file);
        }
        fclose(file);
    }

    return 0;
}