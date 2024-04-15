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
#include <time.h>

#define MY_PORT 3535
#define BACKLOG 10
#define MAX_DATA_SIZE 1024

int factorial(int n);
double tiempo_transcurrido(struct timespec *inicio, struct timespec *fin){
    return (fin->tv_sec - inicio->tv_sec) + (fin->tv_nsec - inicio->tv_nsec);
}

void prueba(){
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);
    factorial(120);
    clock_gettime(CLOCK_MONOTONIC, &end);
    double tiempo_t = tiempo_transcurrido(&start, &end);
    printf("Tiempo transcurrido: %f segundos\n", tiempo_t);
}


int main(){

    pid_t pid;
    int sockfd, new_fd, numbytes;
    struct sockaddr_in my_addr;
    struct sockaddr_in client_addr;
    int sin_size;
    char buf[MAX_DATA_SIZE];
    int yes = 1;

    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1){
        perror("socket");
        exit(1);
    }
    if (setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(int)) == -1) {
        perror("setsockopt");
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
    

        if((numbytes = recv(new_fd, buf, MAX_DATA_SIZE, 0)) == -1){
            perror("recv");
            exit(1);
        }

        buf[numbytes] = '\0';

        printf("Received: %s", buf);
        
        struct timespec start, end;
        clock_gettime(CLOCK_MONOTONIC, &start);
        int result = factorial(atoi(buf));
        clock_gettime(CLOCK_MONOTONIC, &end);
        double tiempo_t = tiempo_transcurrido(&start, &end);
        
        bzero(buf, MAX_DATA_SIZE);
        itoa(result, buf);
        send()

        // if(!fork()){
        //     if(send(new_fd, "Hello world\n", 14, 0) == -1) perror("send");
        //     close(new_fd);
        //     exit(0);
        // }
        close(new_fd);
        while(waitpid(-1, NULL, WNOHANG) > 0);
    }

    return 0;
}

int factorial(int n){
    if (n<=1) return 1;
    return n * factorial(n-1);
}
