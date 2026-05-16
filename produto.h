#ifndef PRODUTO_H
#define PRODUTO_H

#define CODIGO_TAM 10
#define PROD_NOME_TAM 100
#define PROD_DESC_TAM 200

typedef struct {
    char codigo[CODIGO_TAM];
    char nome[PROD_NOME_TAM];
    char descricao[PROD_DESC_TAM];
    double preco;
    int estoque;
} Produto;

void produto_preencher(Produto *p);
void produto_exibir(Produto *p);
int produto_para_string(Produto *p, char *buffer, int tamanho);
int produto_de_string(Produto *p, char *linha);
void produto_exibir_resumo(char *chave, long posicao);
int produto_atualizar_estoque(Produto *p, int quantidade);

#endif