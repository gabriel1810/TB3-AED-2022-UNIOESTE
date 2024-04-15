#include "arvore.h"
#include "produto.h"
#include "interface.h"


int main()
{

    FILE *arqIndex = abrir_arquivo_index();

    FILE *arqDados = abrir_arquivo_dados();

    int index;
    do
    {
        system("cls");
        index = menu();
        switch (index)
        {
        case 1:
            menu_ler_arquivo(arqIndex, arqDados);
            break;
        case 2:
            menu_inserir(arqIndex, arqDados);
            break;

        case 3:
            menu_alterar_preco(arqIndex, arqDados);
            break;
        case 4:
            menu_alterar_estoque(arqIndex, arqDados);
            break;

        case 5:
            menu_remover(arqIndex, arqDados);
            break;

        case 6:
            menu_imprimir_item(arqIndex, arqDados);
            break;

        case 7:
            imprimir_arvore(arqIndex);

            break;
        case 8:
            imprimir_tudo(arqIndex, arqDados);
            break;

        case 9:
            imprimir_livres_arvore(arqIndex);
            break;
        case 10:
            imprimir_livres_dados(arqDados);
            break;

        case 0:
            exit(1);
            break;
        default:
            printf("\nOpção Invalida.\n");
            break;
        }
        printf("Pressione qualquer tecla para continuar...");
        getchar();
    } while (index);

    fclose(arqIndex);
    fclose(arqDados);
    return 0;
}
