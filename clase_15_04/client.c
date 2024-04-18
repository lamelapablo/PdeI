#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 3535
#define MAX_DATA_SIZE 100

//netstat -ano | findstr :3535
//netsh interface portproxy add v4tov4 listenport=3535 listenaddress=0.0.0.0 connectport=3535 connectaddress= 192.168.160.103
//netsh interface portproxy delete v4tov4 listenport=3535 listenaddress=0.0.0.0
//netsh interface portproxy show all

int main(int argc, char *argv[]){
    int sockfd, numbytes;
    char buff[MAX_DATA_SIZE];
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

    if(connect(sockfd, (struct sockaddr *)&server_addr, sizeof(struct sockaddr)) == -1){
        perror("connect");
        exit(1);
    }

    int n=0;
    printf("Ingrese el numero del cual quiere calcular su factorial: ");
    while ((buff[n++] = getchar()) != '\n');

    send(sockfd, buff, sizeof(buff), 0);

    bzero(buff, sizeof(buff));

    if((numbytes = recv(sockfd, buff, MAX_DATA_SIZE, 0)) == -1){
            perror("recv");
            exit(1);
        }

    buff[numbytes] = '\0';
    printf("El resultado es: %s\n", buff);

    bzero(buff, sizeof(buff));

    if((numbytes = recv(sockfd, buff, MAX_DATA_SIZE, 0)) == -1){
            perror("recv");
            exit(1);
        }

    buff[numbytes] = '\0';

    printf("El timepo transcurrido es %s segundos\n", buff);

    close(sockfd);

    return 0;
}