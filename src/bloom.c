#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "bloom.h"

/*
    ARQUIVO DE IMPLEMENTAÇÃO (bloom.c)
    CONTÉM A LÓGICA E TODA A MANIPULAÇÃO DE BITS DO FILTRO DE BLOOM
*/

// Cria o Filtro de Bloom recebendo diretamente o tamanho do vetor em bits (m)
filtroBloom *criar_bloom(int m)
{
    filtroBloom *novo_filtro = (filtroBloom *)malloc(sizeof(filtroBloom));
    if (!novo_filtro) {
        return NULL;
    }

    novo_filtro->tam_vetor = m;

    // Converte bits em bytes ? o (+7) garante arredondamento para cima
    int tamanho_bytes = (m + 7) / 8;

    novo_filtro->vetor_bits = (uint8_t *)calloc(tamanho_bytes, sizeof(uint8_t));
    if (!novo_filtro->vetor_bits)
    {
        free(novo_filtro);
        return NULL;
    }

    printf("[DEBUG] Bloom criado: m=%d bits (~%d bytes), k=%d hashes\n",
           m, tamanho_bytes, QTD_SEMENTES);

    return novo_filtro;
}

// Insere um elemento no filtro ligando QTD_SEMENTES bits no vetor
void inserir_bloom(filtroBloom *filtro, const char *usuario)
{
    // Para cada semente fixa, calcula uma posição diferente e liga o bit
    for (int i = 0; i < QTD_SEMENTES; i++)
    {
        uint32_t hash_gigante = hash_djb2_semeada(usuario, SEMENTES[i]);
        int posicao_no_vetor = hash_gigante % filtro->tam_vetor;

        setar_bit(filtro, posicao_no_vetor);
    }
}

// Consulta se um elemento possivelmente existe no filtro
int consultar_bloom(filtroBloom *filtro, const char *usuario)
{
    // Se QUALQUER um dos bits estiver em 0, o elemento certamente não existe
    for (int i = 0; i < QTD_SEMENTES; i++)
    {
        uint32_t hash_gigante = hash_djb2_semeada(usuario, SEMENTES[i]);
        int posicao_no_vetor = hash_gigante % filtro->tam_vetor;

        if (testar_bit(filtro, posicao_no_vetor) == 0)
        {
            return 0; // Certeza absoluta que NÃO está no conjunto
        }
    }

    return 1; // POSSIVELMENTE está no conjunto
}

// Desaloca a memória do filtro de forma segura
void liberar_bloom(filtroBloom *filtro)
{
    if (filtro)
    {
        // Libera o vetor de bits primeiro...
        if (filtro->vetor_bits)
        {
            free(filtro->vetor_bits);
        }
        // ...depois libera a struct principal
        free(filtro);
    }
}

// Liga (muda para 1) um bit específico no vetor de bytes
void setar_bit(filtroBloom *filtro, int posicao)
{
    int indice_byte = posicao / 8;       // Encontra o byte certo
    int deslocamento_bit = posicao % 8;  // Encontra a posição dentro do byte (0 a 7)

    // Operador OR (|): liga o bit desejado sem apagar os vizinhos
    filtro->vetor_bits[indice_byte] |= (1 << deslocamento_bit);
}

// Lê o valor (0 ou 1) de um bit específico no vetor
int testar_bit(filtroBloom *filtro, int posicao)
{
    int indice_byte = posicao / 8;
    int deslocamento_bit = posicao % 8;

    // Operador AND (&): isola e retorna apenas o bit da posição desejada
    return (filtro->vetor_bits[indice_byte] & (1 << deslocamento_bit)) != 0;
}

// Algoritmo DJB2 modificado com semente para simular múltiplas funções hash
uint32_t hash_djb2_semeada(const char *str, int semente)
{
    unsigned long hash = 5381;
    hash += semente; // Efeito avalanche: ponto de partida único por semente
    int caractere;

    while ((caractere = *str++))
    {
        // (hash << 5) + hash é equivalente a hash * 33, otimizado em hardware
        hash = ((hash << 5) + hash) + caractere;
    }
    return (uint32_t)hash;
}