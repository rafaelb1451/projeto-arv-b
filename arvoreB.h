#ifndef ARVOREB_H
#define ARVOREB_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Configuração da Árvore-B (ordem 3)
#define ORDEM 3
#define MAX_CHAVES (2*ORDEM - 1)  // 5
#define MIN_CHAVES (ORDEM - 1)     // 2
#define MAX_FILHOS (2*ORDEM)       // 6

// Estrutura de índice (chave -> posição)
typedef struct {
    char chave[20];      // CPF para cliente, Código para produto
    long posicao;        // Posição no arquivo
} ItemIndice;

// Estrutura do nó da Árvore-B
typedef struct NoArvB {
    int num_chaves;
    ItemIndice chaves[MAX_CHAVES];
    struct NoArvB *filhos[MAX_FILHOS];
    int folha;
} NoArvB;

// ========== FUNÇÕES BÁSICAS ==========
NoArvB* ArvB_criar(int folha);
void ArvB_destruir(NoArvB *raiz);

// ========== BUSCA ==========
long ArvB_buscar(NoArvB *raiz, char *chave);

// ========== INSERÇÃO ==========
void ArvB_inserir(NoArvB **raiz, char *chave, long posicao);
void ArvB_inserir_nao_cheio(NoArvB *no, char *chave, long posicao);
void ArvB_dividir(NoArvB *pai, int pos_filho);

// ========== REMOÇÃO ==========
int ArvB_remover(NoArvB **raiz, char *chave);
int ArvB_remover_interno(NoArvB *no, char *chave);
int ArvB_remover_folha(NoArvB *no, int pos);
int ArvB_remover_interno_no(NoArvB *no, int pos);
void ArvB_emprestar_direita(NoArvB *pai, int pos_filho);
void ArvB_emprestar_esquerda(NoArvB *pai, int pos_filho);
void ArvB_fundir(NoArvB *pai, int pos_filho);
void ArvB_rebalancear(NoArvB *pai, int pos_filho);

// ========== LISTAGEM ORDENADA ==========
void ArvB_listar(NoArvB *raiz, void (*exibir)(char*, long));
void ArvB_listar_rec(NoArvB *no, void (*exibir)(char*, long));

// ========== PERSISTÊNCIA ==========
void ArvB_salvar(NoArvB *raiz, const char *nome_arquivo);
NoArvB* ArvB_carregar(const char *nome_arquivo);
void ArvB_salvar_rec(FILE *fp, NoArvB *no);
NoArvB* ArvB_carregar_rec(FILE *fp);

#endif