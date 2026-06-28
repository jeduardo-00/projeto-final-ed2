#ifndef BLOOM_H
#define BLOOM_H

#include <stdint.h>

/*
    ARQUIVO DE CABEÇALHO (bloom.h)
    CONTÉM A DEFINIÇÃO DA STRUCT E OS PROTÓTIPOS DAS FUNÇÕES DO FILTRO DE BLOOM
*/

// Quantidade fixa de funções hash (sementes)
#define QTD_SEMENTES 3

// Sementes fixas para a função hash DJB2 ? números primos para melhor dispersão
static const int SEMENTES[QTD_SEMENTES] = {1, 31, 131};

// Estrutura do Filtro de Bloom
typedef struct filtrobloom {
    uint8_t *vetor_bits;  // Vetor de bytes que representa o vetor de bits
    int tam_vetor;        // Tamanho do vetor em bits (m)
} filtroBloom;

// Protótipos
filtroBloom *criar_bloom(int m);
void inserir_bloom(filtroBloom *filtro, const char *usuario);
int consultar_bloom(filtroBloom *filtro, const char *usuario);
void liberar_bloom(filtroBloom *filtro);
void setar_bit(filtroBloom *filtro, int posicao);
int testar_bit(filtroBloom *filtro, int posicao);
uint32_t hash_djb2_semeada(const char *str, int semente);

#endif