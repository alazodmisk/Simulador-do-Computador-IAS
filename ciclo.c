#include "ciclo.h"
#include "bancoRegistradores.h"
#include "memoria.h"


#define BITS_INSTRUCAO
//Pegar os bits do opcode

static int usar_ibr = 0;

void buscaInstrucao(){
    if (usar_ibr){
        regs.MAR = regs.MBR & BITS_ENDERECO;
        regs.IR = (regs.IBR >> 12) & BITS_OPCODE;
        usar_ibr = 0;
    }
    else{
        regs.MAR = regs.PC;
        regs.MBR = lePalavra(regs.MAR);
        regs.PC = regs.PC ++;

        decodificacaoInstrucao(); // Só chama a decodifica instrução se não houver instrução à direita
    }
}


void decodificacaoInstrucao(){
    regs.IR = (regs.MBR >> 32) & BITS_OPCODE;
    regs.MAR = (regs.MBR >> 20) & BITS_ENDERECO;
    regs.IBR = regs.MBR & BITS_INSTR_DIREITA;

    usar_ibr = 1;
}


void buscaOperando(){
    regs.MBR = lePalavra(regs.MAR);
}


void executaInstrucao(){
    /*
    Fazer o codigo
    */
}


void escreveResultado(){
    escrevePalavra(regs.AC);
}