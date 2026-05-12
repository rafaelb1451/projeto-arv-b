#ifndef ARVB_H
#define ARVB_H

#include "cliente.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// CONSTANTES DA ÁRVORE
#define ORDEM 3                      // Ordem da árvore
#define MAX_CHAVES (2*ORDEM - 1)     // Máximo: 5 chaves por nó
#define MIN_CHAVES (ORDEM - 1)       // Mínimo: 2 chaves por nó
#define MAX_FILHOS (2*ORDEM)         // Máximo: 6 filhos por nó

// ESTRUTURA: ItemIndice
// OBJETIVO: Guarda a relação CPF → Posição no arquivo

typedef struct {
    char cpf[CPF_TAM];
    long posicao;
} ItemIndice;


// ESTRUTURA: NoArvB 
// OBJETIVO: Representa um nó da Árvore-B

typedef struct NoArvB {
    int num_chaves;                         // Quantas chaves tem
    ItemIndice chaves[MAX_CHAVES];          // Array de chaves
    struct NoArvB  *filhos[MAX_FILHOS];     // Ponteiros para filhos
    int folha;                              // 1 = folha, 0 = interno
} NoArvB ;

// FUNÇÕES DE CRIAÇÃO E DESTRUIÇÃO

NoArvB *ArvB_criar(int folha);
void ArvB_destruir(NoArvB  *raiz);

// FUNÇÃO DE BUSCA

long ArvB_buscar(NoArvB  *raiz, char *cpf);

// FUNÇÕES DE INSERÇÃO
void ArvB_inserir(NoArvB  **raiz, char *cpf, long posicao);
void ArvB_inserir_nao_cheio(NoArvB  *no, char *cpf, long posicao);
void ArvB_dividir(NoArvB  *pai, int pos_filho);

// FUNÇÕES DE REMOÇÃO
int ArvB_remover(NoArvB  **raiz, char *cpf);
int ArvB_remover_interno(NoArvB  *no, char *cpf);
int ArvB_remover_folha(NoArvB  *no, int pos);
int ArvB_remover_interno_no(NoArvB  *no, int pos);
void ArvB_emprestar_direita(NoArvB  *pai, int pos_filho);
void ArvB_emprestar_esquerda(NoArvB  *pai, int pos_filho);
void ArvB_fundir(NoArvB  *pai, int pos_filho);
void ArvB_rebalancear(NoArvB  *pai, int pos_filho);

// FUNÇÕES DE LISTAGEM ORDENADA
void ArvB_listar_ordenado(NoArvB  *raiz, const char *nome_arquivo);
int ArvB_listar_ordenado_rec(NoArvB  *no, FILE *dados);
int buscar_e_exibir_cliente(FILE *dados, char *cpf, long posicao);

// FUNÇÕES DE PERSISTÊNCIA (Salvar/Carregar)
void ArvB_salvar(NoArvB  *raiz, const char *nome_arquivo);
NoArvB * ArvB_carregar(const char *nome_arquivo);
void ArvB_salvar_no(FILE *fp, NoArvB  *no);
NoArvB * ArvB_carregar_no(FILE *fp);

#endif