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
    LOTE
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
void menuBusca();

// Variáveis globais
Estatistica relatorio = {0};
TabelaHash *hash;
filtroBloom *bloom;

// Constantes

// Tamanho da tabela calculada de acordo o fator de carga
#define TAM 1429 // 1000
// #define TAM 114293 // 10000
// #define TAM 142867 // 100000
int main()
{
    // Criando a tabela
    hash = hash_criar(TAM);
    // bloom = criar_bloom(TAM);

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
                printf("LOTES DADOS\n");
                lote();
                break;
            case SAIR:
                printf("ENCERRANDO PROGRAMA\n");
                break;
            
            default:
                printf("INFORMAÇÃO INVALIDA\n");
                break;
        }

    } while (op != SAIR);
    return 0;
    
}


void menu()
{
    printf("------------MENU------------\n");
    printf("1 - INSERIR USUARIO\n2 - CONSULTAR USUARIO\n3 - ESTATISTICA\n4 - INSERIR USUARIOS EM LOTE\n0 - SAIR\n");
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
    int verifica_bloom = 1; // TESTE
    printf("----------CADASTRO DE USUARIO----------\n");
    printf("Usuario: ");
    scanf("%s", usuario);

    printf("Dados enviado para a funcao: %s\n", usuario);
    
    verifica_tabela = hash_inserir(hash,usuario);
    // Funcao para filtro de bloom

    // Funcao para envio 1
    if (verifica_tabela == 1 && verifica_bloom == 1)
    {
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
    int verifica_bloom = 1; // TESTE
    int verifica_tabela = 0; // TESTE
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
        // verificador = funcaodebloom
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
    
    int taxa = 0;
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
        printf("- Taxa percentual de falso positivo: %d %% \n", taxa);
        printf("- Tempo medio de consulta: %f\n", mediaTime);
        printf("- Numero de colisoes: %d\n", hash->contador_colisoes);
        
    }
    
    printf("---------------------------------------------------------\n");
}
void lote()
{
    char usuario[12];
    int verifica_tabela = 0; // TESTE
    int verifica_bloom = 1; // TESTE
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
        if (verifica_tabela == 1 && verifica_bloom == 1)
        {
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