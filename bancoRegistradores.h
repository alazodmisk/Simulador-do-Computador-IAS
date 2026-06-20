#ifndef REGISTRADORES_H
#define REGISTRADORES_H

typedef struct 
{
    long long int AC;
    long long int MQ;
    long long int MBR;

    int IBR;
    int IR;
    int MAR;
    int PC;

    int ERRO;
}bancoRegistradores;

void inicializarRegistradores();

extern bancoRegistradores regs;

#endif
