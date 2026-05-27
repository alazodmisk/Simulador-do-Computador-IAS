#ifndef MEMORIA_H
#define MEMORIA_H

#define BITS_ENDERECO 0xFFF
#define BITS_PALAVRA 0xFFFFFFFFFFULL

#define TAMANHO_MEMORIA 1000
#define INICIO_INSTRUCOES 100

// Promessas das funções (apenas o esqueleto)
void inicializarMemoria();
long long int lePalavra(int endereco);
void escrevePalavra(int endereco, long long int palavra);

#endif