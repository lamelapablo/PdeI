#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>

#define PORT 3535
#define MAX_DATA_SIZE 1024
#define NUM_CONNECTIONS 10

//netstat -ano | findstr :3535
//netsh interface portproxy add v4tov4 listenport=3535 listenaddress=0.0.0.0 connectport=3535 connectaddress= 192.168.160.103
//netsh interface portproxy delete v4tov4 listenport=3535 listenaddress=0.0.0.0
//netsh interface portproxy show all

void recibir_bytes(int sockfd, char *buff);
int generar_numero_random() {
    // Semilla para la generación de números aleatorios
    srand(time(NULL));
    
    // Generar un número aleatorio entre 1 y 25
    return (rand() % 10) + 1;
}

int main(int argc, char *argv[]){
    
    // Crear múltiples conexiones utilizando fork()
    for (int i = 0; i < NUM_CONNECTIONS; ++i) {
        int sockfd;
        char buff[MAX_DATA_SIZE];
        struct sockaddr_in server_addr;

        if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1){
            perror("socket");
            exit(1);
        }

        struct sockaddr_in server_addr;
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(PORT);
        server_addr.sin_addr.s_addr = inet_addr("127.0.0.0");
        bzero(&(server_addr.sin_zero), 8); // pongo en 0 el resto de la estructura

        if(connect(sockfd, (struct sockaddr *)&server_addr, sizeof(struct sockaddr)) == -1){
            perror("connect");
            exit(1);
        }

        pid_t pid = fork();
        
        if (pid == -1) {
            perror("fork");
            exit(EXIT_FAILURE);
        } else if (pid == 0) { // Proceso hijo
            printf("PID %d creando conexión...\n", getpid());

            // Enviar numero random al servidor
            int num = generar_numero_random()+i;
            printf("PID %d | Numero generado: %d\n", getpid(), num);
            sprintf(buff, "%d", num);
            send(sockfd, buff, strlen(buff), 0);
            printf("PID %d | Mensaje enviado al servidor\n", getpid());

            //recibo el calculo del factorial
            bzero(buff, sizeof(buff));
            recibir_bytes(sockfd, buff);
            printf("PID %d | El resultado es: %s\n", getpid(), buff);
            bzero(buff, sizeof(buff));

            //recibo cuanto tiempo le tomo al server hacer el calculo
            recibir_bytes(sockfd, buff);
            printf("PID %d | El tiempo transcurrido es %s milisegundos\n", getpid(), buff);
            bzero(buff, sizeof(buff));

            // Cerrar el socket y terminar el proceso hijo
            close(sockfd);
            exit(EXIT_SUCCESS);
        }
        // Cerrar el socket del padre
        close(sockfd);
    }

    // Esperar a que todos los procesos hijos terminen
    for (int i = 0; i < NUM_CONNECTIONS; ++i) {
        wait(NULL);
    }

    return 0;
}

void recibir_bytes(int sockfd, char *buff){
    int numbytes;
    if((numbytes = recv(sockfd, buff, MAX_DATA_SIZE, 0)) == -1){
        perror("recv");
        exit(1);
    }
    buff[numbytes] = '\0';
}

