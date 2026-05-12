#ifndef CLIENTE_H
#define CLIENTE_H

#define MAX_NOME 100
#define MAX_EMAIL 100
#define CPF_TAM 12

typedef struct {
    char cpf[CPF_TAM];
    int idade;
    char nome[MAX_NOME];
    char email[MAX_EMAIL];
} Cliente;

// Protótipos das funções
void cliente_preencher(Cliente *c);
void cliente_exibir(Cliente *c);
int cliente_para_string(Cliente *c, char *buffer, int tamanho);
int cliente_de_string(Cliente *c, char *linha);

#endif