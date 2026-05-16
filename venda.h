#ifndef VENDA_H
#define VENDA_H

#include "produto.h"
#include "cliente.h"
#include <time.h>

#define MAX_ITENS 20

typedef struct {
    char codigo_produto[CODIGO_TAM];
    int quantidade;
    double preco_unitario;
    double subtotal;
} ItemVenda;

typedef struct {
    int id;
    char cpf_cliente[CPF_TAM];
    time_t data;
    ItemVenda itens[MAX_ITENS];
    int num_itens;
    double total;
    char status[15];
} Venda;

void venda_inicializar(Venda *v, char *cpf);
void venda_adicionar_item(Venda *v, Produto *p, int quantidade);
void venda_calcular_total(Venda *v);
void venda_exibir(Venda *v);
int venda_para_string(Venda *v, char *buffer, int tamanho);
int venda_de_string(Venda *v, char *linha);
void venda_salvar(Venda *v);
void vendas_listar();
void vendas_por_cliente(char *cpf);

#endif