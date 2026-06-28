#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "bloom.h"

/*
    ARQUIVO DE IMPLEMENTAÇÃO (bloom.c)
    CONTÉM A LÓGICA MATEMATICA E TODA A MANIPULAÇÃO DE BITS DO FILTRO DE BLOOM
*/

//  Inicializa o Filtro de Bloom calculando as melhores dimensòes dinamicamente
filtroBloom *criar_bloom(int n_esperado, double prob_falso_positivo)
{
    filtroBloom *novo_filtro = (filtroBloom *)malloc(sizeof(filtroBloom));
    if (!novo_filtro) {
        return NULL;
    }

    //  1. Dimensionamento do Vetor (m): m = -(n * ln(p)) / (ln(2)^2)
    double numerador_m = -(n_esperado * log(prob_falso_positivo));
    double denominador_m = pow(log(2), 2);
    int m = (int)ceil(numerador_m / denominador_m); // ceil: garante espaço extra arredondando para cima

    //  2. Dimensionamento de Hashes (k): k = (m / n) * ln(2)
    int k = (int)round(((double)m / n_esperado) * log(2));

    novo_filtro->tam_vetor = m;
    novo_filtro->qtd_hashs = k;

    //  3. Alocação Física: Converte bits em bytes (1 byte = 8 bits)
    // O (+ 7) assegura o teto da divisão, evitando falta de memória para os últimos bits
    int tamanho_bytes = (m + 7) / 8;

    //  O calloc é OBRIGATÓRIO, pois garante que o "quadro" inicie totalmente zerado
    novo_filtro->vetor_bits = (uint8_t *)calloc(tamanho_bytes, sizeof(uint8_t));

    //  Tratamento de falha de alocação de memória (Evita memory leak)
    if (!novo_filtro->vetor_bits)
    {
        free(novo_filtro);
        return NULL;
    }

    //  Log de depuração (debug) para o relatório
    printf("[DEBUG] Bloom criado: n=%d, p=%.2f | m=%d bits (~%d bytes), k=%d hashes\n",
           n_esperado, prob_falso_positivo, m, tamanho_bytes, k);

    return novo_filtro;
}

//  Verifica a possível existência de um elemento (Pode gerar Falso Positivo)
int consultar_bloom(filtroBloom *filtro, const char *usuario)
{
    //  Testa as k posições. Se APENAS UMA for 0, o elemento definitivamente não existe.
    for (int i = 0; i < filtro->qtd_hashs; i++)
    {
        uint32_t hash_gigante = hash_djb2_semeada(usuario, i); // 'i' atua como semente
        int posicao_no_vetor = hash_gigante % filtro->tam_vetor;
        
        if (testar_bit(filtro, posicao_no_vetor) == 0)
        {
            return 0; //    Certeza absoluta que NÃO está no conjunto
        }
    }
    
    return 1; //    POSSIVELMENTE está no conjunto
}

//  Insere um novo elemento no filtro, ligando 'k' bits correspondentes
void inserir_bloom(filtroBloom *filtro, const char *usuario)
{
    //  Roda o algoritmo hash 'k' vezes (usando sementes diferentes)
    for (int i = 0; i < filtro->qtd_hashs; i++)
    {
        uint32_t hash_gigante = hash_djb2_semeada(usuario, i);
        int posicao_no_vetor = hash_gigante % filtro->tam_vetor; // Método da divisão para caber no vetor
        
        setar_bit(filtro, posicao_no_vetor);
    }
}

//  Desaloca de forma segura a memória do filtro
void liberar_bloom(filtroBloom *filtro)
{
    if (filtro)
    {
        //  Liberta o caminhão de bytes primeiro...
        if (filtro->vetor_bits)
        {
            free(filtro->vetor_bits);
        }
        //  ... e depois liberta a struct principal.
        free(filtro);
    }
}

//  Escovação de Bits: Liga (Muda para 1) um bit específico no vetor de bytes
void setar_bit(filtroBloom *filtro, int posicao)
{
    int indice_byte = posicao / 8;       // Encontra a "gaveta" (Byte)
    int deslocamento_bit = posicao % 8;  // Encontra a posição exata (0 a 7) dentro do Byte

    //  Operador OR (|): Liga o bit desejado (máscara) apagar os vizinhos que já estavam ligados
    filtro->vetor_bits[indice_byte] |= (1 << deslocamento_bit);
}

//  Escovação de Bits: Lê o valor (0 ou 1) de um bit específico
int testar_bit(filtroBloom *filtro, int posicao)
{
    int indice_byte = posicao / 8;
    int deslocamento_bit = posicao % 8;

    //  Operador AND (&): Funciona como um "Raio-X", isolando e retornando apenas o bit da máscara
    return (filtro->vetor_bits[indice_byte] & (1 << deslocamento_bit)) != 0;
}

//  Algoritmo DJB2 (Modificado com Semente para simular múltiplas funções hash)
uint32_t hash_djb2_semeada(const char *str, int semente) {
    unsigned long hash = 5381;
    hash += semente; //     Efeito Avalanche: Muda o ponto de partida gerando um caminho numérico único
    int caractere;

    while ((caractere = *str++)) {
        //  (hash << 5) + hash é uma forma super otimizada a nível de hardware de fazer (hash * 33)
        hash = ((hash << 5) + hash) + caractere;
    }
    return (uint32_t)hash;
}