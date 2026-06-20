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

    regs.ERRO = 0;
}