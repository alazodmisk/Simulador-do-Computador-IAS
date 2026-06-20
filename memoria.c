#include <stdio.h>
#include "memoria.h"
#include "bancoRegistradores.h"


long long int memoria[TAMANHO_MEMORIA];


void inicializarMemoria(){
    for(int i=0; i<TAMANHO_MEMORIA; i++){
        memoria[i] = 0;
    }
}


long long int lePalavra(){
    printf("Palavra lida do endereco: %d\n", regs.MAR);
    return memoria[regs.MAR];
}


void escrevePalavra(){
    printf("Palavra escrita no endereco: %d\n", regs.MAR);
    long long int palavraEscrever = regs.MBR & BITS_PALAVRA; //0 todos bits à esquerda dos 40 menos significativos
    memoria[regs.MAR] = palavraEscrever;
}
