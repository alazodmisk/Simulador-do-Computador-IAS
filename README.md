# Simulador do Computador IAS

Este projeto é um simulador da clássica arquitetura do **Computador IAS**, desenvolvido como parte da disciplina de **Arquitetura de Computadores 1 (Arq1)**.

O simulador é capaz de ler um arquivo de texto contendo instruções e dados rudimentares, carregá-los em uma estrutura que simula a memória principal, e executar o ciclo de instrução (Busca, Decodificação e Execução) utilizando registradores específicos da arquitetura (como PC, AC, MQ, MBR, MAR, IR e IBR).

---

## Autores

* **Guilherme Henrique Viana Pichitelli Vitor**
* **Munir Berg Shehadeh**

---

## Como Compilar e Executar

Para compilar o simulador, certifique-se de ter o compilador `gcc` instalado em seu ambiente (Linux/WSL). 

Abra o terminal na pasta do projeto e execute o seguinte comando:

```bash
gcc bancoRegistradores.c ciclo.c inicializador.c memoria.c -o simulador
```

Execute com ./simulador

Exemplo de organização de um arquivo .txt que o simulador processa:
LOAD 102
ADD 103
STOR 104 (Valor do endereço)
DADO 5
DADO 10 (Valor do dado)
HALT 0 (Marca o fim da execução e não é obrigatório)