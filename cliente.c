#include "cliente.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void limpar_buffer() {
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF);
}

void cliente_preencher(Cliente *c) {
    int valido;
    // ========== CPF (com validação) ==========
    do {
        valido = 1;
        printf("CPF (11 digitos): ");
        fgets(c->cpf, CPF_TAM, stdin);
        c->cpf[strcspn(c->cpf, "\n")] = '\0';
        
        limpar_buffer();
        
        // Verifica se tem 11 digitos
        if(strlen(c->cpf) != 11) {
            printf("  ❌ ERRO: CPF deve ter exatamente 11 digitos!\n");
            printf("  Digite novamente.\n\n");
            valido = 0;
        }
        
        // Verifica se só tem números
        for(int i = 0; i < strlen(c->cpf) && valido; i++) {
            if(c->cpf[i] < '0' || c->cpf[i] > '9') {
                printf("  ❌ ERRO: CPF deve conter apenas numeros!\n");
                printf("  Digite novamente.\n\n");
                valido = 0;
            }
        }
    } while(!valido);
    
    // ========== NOME ==========
    printf("Nome: ");
    fgets(c->nome, NOME_TAM, stdin);
    c->nome[strcspn(c->nome, "\n")] = '\0';
    
    // Verifica se o nome não está vazio
    if(strlen(c->nome) == 0) {
        strcpy(c->nome, "Nao informado");
    }
    
    // ========== EMAIL ==========
    printf("Email: ");
    fgets(c->email, EMAIL_TAM, stdin);
    c->email[strcspn(c->email, "\n")] = '\0';
    
    if(strlen(c->email) == 0) {
        strcpy(c->email, "Nao informado");
    }
    
    // ========== TELEFONE ==========
    printf("Telefone: ");
    fgets(c->telefone, TELEFONE_TAM, stdin);
    c->telefone[strcspn(c->telefone, "\n")] = '\0';
    
    if(strlen(c->telefone) == 0) {
        strcpy(c->telefone, "Nao informado");
    }
    
    // ========== ENDERECO ==========
    printf("Endereco: ");
    fgets(c->endereco, ENDERECO_TAM, stdin);
    c->endereco[strcspn(c->endereco, "\n")] = '\0';
    
    if(strlen(c->endereco) == 0) {
        strcpy(c->endereco, "Nao informado");
    }
}

// ============================================
// EXIBE OS DADOS DO CLIENTE
// ============================================
void cliente_exibir(Cliente *c) {
    printf("\n╔════════════════════════════════════════════════════════════════╗\n");
    printf("  ║                      DADOS DO CLIENTE                          ║\n");
    printf("  ╠════════════════════════════════════════════════════════════════╣\n");
    printf("  ║ CPF:      %-45s ║\n", c->cpf);
    printf("  ║ Nome:     %-45s ║\n", c->nome);
    printf("  ║ Email:    %-45s ║\n", c->email);
    printf("  ║ Telefone: %-45s ║\n", c->telefone);
    printf("  ║ Endereco: %-45s ║\n", c->endereco);
    printf("  ╚════════════════════════════════════════════════════════════════╝\n");
}

// ============================================
// CONVERTE CLIENTE PARA STRING (SALVAR NO ARQUIVO)
// ============================================
int cliente_para_string(Cliente *c, char *buffer, int tamanho) {
    // Ordem: CPF;NOME;EMAIL;TELEFONE;ENDERECO
    return snprintf(buffer, tamanho, "%s;%s;%s;%s;%s\n",
                    c->cpf, 
                    c->nome, 
                    c->email, 
                    c->telefone, 
                    c->endereco);
}

// ============================================
// CONVERTE STRING PARA CLIENTE (CARREGAR DO ARQUIVO)
// ============================================
int cliente_de_string(Cliente *c, char *linha) {
    char *token;
    
    // Remove quebra de linha
    linha[strcspn(linha, "\n")] = '\0';
    
    // Ordem: CPF;NOME;EMAIL;TELEFONE;ENDERECO
    token = strtok(linha, ";");
    if(!token) return 0;
    strcpy(c->cpf, token);
    
    token = strtok(NULL, ";");
    if(!token) return 0;
    strcpy(c->nome, token);
    
    token = strtok(NULL, ";");
    if(!token) return 0;
    strcpy(c->email, token);
    
    token = strtok(NULL, ";");
    if(!token) return 0;
    strcpy(c->telefone, token);
    
    token = strtok(NULL, ";");
    if(!token) return 0;
    strcpy(c->endereco, token);
    
    return 1;
}

// ============================================
// EXIBE RESUMO DO CLIENTE (PARA LISTAGEM)
// ============================================
void cliente_exibir_resumo(char *chave, long posicao) {
    FILE *fp = fopen("clientes.txt", "r");
    if(!fp) {
        printf("   CPF: %s (arquivo nao encontrado)\n", chave);
        return;
    }
    
    Cliente c;
    int encontrado = 0;
    char linha[500];
    
    // Tenta primeiro pela posição (mais rápido)
    if(posicao >= 0) {
        fseek(fp, posicao, SEEK_SET);
        if(fgets(linha, 500, fp)) {
            if(cliente_de_string(&c, linha) && strcmp(c.cpf, chave) == 0) {
                printf("   %-12s | %-30s | %-20s\n", c.cpf, c.nome, c.email);
                encontrado = 1;
            }
        }
    }
    
    // Se não encontrou pela posição, faz busca sequencial (fallback)
    if(!encontrado) {
        rewind(fp);  // Volta ao início do arquivo
        while(fgets(linha, 500, fp)) {
            if(cliente_de_string(&c, linha) && strcmp(c.cpf, chave) == 0) {
                printf("   %-12s | %-30s | %-20s\n", c.cpf, c.nome, c.email);
                encontrado = 1;
                break;
            }
        }
    }
    
    if(!encontrado) {
        printf("    CPF: %s (nao encontrado no arquivo)\n", chave);
    }
    
    fclose(fp);
}