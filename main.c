#include <stdio.h>
#include "./mylib/information-presentation.h"
#include "./mylib/ping.h"

int main(int argc, char *argv[]) {
    char * hostname;
    int choice = 0;

    welcome();
    hostname = showHostname();
    showIPv4(hostname);
    showIPv6(hostname);
    do{

        printf("\n---------------------------------------------------------------\n");
        printf("Escolha um servido para testar a conexao: \n");
        printf("    1. www.google.com\n");
        printf("    2. www.youtube.com\n");

        scanf("%d", &choice);
    }while(choice != 1 && choice != 2);

    printf("\n---------------------------------------------------------------\n");
    if(choice == 1){
        ping("8.8.8.8", "www.google.com");
    }else{
        ping("142.250.218.206", "www.youtube.com");
    }
    return 0;
}
