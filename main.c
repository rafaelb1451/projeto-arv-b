#include "arvb.h"
#include "cliente.c"
#include "arvb.c"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define ARQ_DADOS "clientes.txt"
#define ARQ_INDICE "indice.dat"

#ifdef _WIN32
    #define limpar system("cls")
#else
    #define limpar system("clear")
#endif

void limparBuffer(){
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void menu() {
    printf("\n");
    printf("========================================\n");
    printf("     SISTEMA DE CADASTRO DE CLIENTES    \n");
    printf("========================================\n");
    printf("1 - Inserir novo cliente\n");
    printf("2 - Buscar cliente por CPF\n");
    printf("3 - Modificar cliente\n");
    printf("4 - Listar todos os clientes (ordenado)\n");
    printf("5 - Remover cliente\n");
    printf("6 - Sair\n");
    printf("========================================\n");
    printf("Opcao: ");
}

// Insere um cliente no arquivo de dados e retorna a posição onde o cliente foi inserido
long inserir_no_arquivo(Cliente *c) {
    FILE *dados = fopen(ARQ_DADOS, "a");
    if(dados == NULL) {
        printf("Erro ao abrir arquivo de dados!\n");
        return -1;
    }
    
    // Vai para o final do arquivo para saber a posição
    fseek(dados, 0, SEEK_END);
    long posicao = ftell(dados);
    
    // Converte cliente para string e escreve
    char buffer[300];
    cliente_para_string(c, buffer, 300);
    fprintf(dados, "%s", buffer);
    
    fclose(dados);
    return posicao;
}

// Busca um cliente no arquivo pela posição

int buscar_no_arquivo(long posicao, Cliente *c) {
    FILE *dados = fopen(ARQ_DADOS, "r");
    if(dados == NULL) return 0;
    
    fseek(dados, posicao, SEEK_SET);
    
    char linha[300];
    if(fgets(linha, 300, dados) == NULL) {
        fclose(dados);
        return 0;
    }
    
    cliente_de_string(c, linha);
    fclose(dados);
    return 1;
}


// Modifica um cliente na posição específica

int modificar_no_arquivo(long posicao, Cliente *c) {
    FILE *dados = fopen(ARQ_DADOS, "r+");
    if(dados == NULL) return 0;
    
    fseek(dados, posicao, SEEK_SET);
    
    char buffer[300];
    cliente_para_string(c, buffer, 300);
    fprintf(dados, "%s", buffer);
    
    fclose(dados);
    return 1;
}

// FUNÇÃO main

int main() {
    NoArvB *raiz = NULL;
    int opcao;
    
    printf("========================================\n");
    printf("   BEM-VINDO AO SISTEMA DE CADASTRO    \n");
    printf("========================================\n");
    
    // Tenta carregar índice existente do disco
    printf("Carregando indice do disco...\n");
    raiz = ArvB_carregar(ARQ_INDICE);
    
    if(raiz == NULL) {
        printf("Nenhum indice encontrado. Iniciando nova arvore.\n");
        raiz = ArvB_criar(1);  // Cria árvore vazia
    }
    
    do {
        menu();
        scanf("%d", &opcao);
        limparBuffer();
        limpar;
        //getchar(); // Limpa o buffer do teclado

        switch(opcao) {
            // CASO 1: INSERIR CLIENTE
            case 1: {
                Cliente c;
                printf("\n--- NOVO CLIENTE ---\n");
                cliente_preencher(&c);
                
                // Verifica se o CPF já existe
                long pos_existente = ArvB_buscar(raiz, c.cpf);
                if(pos_existente != -1) {
                    printf("ERRO: CPF %s ja cadastrado!\n", c.cpf);
                    break;
                }
                
                // Insere no arquivo de dados
                long posicao = inserir_no_arquivo(&c);
                if(posicao == -1) {
                    printf("ERRO ao inserir no arquivo!\n");
                    break;
                }
                
                // Insere no índice (árvore)
                ArvB_inserir(&raiz, c.cpf, posicao);
                
                // Salva o índice no disco
                ArvB_salvar(raiz, ARQ_INDICE);
                
                printf("\n Cliente inserido com sucesso!\n");
                printf("   Posição no arquivo: %ld bytes\n", posicao);
                break;
            }
            
            // CASO 2: BUSCAR CLIENTE
            case 2: {
                char cpf[CPF_TAM];
                printf("\nCPF para buscar: ");
                fgets(cpf, CPF_TAM, stdin);
                cpf[strcspn(cpf, "\n")] = '\0';
                
                // Busca a posição na árvore
                long posicao = ArvB_buscar(raiz, cpf);
                
                if(posicao == -1) {
                    printf("\n Cliente com CPF %s nao encontrado!\n", cpf);
                    break;
                }
                
                // Busca os dados no arquivo
                Cliente c;
                if(buscar_no_arquivo(posicao, &c)) {
                    cliente_exibir(&c);
                    printf("Posicao no arquivo: %ld bytes\n", posicao);
                } else {
                    printf(" Erro ao ler dados do cliente!\n");
                }
                break;
            }
            
            // CASO 3: MODIFICAR CLIENTE
            case 3: {
                ArvB_listar_ordenado(raiz, ARQ_DADOS);

                char cpf[CPF_TAM];
                printf("\nCPF para modificar: ");
                fgets(cpf, CPF_TAM, stdin);
                cpf[strcspn(cpf, "\n")] = '\0';
                
                // Busca a posição na árvore
                long posicao = ArvB_buscar(raiz, cpf);
                
                if(posicao == -1) {
                    printf("\n Cliente com CPF %s nao encontrado!\n", cpf);
                    break;
                }
                
                // Carrega os dados atuais
                Cliente c;
                if(!buscar_no_arquivo(posicao, &c)) {
                    printf(" Erro ao ler cliente!\n");
                    break;
                }
                
                printf("\n--- MODIFICANDO CLIENTE ---\n");
                printf("Deixe em branco para manter o valor atual\n\n");
                
                char buffer[100];
                
                // Modifica nome
                printf("Nome atual: %s\n", c.nome);
                printf("Novo nome: ");
                fgets(buffer, 100, stdin);
  
                buffer[strcspn(buffer, "\n")] = '\0';
                if(strlen(buffer) > 0) {
                    strcpy(c.nome, buffer);
                }
                
                // Modifica email
                printf("Email atual: %s\n", c.email);
                printf("Novo email: ");
                fgets(buffer, 100, stdin);

                buffer[strcspn(buffer, "\n")] = '\0';
                if(strlen(buffer) > 0) {
                    strcpy(c.email, buffer);
                }
                
                // Modifica idade
                printf("Idade atual: %d\n", c.idade);
                printf("Nova idade: ");
                char idade_str[10];
                fgets(idade_str, 10, stdin);

                if(strlen(idade_str) > 1) {
                    c.idade = atoi(idade_str);
                }
                
                // Salva as modificações no arquivo
                if(modificar_no_arquivo(posicao, &c)) {
                    printf("\n Cliente modificado com sucesso!\n");
                } else {
                    printf(" Erro ao modificar cliente!\n");
                }
                break;
            }
            
            // CASO 4: LISTAR TODOS ORDENADO
            case 4: {
                ArvB_listar_ordenado(raiz, ARQ_DADOS);
                break;
            }
            
            // CASO 5: REMOVER CLIENTE
            case 5: {
                ArvB_listar_ordenado(raiz, ARQ_DADOS);

                char cpf[CPF_TAM];
                printf("\nCPF para remover: ");
                fgets(cpf, CPF_TAM, stdin);
                cpf[strcspn(cpf, "\n")] = '\0';
                
                // Verifica se o cliente existe
                long posicao = ArvB_buscar(raiz, cpf);
                if(posicao == -1) {
                    printf("\n Cliente com CPF %s nao encontrado!\n", cpf);
                    break;
                }
                
                // Remove da árvore
                if(ArvB_remover(&raiz, cpf)) {
                    // Salva o índice atualizado
                    ArvB_salvar(raiz, ARQ_INDICE);
                    printf("\n Cliente removido do indice!\n");
                    printf("   (O registro permanece no arquivo, mas não será encontrado)\n");
                } else {
                    printf("\n Erro ao remover cliente!\n");
                }
                break;
            }
            
            // CASO 6: SAIR
            case 6: {
                printf("\nSalvando indice no disco...\n");
                ArvB_salvar(raiz, ARQ_INDICE);
                break;
            }
            
            default: {
                printf("\n Opçao invalida! Tente novamente.\n");
                break;
            }
        }
        printf("Pressione 'Enter' para prosseguir...");
        limparBuffer();
        limpar;
        
    } while(opcao != 6);
    
    // Libera a memória da árvore
    ArvB_destruir(raiz);
    
    return 0;
}