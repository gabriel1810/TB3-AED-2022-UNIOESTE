#include "produto.h"

Fila *criar_fila()
{
    Fila *aux = (Fila *)malloc(sizeof(Fila));
    aux->inicio = aux->fim = NULL;
    return aux;
}

void enqueue(Fila *fila, Arvore item)
{
    struct noFila *aux = (struct noFila *)malloc(sizeof(struct noFila));
    aux->item = item;
    aux->prox = NULL;
    if (fila->inicio == NULL)
        fila->inicio = fila->fim = aux;
    else
    {
        fila->fim->prox = aux;
        fila->fim = aux;
    }
}

struct noFila *dequeue(Fila *fila)
{
    struct noFila *aux;
    aux = fila->inicio;
    if (fila->inicio != NULL)
        fila->inicio = fila->inicio->prox;
    return aux;
}

FILE *abrir_arquivo_dados()
{
    FILE *dados = fopen("Dados.bin", "rb+");
    if (dados == NULL)
    {
        dados = fopen("Dados.bin", "wb+");
        cria_cab_dados(dados);
    }
    return dados;
}

Produto ler_produto(FILE *dados, int pos)
{
    Produto p;
    fseek(dados, sizeof(Cab_dados) + pos * sizeof(Produto), SEEK_SET);
    fread(&p, sizeof(Produto), 1, dados);
    return p;
}

void escreve_produto(FILE *dados, Produto p, int pos)
{
    fseek(dados, sizeof(Cab_dados), SEEK_SET);
    fseek(dados, pos * sizeof(Produto), SEEK_CUR);
    fwrite(&p, sizeof(Produto), 1, dados);
}

void inserir_arquivo_dados(FILE *dados, Produto p)
{
    Cab_dados cab = le_cab_dados(dados);

    if (cab.livre == -1)
    {
        escreve_produto(dados, p, cab.topo);
        cab.topo++;
    }
    else
    {
        Produto vazio = ler_produto(dados, cab.livre);
        escreve_produto(dados, p, cab.livre);
        cab.livre = vazio.codigo;
    }
    escreve_cab_dados(dados, cab);
}

void escreve_cab_dados(FILE *arqData, Cab_dados cab)
{
    fseek(arqData, 0, SEEK_SET);
    fwrite(&cab, sizeof(Cab_dados), 1, arqData);
}

void inserir(FILE *index, FILE *dados, Produto p)
{
    Cab_dados cabDados = le_cab_dados(dados);
    Cab_index cabIndex = le_cab_index(index);

    if (cabIndex.raiz == -1 || busca_dado(index, p.codigo) == -1)
    {
        if (cabDados.livre == -1)
            inserir_arvore(index, p.codigo, cabDados.topo);
        else
            inserir_arvore(index, p.codigo, cabDados.livre);
        inserir_arquivo_dados(dados, p);
    }
}

void alterar_estoque(FILE *dados, int posDado, int estoque)
{
    Produto p = ler_produto(dados, posDado);
    p.estoque = estoque;
    escreve_produto(dados, p, posDado);
}

void alterar_preco(FILE *dados, int posDado, float preco)
{
    Produto p = ler_produto(dados, posDado);
    p.preco = preco;
    escreve_produto(dados, p, posDado);
}

void alterar(FILE *index, FILE *dados, int codigo, int estoque, float preco)
{
    int posDado = busca_dado(index, codigo);
    if (posDado != -1)
    {
        if (estoque != -1)
            alterar_estoque(dados, posDado, estoque);
        if (preco != -1)
            alterar_preco(dados, posDado, preco);
    }
    else
        printf("Codigo invalido!!\n");
}
