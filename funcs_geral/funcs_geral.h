#ifndef FUNCS_GERAL_H
#define FUNCS_GERAL_H

#include "..\\funcs_arvb\\arvb.h"
#include "..\\funcs_cliente\\cliente.c"
#include "..\\funcs_arvb\\arvb.c"

#define ARQ_DADOS "..\\funcs_cliente\\clientes.txt" // Salvo numa pasta a parte
#define ARQ_INDICE "..\\funcs_cliente\\indice.dat"

#define limpar system("cls")

void limparBuffer();
void menu();
long inserir_no_arquivo(Cliente *c);
int buscar_no_arquivo(long posicao, Cliente *c);
int modificar_no_arquivo(long posicao, Cliente *c);

#endif