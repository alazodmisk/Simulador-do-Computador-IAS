#include "memoria.h"


long long int memoria[TAMANHO_MEMORIA];


void inicializarMemoria(){
    for(int i=0; i<TAMANHO_MEMORIA; i++){
        memoria[i] = 0;
    }
}


long long int lePalavra(int endereco){ //Instrucao vai vir num int ou qualquer coisa maior que int 
    int ender = endereco & BITS_ENDERECO; //0 todos os bits à esquerda dos 12 menos significativos
    return memoria[ender];
}


void escrevePalavra(int endereco, long long int palavra){
    int ender = endereco & BITS_ENDERECO;
    long long int palavraEscrever = palavra & BITS_PALAVRA; //0 todos bits à esquerda dos 40 menos significativos
    memoria[ender] = palavraEscrever;
}
