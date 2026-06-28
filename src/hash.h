#ifndef HASH_H
#define HASH_H

typedef struct No {
    char id[12];           // Armazena o ID do usuário (Formato [8 letras][3 números] + '\0')
    struct No* proximo;    // Ponteiro para o próximo nó da lista encadeada em caso de colisão
} No;


 
typedef struct {           //Encapsula o vetor principal e as variáveis necessárias para as métricas do relatório.
    No** tabela;           // Vetor de ponteiros (ponteiro duplo) que armazena os nós heads das listas
    int tamanho;           // Tamanho máximo da tabela (M) - Ideal que seja um número primo
    int quantidade;        // Quantidade de elementos atualmente inseridos (N)
    int contador_colisoes; // Armazena a quantidade de colisões reais identificadas no sistema
} TabelaHash;


// FUNÇÕES PRINCIPAIS DE MANIPULAÇÃO DA HASH


TabelaHash* hash_criar(int tamanho_inicial);    // Cria a estrutura na memória RAM e limpa as posições

unsigned int hash_funcao(const char* texto, int tamanho_tabela);    // Transforma o ID de texto em um índice numérico válido (Algoritmo DJB2)

int hash_inserir(TabelaHash* th, const char* id);        // Insere um novo usuário na tabela se ele já não existir

int hash_buscar(TabelaHash* th, const char* id);    // Procura por um usuário na tabela (Retorna 1 se achar, 0 se não)

void hash_liberar(TabelaHash* th);       // Libera toda a memória alocada dinamicamente para evitar vazamentos (Memory Leaks)

//FUNÇÕES ULTILIZADAS NO RELATÓRIO

double hash_obter_fator_carga(TabelaHash* th);      // Retorna o Fator de Carga (Alpha = N / M) para análise de ocupação
int hash_obter_colisoes(TabelaHash* th);            // Retorna a quantidade total de colisões identificadas

#endif // HASH_H