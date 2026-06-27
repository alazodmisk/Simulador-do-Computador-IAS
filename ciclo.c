#include <stdio.h>
#include "ciclo.h"
#include "bancoRegistradores.h"
#include "memoria.h"


int usar_ibr = 0;

void buscaInstrucao(){
    if (usar_ibr){
        regs.MAR = regs.IBR & BITS_ENDERECO;
        regs.IR = (regs.IBR >> 12) & BITS_OPCODE;
        usar_ibr = 0;
        printf("Instrucao colhida o registrador IBR\n");
    }
    else{
        printf("Instrucao lida da memoria\n");
        regs.MAR = regs.PC;
        regs.MBR = lePalavra();
        regs.PC++;

        decodificacaoInstrucao(); // Só chama a decodifica instrução se não houver instrução à direita
    }
}


void decodificacaoInstrucao(){
    printf("Instrucao decodificada\n");
    regs.IR = (regs.MBR >> 32) & BITS_OPCODE;
    regs.MAR = (regs.MBR >> 20) & BITS_ENDERECO;
    regs.IBR = regs.MBR & BITS_INSTR_DIREITA;

    usar_ibr = 1;
}


void buscaOperando(){
    regs.MBR = lePalavra();
}


void executaInstrucao(){
    switch (regs.IR)
    {
        case 0x00: // Instrução: HALT | Binário: 00000000
            printf("Instrupbpibihpcao HALT executada. Parando o ciclo.\n");
            return;
        // TRANSFERÊNCIA DE DADOS     
        case 0x01: // Instrução: LOAD M(X) | Binário: 00000001
            buscaOperando();
            regs.AC = regs.MBR;
            break;

        case 0x02: // Instrução: LOAD -M(X) | Binário: 00000010
            buscaOperando();
            regs.AC = -regs.MBR; // Acho que isso não pode ser feito
            break;
        
        case 0x03: // Instrução: LOAD |M(X)| | Binário: 00000011
            buscaOperando();
            if (regs.MBR >= 0 & 0x8000000000ULL){
                regs.AC = regs.MBR;
            }
            else{
                regs.AC = -regs.MBR;
            }
            break;
        
        case 0x04: // Instrução: LOAD -|M(X)| | Binário: 00000100
            buscaOperando();
            if (regs.MBR >= 0 & 0x8000000000ULL){
                regs.AC = -regs.MBR;
            }
            else{
                regs.AC = regs.MBR;
            }
            break;
        
        case 0x09: // Instrução: LOAD MQ, M(X) | Binário: 00001001
            buscaOperando(); 
            regs.MQ = regs.MBR;
            break;
        
        case 0x0A: // Instrução: LOAD MQ | Binário: 00001010
            regs.AC = regs.MQ;
            break;
        
        case 0x21: // Instrução: STOR M(X) | Binário: 00100001
            regs.MBR = regs.AC & BITS_PALAVRA;
            escreveResultado();
            break;


        // ARITMÉTICA
        case 0x05: // Instrução: ADD M(X) | Binário: 00000101
            buscaOperando();
            regs.AC = regs.AC + regs.MBR & BITS_PALAVRA;;
            break;
        
        case 0x06: // Instrução: SUB M(X) | Binário: 00000110
            buscaOperando();
            regs.AC = regs.AC - regs.MBR & BITS_PALAVRA;
            break;
        
        case 0x07: // Instrução: ADD |M(X)| | Binário: 00000111
            buscaOperando();
            if (regs.MBR >= 0 & 0x8000000000ULL){
                regs.AC = regs.AC + regs.MBR & BITS_PALAVRA;
            }
            else{
                regs.AC = regs.AC - regs.MBR & BITS_PALAVRA;
            }
            break;
        
        case 0x08: // Instrução: SUB |M(X)| | Binário: 00001000
            buscaOperando();
            if (regs.MBR >= 0 & 0x8000000000ULL){
                regs.AC = regs.AC - regs.MBR & BITS_PALAVRA;
            }
            else{
                regs.AC = regs.AC + regs.MBR & BITS_PALAVRA;
            }
            break;
        
        case 0x0B: // Instrução: MUL M(X) | Binário: 00001011
            buscaOperando();
                unsigned long long resultado;
                buscaOperando();
                resultado = (unsigned long long)regs.MQ * (unsigned long long)regs.MBR;
                regs.MQ = resultado & BITS_PALAVRA;
                regs.AC = (resultado >> 40) & BITS_PALAVRA;
            break;
        
        case 0x0C: // Instrução: DIV M(X) | Binário: 00001100
            buscaOperando();
            
            if (regs.MBR == 0) {
                fprintf(stderr, "Erro: divisao por zero.\n");
                regs.ERRO = 1;
                return;
            }
            
            regs.MQ = regs.AC / regs.MBR;
            regs.AC = regs.AC % regs.MBR;
            break;
        
        case 0x14: // Instrução: LSH | Binário: 00010100
            regs.AC = regs.AC << 1; 
            break;
        
        case 0x15: // Instrução: RSH | Binário: 00010101
            regs.AC = regs.AC >> 1; 
            break;


        // DESVIO (JUMP)       
        case 0x0D: // Instrução: JUMP M(X, 0:19) | Binário: 00001101
            regs.PC = regs.MAR; // Salta para o endereço da metade esquerda
            usar_ibr = 0; // Garante que a próxima instrução seja buscada da memória, não do IBR
            break;
        
        case 0x0E: // Instrução: JUMP M(X, 20:39) | Binário: 00001110
            regs.PC = regs.MAR; // Salta para o endereço da metade direita
            regs.MBR = lePalavra();
            regs.IBR = regs.MBR & BITS_INSTR_DIREITA;
            regs.PC++;
            usar_ibr = 1;
            break;
        
        case 0x0F: // Instrução: JUMP+ M(X, 0:19) | Binário: 00001111
            if ((regs.AC & 0x8000000000ULL) == 0) {
                regs.PC = regs.MAR; // Salta para o endereço da metade esquerda
                usar_ibr = 0; // Garante que a próxima instrução seja buscada da memória, não do IBR
            }
            break;
        
        case 0x10: // Instrução: JUMP+ M(X, 20:39) | Binário: 00010000
            if ((regs.AC & 0x8000000000ULL) == 0) {
                regs.PC = regs.MAR; // Salta para o endereço da metade direita
                regs.MBR = lePalavra(regs.PC);
                regs.IBR = regs.MBR & BITS_INSTR_DIREITA;
                regs.PC++;
                usar_ibr = 1;
            }
            break;


        // MODIFICAÇÃO DE ENDEREÇO 
        case 0x12: // Instrução: STOR M(X, 8:19) | Binário: 00010010
            buscaOperando(); 
            regs.MBR = regs.MBR & ~((unsigned long long)BITS_ENDERECO << 20);
            long long int novoEndEsq = regs.AC & BITS_ENDERECO;
            regs.MBR = regs.MBR | novoEndEsq;
            escrevePalavra();
            break;
        
        case 0x13: //Instrução: STOR M(X, 28:39) | Binário: 00010011
            buscaOperando(); 
            regs.MBR = regs.MBR & ~((unsigned long long)BITS_ENDERECO);
            long long int novoEndDir = regs.AC & BITS_ENDERECO;
            regs.MBR = regs.MBR | novoEndDir;
            escrevePalavra();
            break;

        default:
            printf("Erro: Opcode desconhecido >> Verifique se o arquivo .txt dos comandos esta correto.");
            break;
    }

}


void escreveResultado(){
    escrevePalavra();
}