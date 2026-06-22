#include <stdio.h>
#include "bancoRegistradores.h"
#include "memoria.h"

bancoRegistradores regs;

void inicializarRegistradores(){
    regs.MBR = 0;
    regs.AC = 0;
    regs.MQ = 0;

    regs.IBR = 0;
    regs.MAR = 0;
    regs.IR = 0;
    
    regs.PC = INICIO_INSTRUCOES;

    regs.ERRO = 0; //Não é registrador, apenas uma flag para indicar erro
}


void imprimeRegs(){
    printf("AC: %lld\n", regs.AC);
    printf("MQ: %lld\n", regs.MQ);
    printf("MBR: %lld\n", regs.MBR);
    printf("IBR: %d\n", regs.IBR);
    printf("IR: %d\n", regs.IR);
    printf("MAR: %d\n", regs.MAR);
    printf("PC: %d\n", regs.PC);
}