#ifndef BLOOM_H
#define BLOOM_H

#include <stdint.h>

/*
    ARQUIVO DE CABEÇALHO (bloom.h)
    O FILTRO E PROTÓTIPOS DAS FUNÇÕES.
*/

// Estrutura Principal do Filtro de Bloom.
typedef struct filtrobloom {
    uint8_t *vetor_bits;  // Vetor de Bits ( com 1 Byte)
    int tam_vetor;        // Tamanho do Vetor de Bits ( Variável 'm')
    int qtd_hashs;        // Quantidade de Hashs ( Variável 'k')
} filtroBloom;

// Inicializa o filtro calculando m e k dinamicamente
filtroBloom *criar_bloom(int n_esperado, double prob_falso_positivo); 

// Operações principais do Filtro
void inserir_bloom(filtroBloom *filtro, const char *usuario);
int consultar_bloom(filtroBloom *filtro, const char *usuario);
void liberar_bloom(filtroBloom *filtro);

// Operações bit a bit (Bitwise) internas
void setar_bit(filtroBloom *filtro, int posicao);
int testar_bit(filtroBloom *filtro, int posicao);

// Função de Hash otimizada (Algoritmo DJB2 Semeado)
uint32_t hash_djb2_semeada(const char *str, int semente);


#endif