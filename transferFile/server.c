#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 3535
#define BACKLOG 10

void transfer_file(int newfd, char filename[]);
void child_process(int newfd);

int main(){

    int sockfd, newfd, sin_size;
    struct sockaddr_in my_addr, client_addr;

    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1){
        perror("socket");
        exit(1);
    }

    my_addr.sin_family = AF_INET;
    my_addr.sin_port = htons(PORT);
    my_addr.sin_addr.s_addr = INADDR_ANY;
    memset(&(my_addr.sin_zero), '\0', 8);

    if((bind(sockfd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr))) == -1){
        perror("bind");
        exit(1);
    }

    if(listen(sockfd, BACKLOG) == -1){
        perror("listen");
        exit(1);
    }

    while(1){
        sin_size = sizeof(struct sockaddr_in);
        if((newfd = accept(sockfd, (struct sockaddr *)&client_addr, &sin_size))==-1){
            perror("accept");
            continue;
        }

        printf("Server got connection from: %s\n", inet_ntoa(client_addr.sin_addr));

        if(!fork()){
            close(sockfd);
            child_process(newfd);
            exit(0);
        }

        close(newfd);
    }

    return 0;
}

void child_process(int newfd){
    char filename[] = "hola.txt";
    transfer_file(newfd, filename);
    close(newfd);   
}

void transfer_file(int newfd, char filename[]){
    char filename_sent[] = "enviado.txt";
    if (send(newfd, filename_sent, strlen(filename_sent), 0) == -1){
        perror("send");
        exit(1);
    }

    FILE *fptr;

    // Open a file in read mode
    fptr = fopen(filename, "r");

    // If the file exist
    if(fptr != NULL) {

        char buffer[1024];
        int bytes_read;
        while ((bytes_read = fread(buffer, 1, sizeof(buffer), fptr)) > 0) {
            if(send(newfd, buffer, bytes_read, 0) == -1){
                perror("send");
                continue;
            }
        }
        fclose(fptr);

    // If the file does not exist
    } else {
        printf("Not able to open the file.\n");
    }

}