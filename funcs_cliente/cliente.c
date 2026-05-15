#include "cliente.h"

// Preenche os dados de um cliente via teclado

void cliente_preencher(Cliente *c) {
    printf("CPF (11 digitos): ");
    fgets(c->cpf, CPF_TAM, stdin);
    c->cpf[strcspn(c->cpf, "\n")] = '\0';
    
    printf("Idade: ");
    scanf("%d", &c->idade);
    getchar();
    
    printf("Nome: ");
    fgets(c->nome, MAX_NOME, stdin);
    c->nome[strcspn(c->nome, "\n")] = '\0';
    
    printf("Email: ");
    fgets(c->email, MAX_EMAIL, stdin);
    c->email[strcspn(c->email, "\n")] = '\0';
}


// Exibe os dados do cliente formatados
void cliente_exibir(Cliente *c) {
    printf("\n=== DADOS DO CLIENTE ===\n");
    printf("CPF: %s\n", c->cpf);
    printf("Idade: %d\n", c->idade);
    printf("Nome: %s\n", c->nome);
    printf("Email: %s\n", c->email);
    printf("========================\n");
}

// Converte Cliente para string (formato: CPF;IDADE;NOME;EMAIL\n)

int cliente_para_string(Cliente *c, char *buffer, int tamanho) {
    return snprintf(buffer, tamanho, "%s;%d;%s;%s\n", c->cpf, c->idade, c->nome, c->email);
}


// Converte string para struct Cliente (formato: CPF;IDADE;NOME;EMAIL\n)

int cliente_de_string(Cliente *c, char *linha) {
    char *token;
    
    token = strtok(linha, ";");
    if(!token) return 0;
    strcpy(c->cpf, token);
    
    token = strtok(NULL, ";");
    if(!token) return 0;
    c->idade = atoi(token);
    
    token = strtok(NULL, ";");
    if(!token) return 0;
    strcpy(c->nome, token);
    
    token = strtok(NULL, "\n");
    if(!token) return 0;
    strcpy(c->email, token);
    
    return 1;
}