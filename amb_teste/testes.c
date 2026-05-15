#include "..\\funcs_arvb\\arvb.h"
#include "..\\funcs_geral\\funcs_geral.h"
#include <stdbool.h>

// Função para exibir de forma mais visual (em formato de árvore)
void exibirArvoreBVisual(NoArvB* no, int nivel, char* prefixo, int ehUltimo) {
    if (no == NULL) return;
    
    // Exibe o prefixo atual
    printf("%s", prefixo);
    
    // Exibe o conector
    if (nivel > 0) {
        printf("%s", ehUltimo ? "└── " : "├── ");
    }
    
    // Exibe as chaves do nó
    printf("[");
    for (int i = 0; i < no->num_chaves; i++) {
        printf("%s", no->chaves[i].cpf);
        if (i < no->num_chaves - 1) printf(", ");
    }
    printf("]\n");
    
    // Prepara o prefixo para os filhos
    char novoPrefixo[256];
    if (nivel == 0) {
        sprintf(novoPrefixo, "");
    } else {
        sprintf(novoPrefixo, "%s%s", prefixo, ehUltimo ? "    " : "│   ");
    }
    
    // Exibe os filhos
    if (!no->folha) {
        for (int i = 0; i <= no->num_chaves; i++) {
            exibirArvoreBVisual(no->filhos[i], nivel + 1, novoPrefixo, i == no->num_chaves);
        }
    }
}

// Função wrapper para exibir a árvore
void exibirArvoreB(NoArvB* raiz) {
    printf("\n=== ÁRVORE B (T=%d) ===\n\n", ORDEM);
    if (raiz == NULL || raiz->num_chaves == 0) {
        printf("Árvore vazia!\n");
        return;
    }
    exibirArvoreBVisual(raiz, 0, "", true);
    printf("\n");
}

// Função para exibir de forma hierárquica simples
void exibirArvoreBSimples(NoArvB* no, int nivel) {
    if (no == NULL) return;
    
    // Espaçamento baseado no nível
    for (int i = 0; i < nivel * 4; i++) {
        printf(" ");
    }
    
    // Exibe as chaves do nó atual
    printf("[");
    for (int i = 0; i < no->num_chaves; i++) {
        printf("%s", no->chaves[i].cpf);
        if (i < no->num_chaves - 1) printf(", ");
    }
    printf("]\n");
    
    if (!no->folha) {
        for (int i = 0; i <= no->num_chaves; i++) {
            exibirArvoreBSimples(no->filhos[i], nivel + 1);
        }
    }
}

// Função para liberar a memória da árvore
void liberarArvoreB(NoArvB* no) {
    if (no == NULL) return;
    
    if (!no->folha) {
        for (int i = 0; i <= no->num_chaves; i++) {
            liberarArvoreB(no->filhos[i]);
        }
    }
    
    free(no);
}

// Função auxiliar para criar um ItemIndice
ItemIndice criarItem(const char* cpf, long posicao) {
    ItemIndice item;
    strcpy(item.cpf, cpf);
    item.posicao = posicao;
    return item;
}

// Função para exibir todos os elementos em ordem (percurso inorder)
void exibirInorder(NoArvB* no) {
    if (no == NULL) return;
    
    int i;
    for (i = 0; i < no->num_chaves; i++) {
        if (!no->folha) {
            exibirInorder(no->filhos[i]);
        }
        printf("%s (pos: %ld)\n", no->chaves[i].cpf, no->chaves[i].posicao);
    }
    
    if (!no->folha) {
        exibirInorder(no->filhos[i]);
    }
}

int main(){
    NoArvB *raiz = NULL;
    int opcao;

    raiz = ArvB_carregar(ARQ_INDICE);

    exibirArvoreB(raiz);

    ArvB_destruir(raiz);
    return 0;
}
