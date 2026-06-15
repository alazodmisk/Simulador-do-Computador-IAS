#include <stdio.h>
#include "ciclo.h"
#include "bancoRegistradores.h"
#include "memoria.h"


int usar_ibr = 0;

void buscaInstrucao(){
    if (usar_ibr){
        regs.MAR = regs.MBR & BITS_ENDERECO;
        regs.IR = (regs.IBR >> 12) & BITS_OPCODE;
        usar_ibr = 0;
        printf("Instrucao colhida o registrador IBR\n");
    }
    else{
        printf("Instrucao lida da memoria\n");
        regs.MAR = regs.PC;
        regs.MBR = lePalavra();
        regs.PC = regs.PC ++;

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
    regs.MBR = lePalavra(regs.MAR);
}


void executaInstrucao(){
    switch (regs.IR)
    {
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
            if (regs.MBR >= 0){
                regs.AC = regs.MBR;
            }
            else{
                regs.AC = -regs.MBR;
            }
            break;
        
        case 0x04: // Instrução: LOAD -|M(X)| | Binário: 00000100
            if (regs.MBR >= 0){
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
            escreveResultado(regs.MBR);
            break;


        // ARITMÉTICA
        case 0x05: // Instrução: ADD M(X) | Binário: 00000101
            buscaOperando();
            regs.AC = regs.AC + regs.MBR;
            break;
        
        case 0x06: // Instrução: SUB M(X) | Binário: 00000110
            buscaOperando();
            regs.AC = regs.AC - regs.MBR; //Não sei se pode fazer isso
            break;
        
        case 0x07: // Instrução: ADD |M(X)| | Binário: 00000111
            if (regs.MBR >= 0){
                regs.AC = regs.AC + regs.MBR;
            }
            else{
                regs.AC = regs.AC - regs.MBR;
            }
            break;
        
        case 0x08: // Instrução: SUB |M(X)| | Binário: 00001000
            if (regs.MBR >= 0){
                regs.AC = regs.AC - regs.MBR;
            }
            else{
                regs.AC = regs.AC + regs.MBR;
            }
            break;
        
        case 0x0B: // Instrução: MUL M(X) | Binário: 00001011
            // regs.MQ * regs.MBR: 40 +significativos -> AC e 40 -significativos MQ
            break;
        
        case 0x0C: // Instrução: DIV M(X) | Binário: 00001100
            // Lógica da divisão: regs.AC / regs.MBR
            regs.MQ = regs.AC / regs.MBR;
            regs.AC = regs.AC % regs.MBR;
            // EH ASSIM QUE FAZ? coloca a dividsao no mq e o resto da div no ac
            break;
        
        case 0x14: // Instrução: LSH | Binário: 00010100
            regs.AC = regs.AC << 1; 
            break;
        
        case 0x15: // Instrução: RSH | Binário: 00010101
            regs.AC = regs.AC >> 1; 
            break;


        // DESVIO (JUMP)       
        case 0x0D: // Instrução: JUMP M(X, 0:19) | Binário: 00001101
            // Lógica do Jump Incondicional Esquerdo
            break;
        
        case 0x0E: // Instrução: JUMP M(X, 20:39) | Binário: 00001110
            // Lógica do Jump Incondicional Direito
            break;
        
        case 0x0F: // Instrução: JUMP+ M(X, 0:19) | Binário: 00001111
            if (regs.AC >= 0) {
                // Lógica do Jump Condicional Esquerdo
            }
            break;
        
        case 0x10: // Instrução: JUMP+ M(X, 20:39) | Binário: 00010000
            if (regs.AC >= 0) {
                // Lógica do Jump Condicional Direito
            }
            break;


        // MODIFICAÇÃO DE ENDEREÇO 
        case 0x12: // Instrução: STOR M(X, 8:19) | Binário: 00010010
            // Substitui o campo de endereço da metade esquerda na memória
            break;
        
        case 0x13: //Instrução: STOR M(X, 28:39) | Binário: 00010011
            // Substitui o campo de endereço da metade direita na memória
            break;

        default:
            printf("Erro: Opcode desconhecido >> Verifique se o arquivo .txt dos comandos esta correto.");
            break;
    }

}


void escreveResultado(){
    escrevePalavra(regs.AC);
}