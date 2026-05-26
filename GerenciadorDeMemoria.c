#include <stdlib.h>

#define BITS_ENDERECO 0xFFF
#define BITS_PALAVRA 0xFFFFFFFFFFULL

long long int memoria[1000];

long long int lePalavra(int instrucao){ //Instrucao vai vir num int ou qualquer coisa maior que int 
    int endereco = instrucao & BITS_ENDERECO; //0 todos os bits à esquerda dos 12 menos significativos
    return memoria[endereco];
}

void escrevePalavra(int instrucao, long long int palavra){
    int endereco = instrucao & BITS_ENDERECO;
    long long int palavraEscrever = palavra & BITS_PALAVRA; //0 todos bits à esquerda dos 40 menos significativos
    memoria[endereco] = palavraEscrever;
}

/*
O que mais o gerenciador de memória tem que fazer? Pois os endereços quem decide é a CPU
Tem que fazer tratamento de erro aqui? realmente não sei wow.
*/