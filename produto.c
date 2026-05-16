#include "produto.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void produto_preencher(Produto *p) {
    printf("Codigo do produto: ");
    fgets(p->codigo, CODIGO_TAM, stdin);
    p->codigo[strcspn(p->codigo, "\n")] = '\0';
    
    printf("Nome: ");
    fgets(p->nome, PROD_NOME_TAM, stdin);
    p->nome[strcspn(p->nome, "\n")] = '\0';
    
    printf("Descricao: ");
    fgets(p->descricao, PROD_DESC_TAM, stdin);
    p->descricao[strcspn(p->descricao, "\n")] = '\0';
    
    printf("Preco: R$ ");
    scanf("%lf", &p->preco);
    getchar();
    
    printf("Estoque inicial: ");
    scanf("%d", &p->estoque);
    getchar();
}

void produto_exibir(Produto *p) {
    printf("\n╔════════════════════════════════════════════╗\n");
    printf("║             DADOS DO PRODUTO               ║\n");
    printf("╠════════════════════════════════════════════╣\n");
    printf("║ Codigo:   %-30s ║\n", p->codigo);
    printf("║ Nome:     %-30s ║\n", p->nome);
    printf("║ Descricao:%-30s ║\n", p->descricao);
    printf("║ Preco:    R$ %-27.2f ║\n", p->preco);
    printf("║ Estoque:  %-30d ║\n", p->estoque);
    printf("╚════════════════════════════════════════════╝\n");
}

int produto_para_string(Produto *p, char *buffer, int tamanho) {
    return snprintf(buffer, tamanho, "%s;%s;%s;%.2f;%d\n",
                    p->codigo, p->nome, p->descricao, p->preco, p->estoque);
}

int produto_de_string(Produto *p, char *linha) {
    char *token = strtok(linha, ";");
    if(!token) return 0;
    strcpy(p->codigo, token);
    
    token = strtok(NULL, ";");
    if(!token) return 0;
    strcpy(p->nome, token);
    
    token = strtok(NULL, ";");
    if(!token) return 0;
    strcpy(p->descricao, token);
    
    token = strtok(NULL, ";");
    if(!token) return 0;
    p->preco = atof(token);
    
    token = strtok(NULL, "\n");
    if(!token) return 0;
    p->estoque = atoi(token);
    
    return 1;
}


int produto_atualizar_estoque(Produto *p, int quantidade) {
    if(quantidade > p->estoque) return 0;
    p->estoque -= quantidade;
    return 1;
}

// ============================================
// EXIBE RESUMO DO PRODUTO (PARA LISTAGEM)
// ============================================
void produto_exibir_resumo(char *chave, long posicao) {
    // Carrega o produto para mostrar mais detalhes
    FILE *fp = fopen("produtos.txt", "r");
    if(!fp) {
        printf("  🏷️  Codigo: %s\n", chave);
        return;
    }
    
    fseek(fp, posicao, SEEK_SET);
    char linha[500];
    if(fgets(linha, 500, fp)) {
        Produto p;
        if(produto_de_string(&p, linha)) {
            printf("  🏷️  %-10s | %-30s | Estoque: %d\n", 
                   p.codigo, p.nome, p.estoque);
        } else {
            printf("  🏷️  Codigo: %s\n", chave);
        }
    } else {
        printf("  🏷️  Codigo: %s\n", chave);
    }
    fclose(fp);
}