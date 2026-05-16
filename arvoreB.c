#include "arvoreB.h"

// ============================================
// CRIA UM NOVO NÓ
// ============================================
NoArvB* ArvB_criar(int folha) {
    NoArvB *novo = (NoArvB*)malloc(sizeof(NoArvB));
    if(!novo) return NULL;
    
    novo->num_chaves = 0;
    novo->folha = folha;
    
    for(int i = 0; i < MAX_FILHOS; i++)
        novo->filhos[i] = NULL;
    
    for(int i = 0; i < MAX_CHAVES; i++) {
        novo->chaves[i].chave[0] = '\0';
        novo->chaves[i].posicao = -1;
    }
    
    return novo;
}


void ArvB_destruir(NoArvB *raiz) {
    if(!raiz) return;
    
    if(!raiz->folha) {
        for(int i = 0; i <= raiz->num_chaves; i++)
            ArvB_destruir(raiz->filhos[i]);
    }
    free(raiz);
}

long ArvB_buscar(NoArvB *raiz, char *chave) {
    NoArvB *no = raiz;
    if(!no) return -1;
    
    while(no) {
        int i = 0;
        while(i < no->num_chaves && strcmp(chave, no->chaves[i].chave) > 0)
            i++;
        
        if(i < no->num_chaves && strcmp(chave, no->chaves[i].chave) == 0)
            return no->chaves[i].posicao;
        
        if(no->folha) return -1;
        no = no->filhos[i];
    }
    return -1;
}

void ArvB_inserir(NoArvB **raiz, char *chave, long posicao) {
    if(!*raiz) {
        *raiz = ArvB_criar(1);
    }
    
    if((*raiz)->num_chaves == MAX_CHAVES) {
        NoArvB *nova_raiz = ArvB_criar(0);
        nova_raiz->filhos[0] = *raiz;
        ArvB_dividir(nova_raiz, 0);
        *raiz = nova_raiz;
    }
    
    ArvB_inserir_nao_cheio(*raiz, chave, posicao);
}

void ArvB_inserir_nao_cheio(NoArvB *no, char *chave, long posicao) {
    int i = no->num_chaves - 1;
    
    if(no->folha) {
        while(i >= 0 && strcmp(chave, no->chaves[i].chave) < 0) {
            no->chaves[i+1] = no->chaves[i];
            i--;
        }
        strcpy(no->chaves[i+1].chave, chave);
        no->chaves[i+1].posicao = posicao;
        no->num_chaves++;
    } else {
        while(i >= 0 && strcmp(chave, no->chaves[i].chave) < 0)
            i--;
        i++;
        
        if(no->filhos[i]->num_chaves == MAX_CHAVES) {
            ArvB_dividir(no, i);
            if(strcmp(chave, no->chaves[i].chave) > 0)
                i++;
        }
        ArvB_inserir_nao_cheio(no->filhos[i], chave, posicao);
    }
}


void ArvB_dividir(NoArvB *pai, int pos_filho) {
    NoArvB *filho = pai->filhos[pos_filho];
    NoArvB *novo = ArvB_criar(filho->folha);
    
    novo->num_chaves = MIN_CHAVES;
    for(int i = 0; i < MIN_CHAVES; i++)
        novo->chaves[i] = filho->chaves[i + ORDEM];
    
    if(!filho->folha) {
        for(int i = 0; i <= MIN_CHAVES; i++)
            novo->filhos[i] = filho->filhos[i + ORDEM];
    }
    
    filho->num_chaves = MIN_CHAVES;
    
    for(int i = pai->num_chaves; i >= pos_filho + 1; i--)
        pai->filhos[i+1] = pai->filhos[i];
    pai->filhos[pos_filho + 1] = novo;
    
    for(int i = pai->num_chaves - 1; i >= pos_filho; i--)
        pai->chaves[i+1] = pai->chaves[i];
    pai->chaves[pos_filho] = filho->chaves[ORDEM - 1];
    pai->num_chaves++;
}


int ArvB_remover(NoArvB **raiz, char *chave) {
    if(!*raiz) return 0;
    
    int removido = ArvB_remover_interno(*raiz, chave);
    
    if((*raiz)->num_chaves == 0 && !(*raiz)->folha) {
        NoArvB *nova = (*raiz)->filhos[0];
        free(*raiz);
        *raiz = nova;
    }
    return removido;
}


int ArvB_remover_interno(NoArvB *no, char *chave) {
    int i = 0;
    while(i < no->num_chaves && strcmp(chave, no->chaves[i].chave) > 0)
        i++;
    
    if(i < no->num_chaves && strcmp(chave, no->chaves[i].chave) == 0) {
        if(no->folha) {
            for(int j = i; j < no->num_chaves - 1; j++)
                no->chaves[j] = no->chaves[j+1];
            no->num_chaves--;
            return 1;
        } else {
            NoArvB *atual = no->filhos[i+1];
            while(!atual->folha)
                atual = atual->filhos[0];
            strcpy(no->chaves[i].chave, atual->chaves[0].chave);
            no->chaves[i].posicao = atual->chaves[0].posicao;
            return ArvB_remover_interno(no->filhos[i+1], no->chaves[i].chave);
        }
    }
    
    if(no->folha) return 0;
    
    if(no->filhos[i]->num_chaves == MIN_CHAVES) {
        if(i > 0 && no->filhos[i-1]->num_chaves > MIN_CHAVES) {
            NoArvB *esq = no->filhos[i-1];
            NoArvB *dir = no->filhos[i];
            
            for(int j = dir->num_chaves; j > 0; j--)
                dir->chaves[j] = dir->chaves[j-1];
            dir->chaves[0] = no->chaves[i-1];
            dir->num_chaves++;
            
            strcpy(no->chaves[i-1].chave, esq->chaves[esq->num_chaves-1].chave);
            no->chaves[i-1].posicao = esq->chaves[esq->num_chaves-1].posicao;
            esq->num_chaves--;
        }
        else if(i < no->num_chaves && no->filhos[i+1]->num_chaves > MIN_CHAVES) {
            NoArvB *esq = no->filhos[i];
            NoArvB *dir = no->filhos[i+1];
            
            esq->chaves[esq->num_chaves] = no->chaves[i];
            esq->num_chaves++;
            
            strcpy(no->chaves[i].chave, dir->chaves[0].chave);
            no->chaves[i].posicao = dir->chaves[0].posicao;
            
            for(int j = 0; j < dir->num_chaves - 1; j++)
                dir->chaves[j] = dir->chaves[j+1];
            dir->num_chaves--;
        }
        else {
            if(i < no->num_chaves) {
                NoArvB *esq = no->filhos[i];
                NoArvB *dir = no->filhos[i+1];
                
                esq->chaves[esq->num_chaves] = no->chaves[i];
                esq->num_chaves++;
                
                for(int j = 0; j < dir->num_chaves; j++)
                    esq->chaves[esq->num_chaves + j] = dir->chaves[j];
                esq->num_chaves += dir->num_chaves;
                
                for(int j = i; j < no->num_chaves - 1; j++)
                    no->chaves[j] = no->chaves[j+1];
                for(int j = i+1; j < no->num_chaves; j++)
                    no->filhos[j] = no->filhos[j+1];
                no->num_chaves--;
                free(dir);
            }
        }
    }
    
    return ArvB_remover_interno(no->filhos[i], chave);
}


void ArvB_listar(NoArvB *raiz, void (*exibir)(char*, long)) {
    if(!raiz) return;
    ArvB_listar_rec(raiz, exibir);
}

void ArvB_listar_rec(NoArvB *no, void (*exibir)(char*, long)) {
    if(!no) return;
    
    for(int i = 0; i < no->num_chaves; i++) {
        if(!no->folha)
            ArvB_listar_rec(no->filhos[i], exibir);
        exibir(no->chaves[i].chave, no->chaves[i].posicao);
    }
    if(!no->folha)
        ArvB_listar_rec(no->filhos[no->num_chaves], exibir);
}


void ArvB_salvar(NoArvB *raiz, const char *nome_arquivo) {
    FILE *fp = fopen(nome_arquivo, "wb");
    if(!fp) return;
    
    int existe = (raiz != NULL);
    fwrite(&existe, sizeof(int), 1, fp);
    if(raiz) ArvB_salvar_rec(fp, raiz);
    fclose(fp);
}

void ArvB_salvar_rec(FILE *fp, NoArvB *no) {
    if(!no) return;
    
    fwrite(&no->num_chaves, sizeof(int), 1, fp);
    fwrite(&no->folha, sizeof(int), 1, fp);
    fwrite(no->chaves, sizeof(ItemIndice), no->num_chaves, fp);
    
    if(!no->folha) {
        for(int i = 0; i <= no->num_chaves; i++) {
            int existe = (no->filhos[i] != NULL);
            fwrite(&existe, sizeof(int), 1, fp);
            if(no->filhos[i]) ArvB_salvar_rec(fp, no->filhos[i]);
        }
    }
}


NoArvB* ArvB_carregar(const char *nome_arquivo) {
    FILE *fp = fopen(nome_arquivo, "rb");
    if(!fp) return NULL;
    
    int existe;
    fread(&existe, sizeof(int), 1, fp);
    if(!existe) {
        fclose(fp);
        return NULL;
    }
    
    NoArvB *raiz = ArvB_carregar_rec(fp);
    fclose(fp);
    return raiz;
}

NoArvB* ArvB_carregar_rec(FILE *fp) {
    NoArvB *no = (NoArvB*)malloc(sizeof(NoArvB));
    if(!no) return NULL;
    
    fread(&no->num_chaves, sizeof(int), 1, fp);
    fread(&no->folha, sizeof(int), 1, fp);
    fread(no->chaves, sizeof(ItemIndice), no->num_chaves, fp);
    
    for(int i = no->num_chaves; i < MAX_CHAVES; i++) {
        no->chaves[i].chave[0] = '\0';
        no->chaves[i].posicao = -1;
    }
    
    if(!no->folha) {
        for(int i = 0; i <= no->num_chaves; i++) {
            int existe;
            fread(&existe, sizeof(int), 1, fp);
            no->filhos[i] = existe ? ArvB_carregar_rec(fp) : NULL;
        }
        for(int i = no->num_chaves + 1; i < MAX_FILHOS; i++)
            no->filhos[i] = NULL;
    } else {
        for(int i = 0; i < MAX_FILHOS; i++)
            no->filhos[i] = NULL;
    }
    
    return no;
}