#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "memoria.h"
#include "ciclo.h"
#include "bancoRegistradores.h"


void esperarEnter(){
    printf("\n[Pressione ENTER para o proximo passo...]");
    
    char c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}


long long int traduzirOpcode(char *nomeInstrucao) {
    if (strcmp(nomeInstrucao, "LOAD") == 0)         return 1;
    if (strcmp(nomeInstrucao, "LOAD_NEG") == 0)     return 2;
    if (strcmp(nomeInstrucao, "LOAD_ABS") == 0)     return 3;
    if (strcmp(nomeInstrucao, "LOAD_ABS_NEG") == 0) return 4;
    if (strcmp(nomeInstrucao, "LOAD_MQ_M") == 0)    return 9;
    if (strcmp(nomeInstrucao, "LOAD_MQ") == 0)      return 10;
    if (strcmp(nomeInstrucao, "STOR") == 0)         return 33;
    
    if (strcmp(nomeInstrucao, "ADD") == 0)      return 5;
    if (strcmp(nomeInstrucao, "SUB") == 0)      return 6;
    if (strcmp(nomeInstrucao, "ADD_ABS") == 0)  return 7;
    if (strcmp(nomeInstrucao, "SUB_ABS") == 0)  return 8;
    if (strcmp(nomeInstrucao, "MUL") == 0)      return 11;
    if (strcmp(nomeInstrucao, "DIV") == 0)      return 12;
    if (strcmp(nomeInstrucao, "LSH") == 0)      return 20;
    if (strcmp(nomeInstrucao, "RSH") == 0)      return 21;
    
    if (strcmp(nomeInstrucao, "JUMP_ESQ") == 0)      return 13;
    if (strcmp(nomeInstrucao, "JUMP_DIR") == 0)      return 14;
    if (strcmp(nomeInstrucao, "JUMP_COND_ESQ") == 0) return 15;
    if (strcmp(nomeInstrucao, "JUMP_COND_DIR") == 0) return 16;
    
    if (strcmp(nomeInstrucao, "STOR_ESQ") == 0) return 18;
    if (strcmp(nomeInstrucao, "STOR_DIR") == 0) return 19;

    if (strcmp(nomeInstrucao, "HALT") == 0) return 0;
    if (strcmp(nomeInstrucao, "DADO") == 0) return -1;
}


void carregarProgramaDeArquivo(char* nomeArquivo){
    inicializarMemoria();
    inicializarRegistradores();
    
    FILE *arquivo = fopen(nomeArquivo, "r");
        if (arquivo == NULL) {
            printf("Erro: Nao foi possivel abrir o arquivo '%s'.\n", nomeArquivo);
            return;
        }

        int enderecoAtualInstrucoes = INICIO_INSTRUCOES;
        int enderecoAtualDados = 0;
        char instrucao[20];
        int valorEnd;
        
        long long int opcodeEsq = 0, endEsq = 0, opcodeDir = 0, endDir = 0;
        int leuEsquerda = 0; 

        while (fscanf(arquivo, "%s %d", instrucao, &valorEnd) == 2) {

            if (enderecoAtualInstrucoes > TAMANHO_MEMORIA || enderecoAtualDados > INICIO_INSTRUCOES){
                    printf("Memoria RAM insuficiente | Interrupção Imediata\n");
                    fclose(arquivo);
                    return;
                }
            
            long long int opcodeAtual = traduzirOpcode(instrucao);

            if (opcodeAtual == -1) {
                regs.MAR = enderecoAtualDados;
                escrevePalavra(valorEnd);
                enderecoAtualDados++;
            } 
            else {
                if (leuEsquerda == 0) {
                    opcodeEsq = opcodeAtual;
                    endEsq = valorEnd;
                    leuEsquerda = 1;
                } else {
                    opcodeDir = opcodeAtual;
                    endDir = valorEnd;
                    
                    long long int palavraCompleta = (opcodeEsq << 32) | (endEsq << 20) | (opcodeDir << 12) | endDir;
                    regs.MAR = enderecoAtualInstrucoes;
                    escrevePalavra(palavraCompleta);
                    enderecoAtualInstrucoes++;
                    leuEsquerda = 0;
                }
            }
        }

        if (leuEsquerda == 1) {
            long long int palavraIncompleta = (opcodeEsq << 32) | (endEsq << 20);
            regs.MAR = enderecoAtualInstrucoes;
            escrevePalavra(palavraIncompleta);
            enderecoAtualInstrucoes++;
        }

        fclose(arquivo);
        printf("Programa Assembly '%s' carregado com sucesso!\n", nomeArquivo);
}


void iniciarCicloDeMaquina(){
    inicializarRegistradores();

    int rodando = 1;
    getchar(); 

    while (rodando) {
        printf("\n============================================\n");
        printf(" INICIANDO NOVO CICLO (PC atual: %d)\n", regs.PC);
        printf("============================================\n");

        buscaInstrucao();
        printf("-> [1] Busca da Instrucao: \n");
        printf("       Opcode (IR) = %d | Endereco (MAR) = %d\n", regs.IR, regs.MAR);
        printf("       \n", regs.IR, regs.MAR);
        esperarEnter();

        buscaOperandos();
        printf("-> [2] Busca de Operandos: \n");
        printf("       Dado carregado (MBR) = %lld\n", regs.MBR);
        esperarEnter();

        executaInstrucao();
        printf("-> [3] Execucao: \n");
        printf("       Acumulador (AC) = %lld | Multiplicador (MQ) = %lld\n", regs.AC, regs.MQ);
        esperarEnter();

        escreveResultado();
        printf("-> [4] Escrita do Resultado: \n");
        printf("       (Se houver escrita, ela foi enviada para a memoria)\n");
        esperarEnter();
        
        if (regs.IR == 0) {
            printf("\n*** Instrucao de HALT encontrada. Parando o ciclo. ***\n");
            rodando = 0;
        }
    }
}


int main (){
    int opcao;
    char nomeArquivo[100];

    printf("\n===================================\n");
    printf("      SIMULADOR COMPUTADOR IAS     \n");
    printf("===================================\n");


    do {
        printf("1. Carregar programa (.txt)\n");
        printf("2. Rodar o programa completo\n");
        printf("0. Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        switch(opcao) {
            case 1:
                printf("Digite o nome do arquivo (ex: programa.txt): ");
                scanf("%s", nomeArquivo);
                carregarProgramaDeArquivo(nomeArquivo);
                break;

            case 2:
                printf("\nIniciando execucao...\n");
                iniciarCicloDeMaquina();
                printf("Execucao finalizada.\n");
                break;

            case 0:
                printf("Desligando o simulador...\n");
                break;

            default:
                printf("Opcao invalida! Tente novamente.\n");
        }
    } while (opcao != 0);

    return 0;
}