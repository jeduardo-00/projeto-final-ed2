#include <stdio.h> 
#include <string.h>
#include <time.h>

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
    double tempoMed;
    

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


int main()
{
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
    char verifica_cadastro = 1; // TESTE
    printf("----------CADASTRO DE USUARIO----------\n");
    printf("Usuario: ");
    scanf("%s", usuario);

    printf("Dados enviado para a funcao: %s\n", usuario);

    // Funcao para envio
    if (verifica_cadastro == 1)
    {
        printf("Usuario cadastrado\n");
        relatorio.quantidade_elemento++;
    }
    
    // Funcao para tabela hash
    // Funcao para filtro de bloom
    relatorio.quantidade_elemento++;
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

    if (op == 1)
    {
        // FUNÇÃO DE VERIFICAR A TABELA

        // Medindo tempo
        clock_t inicio = clock();
        if (verifica_tabela == 1)
        {
            printf("Usuario cadastrado!\n");
        }
        else
        {
            printf("Usuario nao cadastrado!\n");
        }
        clock_t fim = clock();

        relatorio.tempoMed += (double)(fim-inicio)/CLOCKS_PER_SEC;
        relatorio.contadorTime ++;
    }
    else if (op == 2)
    {
        clock_t inicio = clock();
        // verificador = funcaodebloom
        if (verifica_bloom == 1)
        {
            printf("Usuario talvez exista. Iremos consultar tabela\n");
            // verifica_tabela = funcaochamandotabela
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
        relatorio.tempoMed += (double)(fim-inicio)/CLOCKS_PER_SEC;
        relatorio.contadorTime ++;
    }
    else
    {
        printf("Opcao invalida\n");
    }
    
    relatorio.busca++;
}

void estatistica()
{
    Estatistica zerado = {0};
    
    int taxa = 0;
    double mediaTime = 0;
    if (relatorio.busca != 0)
    {
        taxa = ((relatorio.falso_positivo*100)/relatorio.busca);
        mediaTime = (relatorio.tempoMed)/relatorio.contadorTime;
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
    }
    
    printf("---------------------------------------------------------\n");
}
void lote()
{
    char usuario[12];
    
    // Abrindo arquivo
    FILE *arquivo = fopen(".\\data\\usuarios_100000.txt", "r");
    if (arquivo == NULL)
    {
        printf("Erro ao abrir o arquivo!\n");
        return;
    }
    while (fscanf(arquivo, "%11s", usuario) == 1)
    {
        printf("Inserindo o usuario %s na tabela e no filtro\n", usuario);

        relatorio.quantidade_elemento++;
    }
    
    fclose(arquivo);
}