#ifndef CLIENTE_H
#define CLIENTE_H

#define CPF_TAM 12
#define NOME_TAM 100
#define EMAIL_TAM 100
#define TELEFONE_TAM 15
#define ENDERECO_TAM 200

typedef struct {
    char cpf[CPF_TAM];
    char nome[NOME_TAM];
    char email[EMAIL_TAM];
    char telefone[TELEFONE_TAM];
    char endereco[ENDERECO_TAM];
} Cliente;

void cliente_preencher(Cliente *c);
void cliente_exibir(Cliente *c);
int cliente_para_string(Cliente *c, char *buffer, int tamanho);
int cliente_de_string(Cliente *c, char *linha);
void cliente_exibir_resumo(char *chave, long posicao);

#endif