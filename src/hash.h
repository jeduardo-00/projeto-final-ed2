#ifndef HASH_H
#define HASH_H

typedef struct No {
    char id[12];           
    struct No* proximo;    
} No;

typedef struct {
    No** tabela;           
    int tamanho;           // Criação da tabela Hash
    int quantidade;        
    int contador_colisoes; 
} TabelaHash;



TabelaHash* hash_criar(int tamanho_inicial);

unsigned int hash_funcao(const char* texto, int tamanho_tabela);

int hash_inserir(TabelaHash* th, const char* id);

int hash_buscar(TabelaHash* th, const char* id);

void hash_liberar(TabelaHash* th); 




double hash_obter_fator_carga(TabelaHash* th);

int hash_obter_colisoes(TabelaHash* th);


#endif // HASH_H // hash