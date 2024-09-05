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
#include <netinet/ip_icmp.h>

#include "ping-utility.h"
#define PACKET_SIZE 65536  //tamanho dos pacotes de 64bits
#define PING_TIMEOUT 1  // Em segundos


void ping(char * hostIp, char * hostName){
    struct addrinfo hints, *destination;
    struct sockaddr_in addr;
    int sockfd;
    struct icmp icmp_pkt;
    char packet[PACKET_SIZE];
    char recv_buf[PACKET_SIZE];
    int seq_num = 0;
    int received = 0;
    int sent = 0;
    int packets = 10; // Número de pacotes a enviar
    long start_time, end_time, total_time = 0;

    //preenche hints com zero
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_RAW;
    hints.ai_protocol = IPPROTO_ICMP;

    //recupera informacoes (addrinfo) atraves do nome do servido, return 0 caso de erro
    if (getaddrinfo(hostIp, NULL, &hints, &destination) != 0) {
        printf("Erro em recuperar as informacoes sobre o %s", hostIp);
        exit(1);
    }

     addr = *(struct sockaddr_in *)destination->ai_addr;

    // Criar um socket raw
    //Sockets raw oferecem um nível baixo de acesso à rede, permitindo a criação e análise detalhada de pacotes de rede. Eles são úteis para implementações personalizadas de protocolos, análise de rede e ferramentas de diagnóstico, mas exigem permissões especiais e cuidado devido à complexidade e potenciais riscos de segurança.
    //rodar no modo sudo
    sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);

    if (sockfd < 0) {
        printf("Erro em ao criar um socket raw");
        exit(1);
    }

    // Configurando o pacote ICMP
    memset(&icmp_pkt, 0, sizeof(icmp_pkt));
    icmp_pkt.icmp_type = ICMP_ECHO;
    icmp_pkt.icmp_code = 0;
    icmp_pkt.icmp_id = getpid();


    for (int i = 0; i < packets; i++) {
        sent++;
        icmp_pkt.icmp_seq = seq_num++;
        icmp_pkt.icmp_cksum = 0;
        memcpy(packet, &icmp_pkt, sizeof(icmp_pkt));
        icmp_pkt.icmp_cksum = checksum(packet, sizeof(packet));

        start_time = get_time_in_ms();

        if (sendto(sockfd, &icmp_pkt, sizeof(icmp_pkt), 0, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
            perror("sendto");
            continue;
        }

        struct timeval timeout;
        timeout.tv_sec = PING_TIMEOUT;
        timeout.tv_usec = 0;

        setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));

        socklen_t addr_len = sizeof(addr);
        if (recvfrom(sockfd, recv_buf, sizeof(recv_buf), 0, (struct sockaddr *)&addr, &addr_len) > 0) {
            received++;
            end_time = get_time_in_ms();
            long rtt = end_time - start_time;
            total_time += rtt;
            printf("Recebido do %s: bytes=%d time=%ldms\n", hostName, PACKET_SIZE, rtt);
        } else {
            perror("receive");
        }

        sleep(PING_TIMEOUT); // Espera 1 segundo entre os pacotes
    }

    printf("\n--- %s Estatistica ---\n", hostName);
    printf("%d pacotes enviado, %d recebido, %.1f%% pacotes perdidos, tempo decorrido %ldms\n",
           sent, received, ((sent - received) / (float)sent) * 100.0, total_time);

    close(sockfd);
    freeaddrinfo(destination);

}