#include "arvoreB.h"
#include "cliente.h"
#include "produto.h"
#include "venda.h"
#include "arvoreB.c"
#include "cliente.c"
#include "produto.c"
#include "venda.c"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Arquivos de dados
#define ARQ_CLIENTES "clientes.txt"
#define ARQ_PRODUTOS "produtos.txt"
#define ARQ_IDX_CLIENTES "idx_clientes.dat"
#define ARQ_IDX_PRODUTOS "idx_produtos.dat"


void verificar_arquivo_clientes() {
    FILE *fp = fopen("clientes.txt", "r");
    if(!fp) {
        printf("Arquivo clientes.txt nao encontrado!\n");
        return;
    }
    
    printf("\n=== CONTEUDO DO ARQUIVO clientes.txt ===\n");
    char linha[500];
    long pos = 0;
    int count = 0;
    
    while(fgets(linha, 500, fp)) {
        printf("Posicao %ld: %s", pos, linha);
        pos = ftell(fp);
        count++;
    }
    
    printf("Total de registros: %d\n", count);
    printf("=========================================\n\n");
    fclose(fp);
}

long salvar_cliente(Cliente *c) {
    FILE *fp = fopen("clientes.txt", "a");
    if(!fp) {
        printf(" Erro ao abrir arquivo clientes.txt\n");
        return -1;
    }
    
    // Pega a posição ANTES de escrever
    fseek(fp, 0, SEEK_END);
    long pos = ftell(fp);
    
    printf("   [DEBUG] Posicao antes de escrever: %ld\n", pos); // Debug
    
    char buffer[500];
    cliente_para_string(c, buffer, 500);
    fprintf(fp, "%s", buffer);
    
    fflush(fp); // Força a escrita no disco
    fclose(fp);
    
    printf("   [DEBUG] Cliente salvo: %s na posicao %ld\n", c->cpf, pos);
    
    return pos;
}

int carregar_cliente(long pos, Cliente *c) {
    FILE *fp = fopen("clientes.txt", "r");
    if(!fp) {
        printf(" Erro ao abrir clientes.txt para leitura\n");
        return 0;
    }
    
    fseek(fp, pos, SEEK_SET);
    char linha[500];
    if(!fgets(linha, 500, fp)) {
        printf("   Debug: Falha ao ler linha na posicao %ld\n", pos);
        fclose(fp);
        return 0;
    }
    
    printf("   Debug: Linha lida: %s\n", linha);
    
    int resultado = cliente_de_string(c, linha);
    fclose(fp);
    return resultado;
}

int atualizar_cliente(long pos, Cliente *c) {
    FILE *fp = fopen("clientes.txt", "r+");
    if(!fp) return 0;
    
    fseek(fp, pos, SEEK_SET);
    char buffer[500];
    cliente_para_string(c, buffer, 500);
    fprintf(fp, "%s", buffer);
    fclose(fp);
    return 1;
}

// funções de produto
long salvar_produto(Produto *p) {
    FILE *fp = fopen(ARQ_PRODUTOS, "a");
    if(!fp) return -1;
    
    fseek(fp, 0, SEEK_END);
    long pos = ftell(fp);
    
    char buffer[500];
    produto_para_string(p, buffer, 500);
    fprintf(fp, "%s", buffer);
    fclose(fp);
    return pos;
}

int carregar_produto(long pos, Produto *p) {
    FILE *fp = fopen(ARQ_PRODUTOS, "r");
    if(!fp) return 0;
    
    fseek(fp, pos, SEEK_SET);
    char linha[500];
    if(!fgets(linha, 500, fp)) {
        fclose(fp);
        return 0;
    }
    produto_de_string(p, linha);
    fclose(fp);
    return 1;
}

int atualizar_produto(long pos, Produto *p) {
    FILE *fp = fopen(ARQ_PRODUTOS, "r+");
    if(!fp) return 0;
    
    fseek(fp, pos, SEEK_SET);
    char buffer[500];
    produto_para_string(p, buffer, 500);
    fprintf(fp, "%s", buffer);
    fclose(fp);
    return 1;
}

// venda
void finalizar_venda(Venda *venda, NoArvB *raiz_produtos) {
    // Atualizar estoque dos produtos
    for(int i = 0; i < venda->num_itens; i++) {
        long pos = ArvB_buscar(raiz_produtos, venda->itens[i].codigo_produto);
        if(pos != -1) {
            Produto p;
            carregar_produto(pos, &p);
            p.estoque -= venda->itens[i].quantidade;
            atualizar_produto(pos, &p);
        }
    }
    
    venda_salvar(venda);
}

//menu
void menu_principal() {
    printf("\n");
    printf("-------------------------------------------------------------\n");
    printf("                   SUPERMERCADO 3 IRMAO \n");
    printf("-------------------------------------------------------------\n");
                    printf("    --- CLIENTES ---\n");
                    printf("1 - Cadastrar cliente\n");
                    printf("2 - Buscar cliente   \n");
                    printf("3 - Listar clientes  \n");
                    printf("4 - Remover cliente  \n");
                    printf("---------------------\n");
                    printf("   --- PRODUTOS ---\n");
                    printf("5 - Cadastrar produto\n");
                    printf("6 - Buscar produto   \n");
                    printf("7 - Listar produtos  \n");
                    printf("8 - Remover produto  \n");
                    printf("---------------------\n");
                    printf("    --- VENDAS --- \n");
                    printf("9 - Nova venda\n");
                    printf("10 - Listar vendas\n");
                    printf("11 - Vendas por cliente\n");
                    printf("---------------------\n");
                    printf("0 - Sair             \n");
                    printf("---------------------\n");
                    printf("Opcao: ");
}

// ========== FUNÇÃO PRINCIPAL ==========
int main() {
    NoArvB *raiz_clientes = NULL;
    NoArvB *raiz_produtos = NULL;
    int opcao;
    
    printf("\n---------------------------------------------------------------\n");
    printf("              BEM-VINDO AO SUPERMERCADO 3 IRMAO\n");
    printf("\n---------------------------------------------------------------\n");
    
    // Carregar dados
    raiz_clientes = ArvB_carregar(ARQ_IDX_CLIENTES);
    if(!raiz_clientes) raiz_clientes = ArvB_criar(1);
    
    raiz_produtos = ArvB_carregar(ARQ_IDX_PRODUTOS);
    if(!raiz_produtos) raiz_produtos = ArvB_criar(1);
    
    do {
        menu_principal();
        scanf("%d", &opcao);
        getchar();
        
        switch(opcao) {
            // ========== CLIENTES ==========
            case 1: { // Cadastrar cliente
                Cliente c;
                printf("\n--- CADASTRO DE CLIENTE ---\n");
                cliente_preencher(&c);
                
                if(ArvB_buscar(raiz_clientes, c.cpf) != -1) {
                    printf(" CPF ja cadastrado!\n");
                    break;
                }
                
                long pos = salvar_cliente(&c);
                ArvB_inserir(&raiz_clientes, c.cpf, pos);
                ArvB_salvar(raiz_clientes, ARQ_IDX_CLIENTES);
                printf("\n Cliente cadastrado com sucesso!\n");
                verificar_arquivo_clientes();
                break;
            }
            
            case 2: { // Buscar cliente
                char cpf[CPF_TAM];
                printf("\nCPF: ");
                fgets(cpf, CPF_TAM, stdin);
                cpf[strcspn(cpf, "\n")] = '\0';
                
                long pos = ArvB_buscar(raiz_clientes, cpf);
                if(pos == -1) {
                    printf(" Cliente nao encontrado!\n");
                    break;
                }
                
                Cliente c;
                carregar_cliente(pos, &c);
                cliente_exibir(&c);
                break;
            }
            
            case 3: { // Listar clientes
                printf("\n ----------------------------------------------------------------\n");
                printf("|                    LISTA DE CLIENTES                           |\n");
                printf("------------------------------------------------------------------\n");
                printf("| CPF          |NOME                           |EMAIL            |\n");
                printf("|----------------------------------------------------------------|\n");
                ArvB_listar(raiz_clientes, cliente_exibir_resumo);
                printf("|----------------------------------------------------------------|\n");
                break;
            }
            
            case 4: { // Remover cliente
                char cpf[CPF_TAM];
                printf("\nCPF: ");
                fgets(cpf, CPF_TAM, stdin);
                cpf[strcspn(cpf, "\n")] = '\0';
                
                if(ArvB_remover(&raiz_clientes, cpf)) {
                    ArvB_salvar(raiz_clientes, ARQ_IDX_CLIENTES);
                    printf(" Cliente removido!\n");
                } else {
                    printf(" Cliente nao encontrado!\n");
                }
                break;
            }
            
            // ========== PRODUTOS ==========
            case 5: { // Cadastrar produto
                Produto p;
                printf("\n--- CADASTRO DE PRODUTO ---\n");
                produto_preencher(&p);
                
                if(ArvB_buscar(raiz_produtos, p.codigo) != -1) {
                    printf(" Codigo ja cadastrado!\n");
                    break;
                }
                
                long pos = salvar_produto(&p);
                ArvB_inserir(&raiz_produtos, p.codigo, pos);
                ArvB_salvar(raiz_produtos, ARQ_IDX_PRODUTOS);
                printf("\n Produto cadastrado com sucesso!\n");
                break;
            }
            
            case 6: { // Buscar produto
                char codigo[CODIGO_TAM];
                printf("\nCodigo: ");
                fgets(codigo, CODIGO_TAM, stdin);
                codigo[strcspn(codigo, "\n")] = '\0';
                
                long pos = ArvB_buscar(raiz_produtos, codigo);
                if(pos == -1) {
                    printf(" Produto nao encontrado!\n");
                    break;
                }
                
                Produto p;
                carregar_produto(pos, &p);
                produto_exibir(&p);
                break;
            }
            
           case 7: { // Listar produtos
            printf("\n   ----------------------------------------------------------------\n");
                printf("|                    LISTA DE PRODUTOS                           |\n");
                printf("| Codigo     │ Nome                           │ Estoque          |\n");
                printf("|----------------------------------------------------------------|\n");
                ArvB_listar(raiz_produtos, produto_exibir_resumo);
                printf("|----------------------------------------------------------------|\n");
                break;
            }
            
            case 8: { // Remover produto
                char codigo[CODIGO_TAM];
                printf("\nCodigo: ");
                fgets(codigo, CODIGO_TAM, stdin);
                codigo[strcspn(codigo, "\n")] = '\0';
                
                if(ArvB_remover(&raiz_produtos, codigo)) {
                    ArvB_salvar(raiz_produtos, ARQ_IDX_PRODUTOS);
                    printf(" Produto removido!\n");
                } else {
                    printf(" Produto nao encontrado!\n");
                }
                break;
            }
            
            // ========== VENDAS ==========
            case 9: { // Nova venda
                char cpf[CPF_TAM];
                printf("\n--- NOVA VENDA ---\n");
                printf("CPF do cliente: ");
                fgets(cpf, CPF_TAM, stdin);
                cpf[strcspn(cpf, "\n")] = '\0';
                
                if(ArvB_buscar(raiz_clientes, cpf) == -1) {
                    printf(" Cliente nao encontrado!\n");
                    break;
                }
                
                Venda venda;
                venda_inicializar(&venda, cpf);
                
                char continuar = 's';
                while(continuar == 's' || continuar == 'S') {
                    char codigo[CODIGO_TAM];
                    int quantidade;
                    
                    printf("\nCodigo do produto: ");
                    fgets(codigo, CODIGO_TAM, stdin);
                    codigo[strcspn(codigo, "\n")] = '\0';
                    
                    long pos = ArvB_buscar(raiz_produtos, codigo);
                    if(pos == -1) {
                        printf(" Produto nao encontrado!\n");
                    } else {
                        Produto p;
                        carregar_produto(pos, &p);
                        
                        printf("Produto: %s\n", p.nome);
                        printf("Preco: R$ %.2f\n", p.preco);
                        printf("Estoque: %d\n", p.estoque);
                        printf("Quantidade: ");
                        scanf("%d", &quantidade);
                        getchar();
                        
                        if(quantidade > p.estoque) {
                            printf(" Estoque insuficiente!\n");
                        } else {
                            venda_adicionar_item(&venda, &p, quantidade);
                            printf(" Item adicionado! Subtotal: R$ %.2f\n", quantidade * p.preco);
                        }
                    }
                    
                    printf("\nAdicionar mais itens? (s/n): ");
                    scanf("%c", &continuar);
                    getchar();
                }
                
                if(venda.num_itens > 0) {
                    venda_exibir(&venda);
                    finalizar_venda(&venda, raiz_produtos);
                    ArvB_salvar(raiz_produtos, ARQ_IDX_PRODUTOS);
                    printf("\n Venda finalizada com sucesso!\n");
                } else {
                    printf(" Venda cancelada - nenhum item adicionado.\n");
                }
                break;
            }
            
            case 10: // Listar vendas
                vendas_listar();
                break;
            
            case 11: { // Vendas por cliente
                char cpf[CPF_TAM];
                printf("\nCPF do cliente: ");
                fgets(cpf, CPF_TAM, stdin);
                cpf[strcspn(cpf, "\n")] = '\0';
                vendas_por_cliente(cpf);
                break;
            }
            
            case 0: // Sair
                printf("\nSalvando dados...\n");
                ArvB_salvar(raiz_clientes, ARQ_IDX_CLIENTES);
                ArvB_salvar(raiz_produtos, ARQ_IDX_PRODUTOS);
                printf(" Dados salvos!\n");
                printf("Programa encerrado. Volte sempre!\n");
                break;
            
            default:
                printf(" Opcao invalida!\n");
        }
    } while(opcao != 0);
    
    ArvB_destruir(raiz_clientes);
    ArvB_destruir(raiz_produtos);
    
    return 0;
}