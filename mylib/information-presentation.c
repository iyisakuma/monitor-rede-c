#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> //biblioteca para usar gethostname
#include <string.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <ifaddrs.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h> 


#include "./erro-handler.h"

void welcome(){
    printf("*** Bem-vindo ao monitor de rede do grupo G4 ;) ***\n");
    printf("** Estas são as informacoes:\n");
}

char * showHostname(){
    char hostname_buffer[256];
    char hostname[256];
    int erro; //-1: Sim e 0: nao
    erro = gethostname(hostname_buffer, sizeof(hostname_buffer));
    checkIfHappendAnErroInHostname(erro);

    printf("-> HOSTNAME: %s \n", hostname_buffer);
    return strcpy(hostname, hostname_buffer);
}


void showIPv4(char *hostname){

    struct ifaddrs *ifaddrs, *ifCurrent; //struct responsavel por armazenar informacoes sobre uma unica interface de rede. Com ela eh possivel descobrir que versao de ip e o endereco
    int ipVersion, erro;
    char host[NI_MAXHOST];

    //A funcao recebe como parametro o ponteiro para um struct ifaddrs, o ponteiro aponta para a regiao de memoria onde esta armazenado uma lista encadeada de interfaces de redes. Saiba mais em https://man7.org/linux/man-pages/man3/getifaddrs.3.html
    erro = getifaddrs(&ifaddrs);

    checkIfHappendAnErroInIP(erro);

    //Itera sobre todas as interfaces de redes disponiveis
    for(ifCurrent = ifaddrs; ifCurrent != NULL;ifCurrent = ifCurrent->ifa_next  ){
        //Caso nao tenha endereco de rede, pula para o proximo registro
        if(ifCurrent->ifa_addr == NULL){
            continue;
        }

        //Pega a versao de ip
        ipVersion = ifCurrent->ifa_addr->sa_family;

        //Se for IPv4
        if(ipVersion == AF_INET){
             // rotina para converter um endereco de rede (socketaddr) para uma string legivel
            getnameinfo(
                ifCurrent->ifa_addr,//ponteiro para a estrutura que sera convertida
                                sizeof(struct sockaddr_in), //tamanho da estrutura passada como primeiro parametro, sockaddr_in6 pq eh um ipv4
                                host,//um buffer que armazena a representacao textual do socketaddr
                                NI_MAXHOST,//tamanho do burrer
                                NULL,//buffer para armazenar a porta e o nome do servido, como nao quero passei null 
                                0,//tamanho do buffer
                                NI_NUMERICHOST);

            // Imprime a interface e o endereço IPv6 correspondente
            printf("-> Interface: %s\tIPv4 Address: %s\n", ifCurrent->ifa_name, host);
        }
    }
}

void showIPv6(char *hostname){
    struct ifaddrs *ifaddrs, *ifCurrent; //struct responsavel por armazenar informacoes sobre uma unica interface de rede. Com ela eh possivel descobrir que versao de ip e o endereco
    int ipVersion, erro;
    char host[NI_MAXHOST];

    erro = getifaddrs(&ifaddrs);

    checkIfHappendAnErroInIP(erro);

    //Itera sobre todas as interfaces de redes disponiveis
    for(ifCurrent = ifaddrs; ifCurrent != NULL;ifCurrent = ifCurrent->ifa_next  ){
        //Caso nao tenha endereco de rede, pula para o proximo registro
        if(ifCurrent->ifa_addr == NULL){
            continue;
        }

        //Pega a versao de ip
        ipVersion = ifCurrent->ifa_addr->sa_family;

        //Se for IPv6
        if(ipVersion == AF_INET6){
            // rotina para converter um endereco de rede (socketaddr) para uma string legivel
            getnameinfo(
                ifCurrent->ifa_addr,//ponteiro para a estrutura que sera convertida
                                sizeof(struct sockaddr_in6), //tamanho da estrutura passada como primeiro parametro, sockaddr_in6 pq eh um ipv6
                                host,//um buffer que armazena a representacao textual do socketaddr
                                NI_MAXHOST,//tamanho do burrer
                                NULL,//buffer para armazenar a porta e o nome do servido, como nao quero passei null 
                                0,//tamanho do buffer
                                NI_NUMERICHOST);

            // Imprime a interface e o endereço IPv6 correspondente
            printf("-> Interface: %s\tIPv6 Address: %s\n", ifCurrent->ifa_name, host);
        }
    }
    //desaloca recursos
    freeifaddrs(ifaddrs);
}