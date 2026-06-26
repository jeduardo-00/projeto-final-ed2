#include <stdio.h> 

// Estruturas
enum Opcoes
{
    SAIR,
    INSERIR,
    CONSULTAR,
    ESTATISTICA,
    LOTE
};

// Funções
void menu();
void inserir();
void consultar();
void lote();

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

void inserir()
{
    char usuario[11];
    printf("----------CADASTRO DE USUARIO----------\n");
    printf("Usuario: ");
    scanf("%s", usuario);

    printf("Dados enviado para a funcao: %s\n", usuario);

    // Funcao para tabela hash
    // Funcao para filtro de bloom
}
void consultar()
{
    char usuario[11];
    printf("----------CONSULTA POR USUARIO----------\n");
    printf("Usuario: ");
    scanf("%s", usuario);

    printf("Dados enviado para a funcao: %s\n", usuario);

    // Funcao para tabela hash
    // Funcao para filtro de bloom
}

void lote()
{
    char usuario[11];
    printf("Insira o nome do arquivo\n");
    printf("Extraindo dados do arquivo\n");
    printf("Acessando cada funcao ao obter os dados\n");
}