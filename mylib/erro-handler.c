#include <stdlib.h>
#include <stdio.h>

void checkIfHappendAnErroInHostname(int status){
    if(status == -1){
        printf("Nao foi possivel recuperar o nome do host");
        exit(1);
    }
}

void checkIfHappendAnErroInIP(int status ){
    if(status == -1){
        printf("Nao foi possivel recuperar o endereco ipv6");
        exit(1);
    }
}
