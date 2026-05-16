#include "venda.h"
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

static int ultimo_id = 0;

void venda_inicializar(Venda *v, char *cpf) {
    v->id = ++ultimo_id;
    strcpy(v->cpf_cliente, cpf);
    v->data = time(NULL);
    v->num_itens = 0;
    v->total = 0;
    strcpy(v->status, "FINALIZADA");
}

void venda_adicionar_item(Venda *v, Produto *p, int quantidade) {
    if(v->num_itens >= MAX_ITENS) return;
    
    ItemVenda *item = &v->itens[v->num_itens];
    strcpy(item->codigo_produto, p->codigo);
    item->quantidade = quantidade;
    item->preco_unitario = p->preco;
    item->subtotal = quantidade * p->preco;
    
    v->num_itens++;
    venda_calcular_total(v);
}

void venda_calcular_total(Venda *v) {
    v->total = 0;
    for(int i = 0; i < v->num_itens; i++)
        v->total += v->itens[i].subtotal;
}

void venda_exibir(Venda *v) {
    char data_str[30];
    struct tm *tm_info = localtime(&v->data);
    strftime(data_str, 30, "%d/%m/%Y %H:%M:%S", tm_info);
    
    printf("\n╔════════════════════════════════════════════════════════════════╗\n");
    printf("║                         CUPOM FISCAL                           ║\n");
    printf("╠════════════════════════════════════════════════════════════════╣\n");
    printf("║ VENDA #%d\n", v->id);
    printf("║ DATA: %s\n", data_str);
    printf("║ CLIENTE CPF: %s\n", v->cpf_cliente);
    printf("╠════════════════════════════════════════════════════════════════╣\n");
    printf("║ %-10s │ %-6s │ %-10s │ %-10s ║\n", "CODIGO", "QTD", "PRECO", "SUBTOTAL");
    printf("╠════════════════════════════════════════════════════════════════╣\n");
    
    for(int i = 0; i < v->num_itens; i++) {
        printf("║ %-10s │ %-6d │ R$ %-7.2f │ R$ %-7.2f ║\n",
               v->itens[i].codigo_produto,
               v->itens[i].quantidade,
               v->itens[i].preco_unitario,
               v->itens[i].subtotal);
    }
    
    printf("╠════════════════════════════════════════════════════════════════╣\n");
    printf("║ TOTAL: R$ %-50.2f ║\n", v->total);
    printf("╚════════════════════════════════════════════════════════════════╝\n");
}

int venda_para_string(Venda *v, char *buffer, int tamanho) {
    return snprintf(buffer, tamanho, "%d;%s;%ld;%s;%.2f;%d\n",
                    v->id, v->cpf_cliente, (long)v->data, v->status, v->total, v->num_itens);
}

int venda_de_string(Venda *v, char *linha) {
    char *token = strtok(linha, ";");
    if(!token) return 0;
    v->id = atoi(token);
    
    token = strtok(NULL, ";");
    if(!token) return 0;
    strcpy(v->cpf_cliente, token);
    
    token = strtok(NULL, ";");
    if(!token) return 0;
    v->data = atol(token);
    
    token = strtok(NULL, ";");
    if(!token) return 0;
    strcpy(v->status, token);
    
    token = strtok(NULL, ";");
    if(!token) return 0;
    v->total = atof(token);
    
    token = strtok(NULL, "\n");
    if(!token) return 0;
    v->num_itens = atoi(token);
    
    if(v->id > ultimo_id) ultimo_id = v->id;
    return 1;
}

void venda_salvar(Venda *v) {
    FILE *fp = fopen("vendas.txt", "a");
    if(!fp) return;
    
    char buffer[1000];
    venda_para_string(v, buffer, 1000);
    fprintf(fp, "%s", buffer);
    fclose(fp);
}

void vendas_listar() {
    FILE *fp = fopen("vendas.txt", "r");
    if(!fp) {
        printf("Nenhuma venda registrada.\n");
        return;
    }
    
    char linha[1000];
    Venda v;
    
    printf("\n╔════════════════════════════════════════════════════════════════╗\n");
    printf("║                    LISTA DE VENDAS                             ║\n");
    printf("╠════════════════════════════════════════════════════════════════╣\n");
    printf("║ %-6s │ %-15s │ %-10s │ %-10s ║\n", "ID", "CPF CLIENTE", "TOTAL", "STATUS");
    printf("╠════════════════════════════════════════════════════════════════╣\n");
    
    while(fgets(linha, 1000, fp)) {
        if(venda_de_string(&v, linha)) {
            printf("║ %-6d │ %-15s │ R$ %-7.2f │ %-10s ║\n",
                   v.id, v.cpf_cliente, v.total, v.status);
        }
    }
    printf("╚════════════════════════════════════════════════════════════════╝\n");
    fclose(fp);
}

// ============================================
// LISTA VENDAS DE UM CLIENTE ESPECÍFICO
// ============================================
void vendas_por_cliente(char *cpf) {
    FILE *fp = fopen("vendas.txt", "r");
    if(!fp) {
        printf("Nenhuma venda encontrada para este cliente.\n");
        return;
    }
    
    char linha[1000];
    Venda v;
    int count = 0;
    double total_gasto = 0;
    
    printf("\n╔════════════════════════════════════════════════════════════════╗\n");
    printf("║              VENDAS DO CLIENTE CPF: %-29s ║\n", cpf);
    printf("╠════════════════════════════════════════════════════════════════╣\n");
    printf("║ %-6s │ %-20s │ %-10s │ %-10s ║\n", "ID", "DATA", "TOTAL", "STATUS");
    printf("╠════════════════════════════════════════════════════════════════╣\n");
    
    while(fgets(linha, 1000, fp)) {
        if(venda_de_string(&v, linha) && strcmp(v.cpf_cliente, cpf) == 0) {
            char data_str[30];
            struct tm *tm_info = localtime(&v.data);
            strftime(data_str, 30, "%d/%m/%Y %H:%M", tm_info);
            
            printf("║ %-6d │ %-20s │ R$ %-7.2f │ %-10s ║\n",
                   v.id, data_str, v.total, v.status);
            total_gasto += v.total;
            count++;
        }
    }
    
    printf("╠════════════════════════════════════════════════════════════════╣\n");
    printf("║ Total de compras: %-5d │ Total gasto: R$ %-10.2f ║\n", count, total_gasto);
    printf("╚════════════════════════════════════════════════════════════════╝\n");
    
    fclose(fp);
}