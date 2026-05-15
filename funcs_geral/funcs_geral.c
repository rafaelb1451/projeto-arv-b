#include "funcs_geral.h"

void limparBuffer(){
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void menu(){
    printf("========================================\n");
    printf("   BEM-VINDO AO SISTEMA DE CADASTRO    \n");
    printf("========================================\n");
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
long inserir_no_arquivo(Cliente *c){
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

int buscar_no_arquivo(long posicao, Cliente *c){
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

int modificar_no_arquivo(long posicao, Cliente *c){
    FILE *dados = fopen(ARQ_DADOS, "r+");
    if(dados == NULL) return 0;
    
    fseek(dados, posicao, SEEK_SET);
    
    char buffer[300];
    cliente_para_string(c, buffer, 300);
    fprintf(dados, "%s", buffer);
    
    fclose(dados);
    return 1;
}

NoArvB *reiniciarDados(NoArvB *raiz){
    NoArvB *novaRaiz = NULL;

    
}
