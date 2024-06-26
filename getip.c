/*
 ** getip.c -- ejemplo de búsqueda DNS
 */
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
int main(int argc, char *argv[]){
    struct hostent *h;
    if (argc != 2) { // Comprobación de errores en la línea de comandos
        fprintf(stderr,"usage: getip address\n");
        exit(1);
    }
    if ((h=gethostbyname(argv[1])) == NULL) { // Obtener información del host
        herror("gethostbyname");
        exit(1);
    }
    printf("Host name : %s\n", h->h_name);
    printf("IP Address : %s\n", inet_ntoa(*((struct in_addr *)h->h_addr_list[0])));
    return 0;
}