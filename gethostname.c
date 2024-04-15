#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <arpa/inet.h>

int main(int argc, char *argv[]){
    struct hostent *h;
    struct in_addr addr;

    if (argc != 2) { // Comprobación de errores en la línea de comandos
        fprintf(stderr,"usage: getip address\n");
        exit(1);
    }

    inet_aton(argv[1], &addr);

    if ((h=gethostbyaddr(&addr, sizeof(addr), AF_INET)) == NULL) { // Obtener información del host
        herror("gethostbyname");
        exit(1);
    }

    printf("Host name: %s\n", h->h_name);
    printf("IP address: %s\n", argv[1]);

    printf("Aliases:\n");
    char **alias;
    for (alias = h->h_aliases; *alias != NULL; alias++) {
        printf("- %s\n", *alias);
    }

    return 0;
}