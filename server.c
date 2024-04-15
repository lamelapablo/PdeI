#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
//#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <unistd.h>

#define MY_PORT 3535
#define BACKLOG 10

int main(){

    int sockfd, new_fd;
    struct sockaddr_in my_addr;
    struct sockaddr_in client_addr;
    int sin_size;

    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1){
        perror("socket");
        exit(1);
    }

    my_addr.sin_family = AF_INET;
    my_addr.sin_port = htons(MY_PORT);
    my_addr.sin_addr.s_addr = INADDR_ANY;
    bzero(&(my_addr.sin_zero), 8);

    if(bind(sockfd, (struct sockaddr*)&my_addr, sizeof(struct sockaddr)) == -1){
        perror("bind");
        exit(1);
    }

    if(listen(sockfd, BACKLOG) == -1){
        perror("listen");
        exit(1);
    }

    while(1){
        sin_size = sizeof(struct sockaddr_in);
        if((new_fd = accept(sockfd, (struct sockaddr*)&client_addr, &sin_size))==-1){
            perror("accept");
            continue;
        }
        printf("Server: got connection from |%s|\n", inet_ntoa(client_addr.sin_addr));

        if(!fork()){
            if(send(new_fd, "Hello world\n", 14, 0) == -1) perror("send");
            close(new_fd);
            exit(0);
        }
        close(new_fd);
        while(waitpid(-1, NULL, WNOHANG) > 0);
    }

    return 0;
}