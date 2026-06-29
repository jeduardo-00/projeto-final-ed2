#include <stdio.h> 
#include <string.h>
#include <time.h>
#include "hash.h"       
#include "bloom.h"       

// Estruturas
enum Opcoes
{
    SAIR,
    INSERIR,
    CONSULTAR,
    ESTATISTICA,
    LOTE,
    BUSCALOTE,
};

typedef struct
{
    int quantidade_elemento;
    int busca;
    int busca_evitada;
    int falso_positivo;
    int contadorTime;
    int contadorBloom;
    double tempoTotal;
    
}Estatistica;


// Funções
void menu();
void inserir();
void consultar();
void estatistica();
void lote();
void loteConsulta();
void menuBusca();

// Variáveis globais
Estatistica relatorio = {0};
TabelaHash *hash;
filtroBloom *bloom;

// Constantes

// FATOR DE CARGA
#define FATORCARGA 0.7

// Tamanho da tabela calculada de acordo o fator de carga
#define TAM 1000 // 1000
// #define TAM 10000 // 10000
// #define TAM 100000 // 100000

int main()
{
    // Criando a tabela
    hash = hash_criar((int)TAM/FATORCARGA);
    bloom = criar_bloom(TAM);

    enum Opcoes op ;
    do
    {
        menu();
        scanf("%d", &op);
        switch (op)
        {
            case INSERIR:
                printf("INSERIR DADOS\n");
                inserir();
                break;
            case CONSULTAR:
                printf("CONSULTAR DADOS\n");
                consultar();
                break;
            case ESTATISTICA:
                printf("ESTATISTICA DE DADOS\n");
                estatistica();
                break;
            case LOTE:
                printf("LOTES DE DADOS\n");
                lote();
                break;
            case BUSCALOTE:
                printf("BUSCA EM LOTES DE DADOS\n");
                loteConsulta();
                break;
            case SAIR:
                printf("ENCERRANDO PROGRAMA\n");
                break;
            
            default:
                printf("INFORMAÇÃO INVALIDA\n");
                break;
        }

    } while (op != SAIR);

    hash_liberar(hash);
    liberar_bloom(bloom);

    return 0;
    
}


void menu()
{
    printf("------------MENU------------\n");
    printf("1 - INSERIR USUARIO\n2 - CONSULTAR USUARIO\n3 - ESTATISTICA\n4 - INSERIR USUARIOS EM LOTE\n5 - CONSULTAR EM LOTE\n0 - SAIR\n");
    printf("----------------------------\n");
    printf("SELECIONE: ");
}
void menuBusca()
{
    printf("------------MENU DE BUSCA------------\n");
    printf("1 - SEM FILTRO DE BLOOM\n2 - COM FILTRO DE BLOOM\n0 - SAIR\n");
    printf("-------------------------------------\n");
    printf("SELECIONE: ");
}

void inserir()
{
    char usuario[12];
    int verifica_tabela = 0; // TESTE
    printf("----------CADASTRO DE USUARIO----------\n");
    printf("Usuario: ");
    scanf("%s", usuario);

    printf("Dados enviado para a funcao: %s\n", usuario);
    
    verifica_tabela = hash_inserir(hash,usuario);

    // Funcao para envio 1
    if (verifica_tabela == 1)
    {
        inserir_bloom(bloom,usuario);
        printf("Usuario cadastrado\n");
        relatorio.quantidade_elemento++;
    }
    else
    {
        printf("Usuario nao foi cadastrado\n");
    }
    
}
void consultar()
{
    char usuario[12];
    int verifica_bloom = 0;
    int verifica_tabela = 0; 
    int op;

    printf("----------CONSULTA POR USUARIO----------\n");
    printf("Usuario: ");
    scanf("%s", usuario);
    menuBusca();
    scanf("%d",&op);

    // Sem filtro
    if (op == 1)
    {
        // Medindo tempo
        clock_t inicio = clock();
        verifica_tabela = hash_buscar(hash,usuario); // FUNÇÃO DE VERIFICAR A TABELA
        clock_t fim = clock();

        if (verifica_tabela == 1)
        {
            printf("Usuario cadastrado!\n");
        }
        else
        {
            printf("Usuario nao cadastrado!\n");
        }
        
        relatorio.tempoTotal += (double)(fim-inicio)/CLOCKS_PER_SEC;
        relatorio.contadorTime ++;
        relatorio.busca++;
    }
    // Com filtro
    else if (op == 2)
    {
        clock_t inicio = clock();
        verifica_bloom = consultar_bloom(bloom,usuario);
        if (verifica_bloom == 1)
        {
            printf("Usuario talvez exista. Iremos consultar tabela\n");
            verifica_tabela = hash_buscar(hash,usuario); // FUNÇÃO DE VERIFICAR A TABELA

            if (verifica_tabela == 1)
            {
                printf("Usuario cadastrado!\n");
            }
            else
            {
                printf("Usuario nao cadastrado!\n");
                printf("Falso Positivo!\n");
                relatorio.falso_positivo++;
            }
            
        }
        else
        {
            printf("Usuario nao cadastrado!\n");
            relatorio.busca_evitada++;
    
        }
        clock_t fim = clock();
        relatorio.tempoTotal += (double)(fim-inicio)/CLOCKS_PER_SEC;
        relatorio.contadorTime ++;
        relatorio.busca++;
        relatorio.contadorBloom++;
    }
    else
    {
        printf("Opcao invalida\n");
    }
    
}

void estatistica()
{
    
    double taxa = 0;
    double mediaTime = 0;
    if (relatorio.busca != 0)
    {
        mediaTime = (relatorio.tempoTotal)/relatorio.contadorTime;
    }
    if (relatorio.falso_positivo != 0 && relatorio.contadorBloom != 0)
    {
        taxa = ((relatorio.falso_positivo*100)/relatorio.contadorBloom);
    }
    
    
    printf("-------------------RELATORIO DOS DADOS-------------------\n");
    if (relatorio.quantidade_elemento == 0 && relatorio.busca == 0)
    {
        printf("RELATORIO VAZIO\n");
    }
    else
    {
        printf("- Quantidade de elementos: %d\n", relatorio.quantidade_elemento);
        printf("- Quantidade de consultas: %d\n", relatorio.busca);
        printf("- Consultas evitadas: %d\n", relatorio.busca_evitada);
        printf("- Numero de falso positivo: %d\n", relatorio.falso_positivo);
        printf("- Taxa percentual de falso positivo: %.2f %% \n", taxa);
        printf("- Tempo medio de consulta: %.15f\n", mediaTime);
        printf("- Numero de colisoes: %d\n", hash->contador_colisoes);
        
    }
    
    printf("---------------------------------------------------------\n");
}
void lote()
{
    char usuario[12];
    int verifica_tabela = 0; // TESTE
    // Abrindo arquivo
    FILE *arquivo = fopen(".\\data\\usuarios_1000.txt", "r");
    if (arquivo == NULL)
    {
        printf("Erro ao abrir o arquivo!\n");
        return;
    }
    while (fscanf(arquivo, "%11s", usuario) == 1)
    {
        printf("Inserindo o usuario %s na tabela e no filtro\n", usuario);

        verifica_tabela = hash_inserir(hash,usuario); // Função para inserir na hash
        // Função para inserir no filtro
        if (verifica_tabela == 1)
        {
            inserir_bloom(bloom,usuario);
            printf("Usuario cadastrado\n");
            relatorio.quantidade_elemento++;
        }
        else
        {
            printf("Usuario nao foi cadastrado\n");
        }
    }
    
    fclose(arquivo);
}
void loteConsulta()
{
    char usuario[12];
    int verifica_bloom = 0;
    int verifica_tabela = 0; 
    int op = 0;

    // Melhoria para a Parte 3 (Experimentos): Escolher o arquivo dinamicamente

    FILE *arquivo = fopen(".\\data\\consulta_1000.txt", "r");
    if (arquivo == NULL)
    {
        printf("Erro ao abrir o arquivo!\n");
        return;
    }

    // 1. PERGUNTA SE QUER COM OU SEM FILTRO ANTES DO LOOP
    menuBusca();
    scanf("%d", &op);

    printf("Iniciando consulta em lote. Aguarde...\n");

    // 2. RAMIFICA A LÓGICA ANTES DE ENTRAR NO LAÇO (Evita overhead de CPU)
    if (op == 1) 
    {
        // LAÇO 1: SEM FILTRO (Busca Direta na Hash)
        while (fscanf(arquivo, "%11s", usuario) == 1)
        {
            clock_t inicio = clock();
            verifica_tabela = hash_buscar(hash, usuario); 
            clock_t fim = clock();

            // Atualiza métricas parciais (sem Bloom)
            relatorio.tempoTotal += (double)(fim - inicio) / CLOCKS_PER_SEC;
            relatorio.contadorTime++;
            relatorio.busca++;
        }
    } 
    else if (op == 2) 
    {
        // LAÇO 2: COM FILTRO (Bloom -> Hash)
        while (fscanf(arquivo, "%11s", usuario) == 1)
        {
            clock_t inicio = clock();
            
            verifica_bloom = consultar_bloom(bloom, usuario);
            
            if (verifica_bloom == 1)
            {
                verifica_tabela = hash_buscar(hash, usuario); 
                if (verifica_tabela != 1)
                {
                    relatorio.falso_positivo++;
                }
            }
            else
            {
                relatorio.busca_evitada++;
            }
            
            clock_t fim = clock();
            
            // Atualiza métricas completas
            relatorio.tempoTotal += (double)(fim - inicio) / CLOCKS_PER_SEC;
            relatorio.contadorTime++;
            relatorio.busca++;
            relatorio.contadorBloom++;
        }
    } 
    else 
    {
        printf("Opcao invalida. Abortando consulta.\n");
        fclose(arquivo);
        return;
    }

    printf("Consulta em lote finalizada com sucesso!\n");
    fclose(arquivo);
}