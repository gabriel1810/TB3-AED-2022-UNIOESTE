#include "interface.h"
#include "arvore.h"
#include "produto.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int menu()
{
    int index;
    printf("|  1 . Carregar arquivo txt\n");
    printf("|  2 . Inserir produto\n");
    printf("|  3 . Atualizar preco\n");
    printf("|  4 . Atualizar estoque\n");
    printf("|  5 . Remover produto\n");
    printf("|  6 . Buscar dados do produto \n");
    printf("|  7 . Imprimir arvore binaria por nivel\n");
    printf("|  8 . Imprimir tudo\n");
    printf("|  9 . Imprimir livres no arquivo de indice\n");
    printf("| 10 . Imprimir livres no arquivo de dados\n");
    printf("|  0 . Sair\n");

    scanf("%d%*c", &index);
    return index;
}

void menu_ler_arquivo(FILE *arqIndex, FILE *arqData)
{
    char nome_arq[50];

    printf("Digite o nome do arquivo.\n");
    scanf("%[^\n]%*c", nome_arq);
    if (strstr(nome_arq, ".txt") == NULL)
    {
        strcat(nome_arq, ".txt");
    }
    le_arquivo_txt(nome_arq, arqIndex, arqData);
}

void menu_inserir(FILE *arqIndex, FILE *arqData)
{
    Produto p;
    printf("Entre com os dados do produto:\n");
    printf("Codigo: ");
    scanf("%d", &p.codigo);
    fflush(stdin);

    printf("Nome: ");
    scanf("%[^\n]%*c", p.nome);

    printf("Marca: ");
    scanf("%[^\n]%*c", p.marca);

    printf("Categoria: ");
    scanf("%[^\n]%*c", p.categoria);

    printf("Estoque: ");
    scanf("%d", &p.estoque);
    fflush(stdin);

    printf("Preco: ");
    scanf("%f%*c", &p.preco);
    fflush(stdin);

    inserir(arqIndex, arqData, p);
}

void menu_alterar_preco(FILE *arqIndex, FILE *arqData)
{
    int codigo, posDado;
    float preco;
    printf("Digite o codigo do produto: ");
    scanf("%d%*c", &codigo);
    posDado = busca_dado(arqIndex, codigo);
    if (posDado != -1)
    {
        printf("Digite o novo preco: ");
        scanf("%f%*c", &preco);
        alterar_preco(arqData, posDado, preco);
        printf("\nPreco atualizado!\n");
    }
    else
        printf("Codigo invalido!\n");
}

void menu_alterar_estoque(FILE *arqIndex, FILE *arqData)
{
    int codigo, posDado;
    float estoque;
    printf("Digite o odigo do produto: ");
    scanf("%d%*c", &codigo);
    posDado = busca_dado(arqIndex, codigo);
    if (posDado != -1)
    {
        printf("Digite o novo estoque: ");
        scanf("%f%*c", &estoque);
        alterar_estoque(arqData, posDado, estoque);
        printf("\nEstoque atualizado!\n");
    }
    else
        printf("Codigo invalido!\n");
}

void menu_remover(FILE *arqIndex, FILE *arqData)
{
    int codigo;
    printf("Codigo do Produto que deseja remover: ");
    scanf("%d%*c", &codigo);
    remover(arqIndex, arqData, codigo);
}

void menu_imprimir_item(FILE *arqIndex, FILE *arqData)
{
    int codigo, posDado;
    printf("Digite o codigo do Produto: ");
    scanf("%d%*c", &codigo);
    posDado = busca_dado(arqIndex, codigo);
    if (posDado != -1)
    {
        imprimir_item(arqData, posDado);
    }
    else
        printf("Codigo invalido!\n");
}

void imprimir_arvore(FILE *arqIndex)
{
    Arvore raiz;
    Cab_index cab = le_cab_index(arqIndex);
    if (cab.raiz != -1)
    {
        raiz = ler_no(arqIndex, cab.raiz);
        imprime_por_nivel(arqIndex, raiz);
    }
    else
        printf("A arvore esta vazia!\n");
}

void imprimir_tudo(FILE *arqIndex, FILE *arqData)
{
    Cab_index cab = le_cab_index(arqIndex);
    if (cab.raiz != -1)
    {
        Arvore raiz = ler_no(arqIndex, cab.raiz);
        int i;

        if (eh_folha(raiz))
            for (i = 0; i < raiz.numChaves; i++)
                imprimir_item(arqData, raiz.dados[i]);
        else
        {
            for (i = 0; i < raiz.numChaves; i++)
            {
                imprime_in_ordem(arqIndex, arqData, ler_no(arqIndex, raiz.filho[i]));
                imprimir_item(arqData, raiz.dados[i]);
            }
            imprime_in_ordem(arqIndex, arqData, ler_no(arqIndex, raiz.filho[i]));
        }
    }
}

void imprimir_livres_arvore(FILE *arqIndex)
{
    Cab_index cab = le_cab_index(arqIndex);
    Arvore vazio = ler_no(arqIndex, cab.livre);
    printf("Posicao livre = %d\n", cab.livre);
    if (cab.livre != -1)
        while (vazio.chave[0] != -1)
        {
            printf("Posicao livre = %d\n", vazio.chave[0]);
            vazio = ler_no(arqIndex, vazio.chave[0]);
        }
}

void imprimir_livres_dados(FILE *arqData)
{
    Cab_dados cab = le_cab_dados(arqData);
    Produto vazio = ler_produto(arqData, cab.livre);
    printf("Posicao livre = %d\n", cab.livre);
    if (cab.livre != -1)
        while (vazio.codigo != -1)
        {
            printf("Posicao livre = %d\n", vazio.codigo);
            vazio = ler_produto(arqData, vazio.codigo);
        }
}

void imprimir_item(FILE *arqData, int posDado)
{
    Produto p = ler_produto(arqData, posDado);
    printf("\n========== cod: %d ==============\n", p.codigo);
    printf("Nome: %s | Marca: %s\n", p.nome, p.marca);
    printf("Categoria: %s\n", p.categoria);
    printf("Estoque: %d | Preco R$: %.2f\n", p.estoque, p.preco);
}
