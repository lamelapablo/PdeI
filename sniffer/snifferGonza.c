#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h> // read(), write(), close()
#include <arpa/inet.h>


//Cabeceras en wsl
#include<netinet/if_ether.h>
#include<netinet/ip.h>
#include<netinet/tcp.h>
#include<netinet/udp.h>
#include<netinet/ip_icmp.h>

#define MAX 65536
   
void func(int sockfd, struct sockaddr_in source, struct sockaddr_in destination)
{
    unsigned char *buffer = (unsigned char *) malloc(MAX); //to receive data
    memset(buffer,0,MAX);

    int addrLen = sizeof(struct sockaddr_in);
    int bufLen;

    // Loop para el sniffer
    while(1){

        bufLen = recvfrom(sockfd, buffer, MAX, 0 /*FLAGS*/, (struct sockaddr *)&source, &addrLen);
        
        if(bufLen<0){
            printf("Error en recepcion.");
        }else{

        //Desarmo la estructura de cabecera Ethernet
            struct ethhdr *eth = (struct ethhdr *)(buffer);
            unsigned short ethhdrlen = sizeof(struct ethhdr);

            printf("\n[*] Ethernet Header\n");
            printf("\t|-Source Address : %.2X-%.2X-%.2X-%.2X-%.2X-%.2X\n",eth->h_source[0],eth->h_source[1],eth->h_source[2],eth->h_source[3],eth->h_source[4],eth->h_source[5]);
            printf("\t|-Destination Address : %.2X-%.2X-%.2X-%.2X-%.2X-%.2X\n",eth->h_dest[0],eth->h_dest[1],eth->h_dest[2],eth->h_dest[3],eth->h_dest[4],eth->h_dest[5]);
            printf("\t|-Protocol : %d\n",eth->h_proto);  

        //Desarmo la estructura de cabecera IP
            struct iphdr *ip = (struct iphdr*)(buffer + ethhdrlen);
            unsigned short iphdrlen = ip->ihl*4;
                
                //Paso las direcciones de la estructura de la cabecera a mis estructuras de direccion
            memset(&source, 0, sizeof(source));
            memset(&destination, 0, sizeof(destination)); 
            source.sin_addr.s_addr = ip->saddr;
            destination.sin_addr.s_addr = ip->daddr;
            
                //Paso los IP a string para imprimirlos mas facil
            char sourceIP[46];
            char destIP[46];
            inet_ntop(AF_INET, &(source.sin_addr),sourceIP, 46); 
            inet_ntop(AF_INET, &(destination.sin_addr),destIP, 46);

                //Imprimo la cabecera
            printf("\n\nIP Encabezado\n");
            printf("\t|-Version : %d\n",(unsigned int)ip->version);
            printf("\t|-IP longitud encabezado : %d DWORDS or %d Bytes\n",(unsigned int)ip->ihl,((unsigned int)(ip->ihl))*4);
            printf("\t|-Type Of Service : %d\n",(unsigned int)ip->tos);
            printf("\t|-IP longitud total : %d Bytes\n",ntohs(ip->tot_len));
            printf("\t|-Id : %d\n",ntohs(ip->id));
            printf("\t|-TTL : %d\n",(unsigned int)ip->ttl);
            printf("\t|-Protocolo : %d\n",(unsigned int)ip->protocol);
            printf("\t|-Checksum : %d\n",ntohs(ip->check));
            printf("\t|-Origen IP : %s\n", sourceIP);
            printf("\t|-Destino IP : %s\n",destIP);
        
        //Desarmo la estructura de la cabecera de capa de transporte
            int protocolHdrLen;

       //Si es TCP
            if((int)ip->protocol == 6){
                struct tcphdr *tcph=(struct tcphdr*)(buffer + iphdrlen + sizeof(struct ethhdr));
                protocolHdrLen = sizeof(struct tcphdr);
                //Imprimo la cabecera
                printf("\n");
                printf("TCP encabezado\n");
                printf("   |-Origen Puerto     : %u\n",ntohs(tcph->source));
                printf("   |-Destino Puerto: %u\n",ntohs(tcph->dest));
                printf("   |-Numero Secuencia   : %u\n",ntohl(tcph->seq));
                printf("   |-Numero Ack: %u\n",ntohl(tcph->ack_seq));
                printf("   |-Encabezado Longitud      : %d DWORDS or %d BYTES\n" ,(unsigned int)tcph->doff,(unsigned int)tcph->doff*4);
                printf("   |-Urgent Flag          : %d\n",(unsigned int)tcph->urg);
                printf("   |-AckFlag : %d\n",(unsigned int)tcph->ack);
                printf("   |-Push Flag            : %d\n",(unsigned int)tcph->psh);
                printf("   |-Reset Flag           : %d\n",(unsigned int)tcph->rst);
                printf("   |-Synchronise Flag     : %d\n",(unsigned int)tcph->syn);
                printf("   |-Finish Flag          : %d\n",(unsigned int)tcph->fin);
                printf("   |-Window         : %d\n",ntohs(tcph->window));
                printf("   |-Checksum       : %d\n",ntohs(tcph->check));
                printf("   |-Puntero Urgente: %d\n",tcph->urg_ptr);
                printf("\n");    
            }

            //Si es UDP
            if((int)ip->protocol == 17){
                struct udphdr *udpHeader=(struct udphdr*)(buffer + iphdrlen + sizeof(struct ethhdr));
                protocolHdrLen = sizeof(struct udphdr);
                    //Imprimo la cabecera
                printf("\n\n[*] UDP Encabezado\n");
                printf("\t|-origen Puerto : %d\n" , ntohs(udpHeader->source));
                printf("\t|-Destino Puerto : %d\n" , ntohs(udpHeader->dest));
                printf("\t|-UDP Longitud : %d\n" , ntohs(udpHeader->len));
                printf("\t|-UDP Checksum : %d\n" , ntohs(udpHeader->check));
            }

            //Si es ICMP
            if((int)ip->protocol == 1){
                struct icmphdr *icmph = (struct icmphdr *)(buffer + iphdrlen + sizeof(struct ethhdr));
                printf("\n\n ICMP Encabezado\n");
                printf("\t|-Tipo: %d\n",(unsigned int)(icmph->type));
                printf("\t|-Code: %d\n",(unsigned int)(icmph->code));
                printf("\t|-Checksum: %d\n",ntohs(icmph->checksum));
            }

                
        //Extraigo el resto del contenido del paquete
            unsigned char * data = (buffer + iphdrlen + sizeof(struct ethhdr) + protocolHdrLen);
            printf("\n\n[*] Contenido:\n");
            printf("\n\t|-Datos: %s", data);

            printf("------------------------------------------------------------------");
            memset(buffer, 0 ,sizeof(buffer));
            bufLen = 0;
        }
    }
}
   
int main()
{
    //Las estructuras de direccion en las que voy a guardar el trafico que intercepto
    struct sockaddr_in source, destination;
    
    //Creo el socket y verifico su creacion
    int sock = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL)); // Uso RAW para el sniffer
    
    if (sock == -1) {
        printf("[*] Falla la creación del socket...\n");
        perror("Error");
        exit(0);
    }
    else{
        printf("[*] Socket creado...\n");
    }

    //Entro al loop principal
    func(sock, source, destination);

    // Cierro el socket al terminar el chat
    close(sock);
}