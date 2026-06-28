#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash.h"


 //CRIAÇÃO DA TABELA HASH
 //Aloca a estrutura de controle e o vetor principal na memória.


 TabelaHash* hash_criar(int tamanho_inicial) {
    TabelaHash* th = (TabelaHash*)malloc(sizeof(TabelaHash));    // Aloca a estrutura gerenciando o tamanho e quantidade e colisões
    if (!th) return NULL;                                        // Proteção contra falta de memória na RAM 

    th->tamanho = tamanho_inicial;
    th->quantidade = 0;
    th->contador_colisoes = 0;
    
       
        
    th->tabela = (No**)calloc(tamanho_inicial, sizeof(No*));       // Aloca o vetor de ponteiros para nós usando calloc
    if (!th->tabela) {                                             // Isso é vital para sabermos se uma posição do vetor está livre ou já ocupada
        free(th);                                                  // Libera o bloco anterior se esta segunda alocação falhar
        return NULL;
    }
    return th;
}



unsigned int hash_funcao(const char* texto, int tamanho_tabela) {   //FUNÇÃO HASH - MODELO MATEMÁTICO DJB2

    unsigned long hash = 5381;                     // Número primo inicial pré determinado pela função escolhido empiricamente por gerar menos colisões
    int caractere;
    
    while ((caractere = *texto++)) {               // Laço que percorre letra por letra da string até encontrar o fim '\0'
                                    
                          
        hash = ((hash << 5) + hash) + caractere;  // Multiplicação por 33 usando deslocamento de bits (hash << 5 equivalendo a hash * 32) + o próprio hash.
    }
    
   return hash % tamanho_tabela;                  // O operador resto % garante que o resultado caia dentro dos limites dos índices da nossa tabela
}



int hash_inserir(TabelaHash* th, const char* id) {    //INSERÇÃO DE ELEMENTOS (RF01 / RF04)
    if (!th) return 0;                                   //Insere o novo usuário no início da lista correspondente (Complexidade O(1)).

     unsigned int indice = hash_funcao(id, th->tamanho);  // Passa o ID na função hash para descobrir em qual índice do vetor ele deve entrar
    No* atual = th->tabela[indice];

    while(atual != NULL) {                               // Varre a lista encadeada deste índice específico para garantir que o ID não está duplicado
        if (strcmp(atual->id, id) == 0) {
            return 0;                                   // O ID já está cadastrado. Aborta a inserção para não gerar duplicidade.
        }
        atual = atual->proximo;
    }

    
    if (th->tabela[indice] != NULL) {                  // Se o vetor naquele índice não for NULL, significa que um elemento já mora lá.
        th->contador_colisoes++;                       // Logo, uma colisão real de chaves sinônimas acabou de acontecer
    }

    
    No* novo_no = (No*)malloc(sizeof(No));               // Aloca memória RAM para receber o novo Nó que guardará os dados
    if (!novo_no) return 0;
    
    
    strncpy(novo_no->id, id, sizeof(novo_no->id) - 1);   // Copia a string de forma segura limitando os caracteres para evitar estouro de bits 
    novo_no->id[sizeof(novo_no->id) - 1] = '\0';         // Garante o caractere nulo terminal
  
    
   
    novo_no->proximo = th->tabela[indice];             // INSERÇÃO NO INÍCIO: O novo nó aponta para quem já estava na lista,
    th->tabela[indice] = novo_no;                      // e o topo do vetor passa a apontar para o novo nó. Mantém o tempo fixo O(1).
    th->quantidade++; 

    return 1;                                          // Foi enserido na tabela
} 



int hash_buscar(TabelaHash* th, const char* id) {           //BUSCA DE ELEMENTOS (RF02)
    if (!th) return 0;                                      //Percorre a lista do índice mapeado procurando a string correspondente.

    
    unsigned int indice = hash_funcao(id, th->tamanho);     // Aplica a função hash para ir direto no índice exato onde o elemento deveria estar
    No* atual = th->tabela[indice];

     
    while (atual != NULL) {                                 // Percorre linearmente apenas a pequena lista encadeada daquele índice específico
        if (strcmp(atual->id, id) == 0) {                            
            return 1;                                       // Se retorna 1, foi encontrado com sucesso.
        }
        atual = atual->proximo;                             // Avança para o próximo nó conectado da lista Caso não encontre o ID escolhido
    }
    return 0;                                               // Se a lista chegou ao fim (NULL) e não achou nada, o usuário não existe
}


 
 double hash_obter_fator_carga(TabelaHash* th) {             //OBTENÇÃO DO FATOR DE CARGA
    if (!th || th->tamanho == 0) return 0.0;                //Calcula o nível de ocupação/estresse da tabela (Alpha = N / M)
    return (double)th->quantidade / th->tamanho;           
}



int hash_obter_colisoes(TabelaHash* th) {   //OBTENÇÃO DE COLISÕES
    if (!th) return 0;                      //Retorna o número acumulado de colisões para o relatório final
    return th->contador_colisoes;
}


void hash_liberar(TabelaHash* th) {                          //LIBERAÇÃO DE MEMÓRIA 
    if (!th) return;                                         //Limpa toda a alocação dinâmica do programa de dentro para fora para evitar memory leaks.

    
    for (int i = 0; i < th->tamanho; i++) {                  // Passa de índice em índice liberando os nós pendurados nas listas encadeadas
        No* atual = th->tabela[i];
        while (atual != NULL) {
            No* temporario = atual;
            atual = atual->proximo;
            free(temporario);                                // Desaloca o nó da lista
        }
    }
    
   
    free(th->tabela);                                         // Por fim, libera o vetor base e a estrutura de gerenciamento principal
    free(th);
}