#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash.h"


TabelaHash* hash_criar(int tamanho_inicial) {
    TabelaHash* th = (TabelaHash*)malloc(sizeof(TabelaHash));
    if (!th) return NULL;

    th->tamanho = tamanho_inicial;
    th->quantidade = 0;
    th->contador_colisoes = 0;
    
   
    th->tabela = (No**)calloc(tamanho_inicial, sizeof(No*));
    if (!th->tabela) {
        free(th);
        return NULL;
    }
    return th;
}


unsigned int hash_funcao(const char* texto, int tamanho_tabela) {
    unsigned long hash = 5381;
    int caractere;
    
    while ((caractere = *texto++)) {
        // Multiplica o valor por 33 e soma o caractere ASCII atual
        hash = ((hash << 5) + hash) + caractere; 
    }
    return hash % tamanho_tabela;
}


int hash_inserir(TabelaHash* th, const char* id) {
    if (!th) return 0;

    unsigned int indice = hash_funcao(id, th->tamanho);
    No* atual = th->tabela[indice];

    
    while (atual != NULL) {
        if (strcmp(atual->id, id) == 0) {
            return 0; 
        }
        atual = atual->proximo;
    }

    
    if (th->tabela[indice] != NULL) {
        th->contador_colisoes++;
    }

    
    No* novo_no = (No*)malloc(sizeof(No));
    if (!novo_no) return 0;
    
   
    strncpy(novo_no->id, id, sizeof(novo_no->id) - 1);
    novo_no->id[sizeof(novo_no->id) - 1] = '\0';
  
    
    novo_no->proximo = th->tabela[indice];
    th->tabela[indice] = novo_no;
    th->quantidade++;

    return 1;
} 


int hash_buscar(TabelaHash* th, const char* id) {
    if (!th) return 0;

    unsigned int indice = hash_funcao(id, th->tamanho);
    No* atual = th->tabela[indice];

    
    while (atual != NULL) {
        if (strcmp(atual->id, id) == 0) {
            return 1; 
        }
        atual = atual->proximo;
    }
    return 0;
}


double hash_obter_fator_carga(TabelaHash* th) {
    if (!th || th->tamanho == 0) return 0.0;
    return (double)th->quantidade / th->tamanho;
}


int hash_obter_colisoes(TabelaHash* th) {
    if (!th) return 0;
    return th->contador_colisoes;
}


void hash_liberar(TabelaHash* th) {
    if (!th) return;

    for (int i = 0; i < th->tamanho; i++) {
        No* atual = th->tabela[i];
        while (atual != NULL) {
            No* temporario = atual;
            atual = atual->proximo;
            free(temporario);
        }
    }
    free(th->tabela);
    free(th);
}