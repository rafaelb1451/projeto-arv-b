#include "arvb.h"

// FUNÇÃO: ArvB_criar

NoArvB *ArvB_criar(int folha) {
    NoArvB  *novo = (NoArvB *)malloc(sizeof(NoArvB ));
    
    if(novo == NULL) {
        printf("ERRO: Memoria insuficiente!\n");
        return NULL;
    }
    
    novo->num_chaves = 0;
    novo->folha = folha;
    
    for(int i = 0; i < MAX_FILHOS; i++) {
        novo->filhos[i] = NULL;
    }
    
    for(int i = 0; i < MAX_CHAVES; i++) {
        novo->chaves[i].cpf[0] = '\0';
        novo->chaves[i].posicao = -1;
    }
    
    return novo;
}

// Libera toda a memória alocada pela árvore

void ArvB_destruir(NoArvB  *raiz) {
    if(raiz == NULL) return;
    
    if(!raiz->folha) {
        for(int i = 0; i <= raiz->num_chaves; i++) {
            ArvB_destruir(raiz->filhos[i]);
        }
    }
    
    free(raiz);
}

// Busca um CPF na árvore e retorna sua posição retorna a Posição no arquivo ou -1 se não encontrado

long ArvB_buscar(NoArvB  *raiz, char *cpf) {
    NoArvB  *no = raiz;
    
    if(no == NULL) return -1;
    
    while(no != NULL) {
        int i = 0;
        while(i < no->num_chaves && strcmp(cpf, no->chaves[i].cpf) > 0) {
            i++;
        }
        
        if(i < no->num_chaves && strcmp(cpf, no->chaves[i].cpf) == 0) {
            return no->chaves[i].posicao;
        }
        
        if(no->folha) return -1;
        
        no = no->filhos[i];
    }
    
    return -1;
}

// OBJETIVO: Insere um novo Cadastro na árvore (interface principal)
void ArvB_inserir(NoArvB  **raiz, char *cpf, long posicao) {
    if(*raiz == NULL) {
        *raiz = ArvB_criar(1);
    }
    
    if((*raiz)->num_chaves == MAX_CHAVES) {
        NoArvB  *nova_raiz = ArvB_criar(0);
        nova_raiz->filhos[0] = *raiz;
        ArvB_dividir(nova_raiz, 0);
        *raiz = nova_raiz;
        ArvB_inserir_nao_cheio(*raiz, cpf, posicao);
    } else {
        ArvB_inserir_nao_cheio(*raiz, cpf, posicao);
    }
}

// Insere um CPF em um nó que não está cheio

void ArvB_inserir_nao_cheio(NoArvB  *no, char *cpf, long posicao) {
    int i = no->num_chaves - 1;
    
    if(no->folha) {
        while(i >= 0 && strcmp(cpf, no->chaves[i].cpf) < 0) {
            no->chaves[i+1] = no->chaves[i];
            i--;
        }
        
        strcpy(no->chaves[i+1].cpf, cpf);
        no->chaves[i+1].posicao = posicao;
        no->num_chaves++;
    } else {
        while(i >= 0 && strcmp(cpf, no->chaves[i].cpf) < 0) {
            i--;
        }
        i++;
        
        if(no->filhos[i]->num_chaves == MAX_CHAVES) {
            ArvB_dividir(no, i);
            if(strcmp(cpf, no->chaves[i].cpf) > 0) {
                i++;
            }
        }
        
        ArvB_inserir_nao_cheio(no->filhos[i], cpf, posicao);
    }
}

// Divide um nó filho cheio em dois e promove a chave do meio para o pai
void ArvB_dividir(NoArvB  *pai, int pos_filho) {
    NoArvB  *filho_cheio = pai->filhos[pos_filho];
    NoArvB  *novo_filho = ArvB_criar(filho_cheio->folha);
    
    novo_filho->num_chaves = MIN_CHAVES;
    
    for(int i = 0; i < MIN_CHAVES; i++) {
        novo_filho->chaves[i] = filho_cheio->chaves[i + ORDEM];
    }
    
    if(!filho_cheio->folha) {
        for(int i = 0; i <= MIN_CHAVES; i++) {
            novo_filho->filhos[i] = filho_cheio->filhos[i + ORDEM];
        }
    }
    
    filho_cheio->num_chaves = MIN_CHAVES;
    
    for(int i = pai->num_chaves; i >= pos_filho + 1; i--) {
        pai->filhos[i+1] = pai->filhos[i];
    }
    pai->filhos[pos_filho + 1] = novo_filho;
    
    for(int i = pai->num_chaves - 1; i >= pos_filho; i--) {
        pai->chaves[i+1] = pai->chaves[i];
    }
    pai->chaves[pos_filho] = filho_cheio->chaves[ORDEM - 1];
    pai->num_chaves++;
}

// remove um CPF da árvore e retorna 1 se removido ou 0 se não encontrado

int ArvB_remover(NoArvB  **raiz, char *cpf) {
    if(*raiz == NULL) {
        printf("Arvore vazia!\n");
        return 0;
    }
    
    int removido = ArvB_remover_interno(*raiz, cpf);
    
    if((*raiz)->num_chaves == 0 && !(*raiz)->folha) {
        NoArvB  *nova_raiz = (*raiz)->filhos[0];
        free(*raiz);
        *raiz = nova_raiz;
    }
    
    return removido;
}

//remove um CPF de um nó interno 
int ArvB_remover_interno(NoArvB  *no, char *cpf) {
    int i = 0;
    
    while(i < no->num_chaves && strcmp(cpf, no->chaves[i].cpf) > 0) {
        i++;
    }
    
    // Caso 1: Encontrou o CPF neste nó
    if(i < no->num_chaves && strcmp(cpf, no->chaves[i].cpf) == 0) {
        if(no->folha) {
            return ArvB_remover_folha(no, i);
        } else {
            return ArvB_remover_interno_no(no, i);
        }
    }
    
    // Caso 2: Não encontrou
    if(no->folha) {
        printf("CPF %s nao encontrado!\n", cpf);
        return 0;
    }
    
    if(no->filhos[i]->num_chaves == MIN_CHAVES) {
        ArvB_rebalancear(no, i);
    }
    
    return ArvB_remover_interno(no->filhos[i], cpf);
}

//remove um CPF de um nó folha
int ArvB_remover_folha(NoArvB  *no, int pos) {
    for(int i = pos; i < no->num_chaves - 1; i++) {
        no->chaves[i] = no->chaves[i+1];
    }
    
    no->chaves[no->num_chaves - 1].cpf[0] = '\0';
    no->chaves[no->num_chaves - 1].posicao = -1;
    no->num_chaves--;
    
    printf("Cliente removido com sucesso!\n");
    return 1;
}

//remove um CPF de um nó interno
int ArvB_remover_interno_no(NoArvB  *no, int pos) {
    char cpf_sucessor[CPF_TAM];
    long pos_sucessor;
    NoArvB  *filho_esq = no->filhos[pos];
    NoArvB  *filho_dir = no->filhos[pos + 1];
    
    if(filho_esq->num_chaves > MIN_CHAVES) {
        NoArvB  *atual = filho_esq;
        while(!atual->folha) {
            atual = atual->filhos[atual->num_chaves];
        }
        strcpy(cpf_sucessor, atual->chaves[atual->num_chaves - 1].cpf);
        pos_sucessor = atual->chaves[atual->num_chaves - 1].posicao;
        ArvB_remover_interno(filho_esq, cpf_sucessor);
    }
    else if(filho_dir->num_chaves > MIN_CHAVES) {
        NoArvB  *atual = filho_dir;
        while(!atual->folha) {
            atual = atual->filhos[0];
        }
        strcpy(cpf_sucessor, atual->chaves[0].cpf);
        pos_sucessor = atual->chaves[0].posicao;
        ArvB_remover_interno(filho_dir, cpf_sucessor);
    }
    else {
        ArvB_fundir(no, pos);
        return ArvB_remover_interno(filho_esq, no->chaves[pos].cpf);
    }
    
    strcpy(no->chaves[pos].cpf, cpf_sucessor);
    no->chaves[pos].posicao = pos_sucessor;
    
    return 1;
}

// Pega uma chave emprestada do irmão direito
void ArvB_emprestar_direita(NoArvB  *pai, int pos_filho) {
    NoArvB  *filho_esq = pai->filhos[pos_filho];
    NoArvB  *filho_dir = pai->filhos[pos_filho + 1];
    
    filho_esq->chaves[filho_esq->num_chaves] = pai->chaves[pos_filho];
    filho_esq->num_chaves++;
    
    strcpy(pai->chaves[pos_filho].cpf, filho_dir->chaves[0].cpf);
    pai->chaves[pos_filho].posicao = filho_dir->chaves[0].posicao;
    
    if(!filho_esq->folha) {
        filho_esq->filhos[filho_esq->num_chaves] = filho_dir->filhos[0];
    }
    
    for(int i = 0; i < filho_dir->num_chaves - 1; i++) {
        filho_dir->chaves[i] = filho_dir->chaves[i+1];
    }
    
    if(!filho_dir->folha) {
        for(int i = 0; i < filho_dir->num_chaves; i++) {
            filho_dir->filhos[i] = filho_dir->filhos[i+1];
        }
    }
    
    filho_dir->num_chaves--;
}

// Pega uma chave emprestada do irmão esquerdo
void ArvB_emprestar_esquerda(NoArvB  *pai, int pos_filho) {
    NoArvB  *filho_esq = pai->filhos[pos_filho - 1];
    NoArvB  *filho_dir = pai->filhos[pos_filho];
    
    for(int i = filho_dir->num_chaves; i > 0; i--) {
        filho_dir->chaves[i] = filho_dir->chaves[i-1];
    }
    
    filho_dir->chaves[0] = pai->chaves[pos_filho - 1];
    filho_dir->num_chaves++;
    
    strcpy(pai->chaves[pos_filho - 1].cpf,
           filho_esq->chaves[filho_esq->num_chaves - 1].cpf);
    pai->chaves[pos_filho - 1].posicao =
           filho_esq->chaves[filho_esq->num_chaves - 1].posicao;
    
    if(!filho_dir->folha) {
        for(int i = filho_dir->num_chaves; i > 0; i--) {
            filho_dir->filhos[i] = filho_dir->filhos[i-1];
        }
        filho_dir->filhos[0] = filho_esq->filhos[filho_esq->num_chaves];
    }
    
    filho_esq->num_chaves--;
}

// Junta um nó com seu irmão e promove a chave do pai para o nó resultante
void ArvB_fundir(NoArvB  *pai, int pos_filho) {
    NoArvB  *filho_esq = pai->filhos[pos_filho];
    NoArvB  *filho_dir = pai->filhos[pos_filho + 1];
    
    filho_esq->chaves[filho_esq->num_chaves] = pai->chaves[pos_filho];
    filho_esq->num_chaves++;
    
    for(int i = 0; i < filho_dir->num_chaves; i++) {
        filho_esq->chaves[filho_esq->num_chaves + i] = filho_dir->chaves[i];
    }
    
    if(!filho_esq->folha) {
        for(int i = 0; i <= filho_dir->num_chaves; i++) {
            filho_esq->filhos[filho_esq->num_chaves + i] = filho_dir->filhos[i];
        }
    }
    
    filho_esq->num_chaves += filho_dir->num_chaves;
    
    for(int i = pos_filho; i < pai->num_chaves - 1; i++) {
        pai->chaves[i] = pai->chaves[i+1];
    }
    
    for(int i = pos_filho + 1; i < pai->num_chaves; i++) {
        pai->filhos[i] = pai->filhos[i+1];
    }
    
    pai->num_chaves--;
    free(filho_dir);
}

// Rebalanceia o nó filho que tem menos de MIN_CHAVES chaves
void ArvB_rebalancear(NoArvB  *pai, int pos_filho) {
    if(pos_filho > 0 && pai->filhos[pos_filho - 1]->num_chaves > MIN_CHAVES) {
        ArvB_emprestar_esquerda(pai, pos_filho);
    }
    else if(pos_filho < pai->num_chaves &&
            pai->filhos[pos_filho + 1]->num_chaves > MIN_CHAVES) {
        ArvB_emprestar_direita(pai, pos_filho);
    }
    else {
        if(pos_filho < pai->num_chaves) {
            ArvB_fundir(pai, pos_filho);
        } else {
            ArvB_fundir(pai, pos_filho - 1);
        }
    }
}

// Lista os clientes em ordem crescente de CPF e salva em um arquivo texto
void ArvB_listar_ordenado(NoArvB  *raiz, const char *nome_arquivo) {
    if(raiz == NULL) {
        printf("Nenhum cliente cadastrado!\n");
        return;
    }
    
    FILE *dados = fopen(nome_arquivo, "r");
    if(dados == NULL) {
        printf("Erro ao abrir arquivo de dados!\n");
        return;
    }
    
    printf("\n");
    printf("============================================================================\n");
    printf("                       LISTA DE CLIENTES (ORDENADO POR CPF)\n");
    printf("============================================================================\n");
    printf("%-15s | %-3s | %-30s | %-25s\n", "CPF", "ID", "NOME", "EMAIL");
    printf("----------------------------------------------------------------------------\n");
    
    int total = ArvB_listar_ordenado_rec(raiz, dados);
    
    printf("----------------------------------------------------------------------------\n");
    printf("Total de clientes: %d\n", total);
    printf("============================================================================\n");
    
    fclose(dados);
}

// Função recursiva para listar clientes em ordem crescente de CPF
int ArvB_listar_ordenado_rec(NoArvB  *no, FILE *dados) {
    int count = 0;
    int i;
    
    if(no == NULL) return 0;
    
    for(i = 0; i < no->num_chaves; i++) {
        if(!no->folha) {
            count += ArvB_listar_ordenado_rec(no->filhos[i], dados);
        }
        
        if(buscar_e_exibir_cliente(dados, no->chaves[i].cpf, no->chaves[i].posicao)) {
            count++;
        }
    }
    
    if(!no->folha) {
        count += ArvB_listar_ordenado_rec(no->filhos[i], dados);
    }
    
    return count;
}

// Busca um cliente no arquivo de dados usando a posição e exibe seus dados formatados
int buscar_e_exibir_cliente(FILE *dados, char *cpf, long posicao) {
    Cliente c;
    char linha[300];
    long pos_atual = ftell(dados);
    
    fseek(dados, posicao, SEEK_SET);
    
    if(fgets(linha, 300, dados) == NULL) {
        fseek(dados, pos_atual, SEEK_SET);
        return 0;
    }
    
    if(!cliente_de_string(&c, linha)) {
        fseek(dados, pos_atual, SEEK_SET);
        return 0;
    }
    
    printf("%-15s | %-3d | %-30s | %-25s\n",
           c.cpf, c.idade, c.nome, c.email);
    
    fseek(dados, pos_atual, SEEK_SET);
    return 1;
}

// ============================================
// SALVA A ÁRVORE COMPLETA
// ============================================
void ArvB_salvar(NoArvB *raiz, const char *nome_arquivo) {
    FILE *fp = fopen(nome_arquivo, "wb");
    if(fp == NULL) {
        printf("Erro ao abrir arquivo para salvar!\n");
        return;
    }
    
    // Salva a árvore recursivamente
    ArvB_salvar_no(fp, raiz);
    
    fclose(fp);
    printf("indice salvo com sucesso!\n");
}

void ArvB_salvar_no(FILE *fp, NoArvB *no) {
    if(no == NULL) {
        int nulo = 1;
        fwrite(&nulo, sizeof(int), 1, fp);
        return;
    }
    
    int nulo = 0;
    fwrite(&nulo, sizeof(int), 1, fp);
    
    // Salva os dados do nó
    fwrite(&no->num_chaves, sizeof(int), 1, fp);
    fwrite(&no->folha, sizeof(int), 1, fp);
    fwrite(no->chaves, sizeof(ItemIndice), no->num_chaves, fp);
    
    // Salva os filhos recursivamente
    if(!no->folha) {
        for(int i = 0; i <= no->num_chaves; i++) {
            ArvB_salvar_no(fp, no->filhos[i]);
        }
    }
}

// ============================================
// CARREGA A ÁRVORE COMPLETA
// ============================================
NoArvB* ArvB_carregar(const char *nome_arquivo) {
    FILE *fp = fopen(nome_arquivo, "rb");
    if(fp == NULL) {
        printf("Arquivo de indice nao encontrado.\n");
        printf("Novo arquivo gerado!\n");
        return NULL;
    }
    
    NoArvB *raiz = ArvB_carregar_no(fp);
    fclose(fp);
    
    printf("indice carregado com sucesso!\n");
    return raiz;
}

NoArvB* ArvB_carregar_no(FILE *fp) {
    int nulo;
    fread(&nulo, sizeof(int), 1, fp);
    
    if(nulo) return NULL;
    
    // Aloca e carrega o nó
    NoArvB *no = (NoArvB*)malloc(sizeof(NoArvB));
    
    fread(&no->num_chaves, sizeof(int), 1, fp);
    fread(&no->folha, sizeof(int), 1, fp);
    fread(no->chaves, sizeof(ItemIndice), no->num_chaves, fp);
    
    // Inicializa as chaves restantes
    for(int i = no->num_chaves; i < MAX_CHAVES; i++) {
        no->chaves[i].cpf[0] = '\0';
        no->chaves[i].posicao = -1;
    }
    
    // Carrega os filhos
    if(!no->folha) {
        for(int i = 0; i <= no->num_chaves; i++) {
            no->filhos[i] = ArvB_carregar_no(fp);
        }
        
        // Inicializa filhos restantes como NULL
        for(int i = no->num_chaves + 1; i < MAX_FILHOS; i++) {
            no->filhos[i] = NULL;
        }
    } else {
        for(int i = 0; i < MAX_FILHOS; i++) {
            no->filhos[i] = NULL;
        }
    }
    
    return no;
}