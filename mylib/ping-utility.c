#include <sys/time.h>
#include <stdlib.h>

// Função para calcular o checksum
unsigned short checksum(void *b, int len) { 
    //ponteiro para buffer de dados
    unsigned short *buf = b;
    //ponteiro para a soma dos elementos
    unsigned int sum = 0;
    unsigned short result;

    //somador de bytes(2 bytes por vez)
    for (sum = 0; len > 1; len -= 2) {
        //inicia a soma com 0, realiza essa iteração até o len (tamanho) seja maior que 1 enquanto que o passo decrementa 2 (16 bits).
        sum += *buf++;
    }

    //se o comprimento do bytes for impar, significa que sobrou um byte
    if (len == 1) {
        sum += *(unsigned char *)buf;
    }

    // >> : shift direito em c
    // trecho de código que cuida overflow
    //O ChatGPT disse:
    //A soma de carry é importante para garantir que o checksum fique restrito a 16 bits, mesmo quando a soma dos dados excede esse limite (overflow). Ao somar o carry (os bits excedentes) de volta à parte inferior, o algoritmo preserva corretamente a integridade dos dados. Esse processo é crucial em protocolos de rede para detectar erros durante a transmissão, garantindo que todos os bits sejam contabilizados adequadamente.
    sum = (sum >> 16) + (sum & 0xFFFF);
    sum += (sum >> 16);
    //Negação de bits a bits (complemento de 1)
    result = ~sum;
    return result;
}

// Função para obter o tempo atual em milissegundos
long get_time_in_ms() {
    struct timeval time;
    gettimeofday(&time, NULL);
    return time.tv_sec * 1000 + time.tv_usec / 1000;
}