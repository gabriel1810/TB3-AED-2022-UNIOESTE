#ifndef PRODUTO_H
#define PRODUTO_H

#include "arvore.h"

typedef struct{
    int codigo;         // Codigo do produto
    char nome[50];      // Nome do produto
    char marca[30];     // Marca do produto
    char categoria[50]; // Categorias do produto
    int estoque;        // Quantidade em estoque
    float preco;        // Preco do produto
} Produto;

struct noFila{
    Arvore item;         // Os itens da fila são nós da arvore
    struct noFila *prox; // Aponta para o proximo da fila
};


typedef struct{
    struct noFila *inicio; // Aponta para o primeiro no da fila
    struct noFila *fim;    // Aponta para o ultimo no da fila
} Fila;


/*
 * @brief Cria o cabeçalho para o arquivo de dados
 * @param arquivo a ser criado
 * @pre condicao O arquivo deve estar aberto
 * @post O cabecalho é escrito
 */
void cria_cab_dados(FILE *dados);
/*
 * @brief Atualiza o cabeçalho de dados
 * @param dados Arquivo de dados
 * @param cab Cabecalho do arquivo de dados
 * @pre O arquivo deve deve estar aberto
 * @post O cabeçalho é atualizado
 */
void escreve_cab_dados(FILE *dados, Cab_dados cab);
/*
 * @brief Faz a remoçao de um produto indicado por posDado do arquivo de dados
 * @param dados Arquivo binario contendo os dados dos produtos
 * @param posDado Posição do dado que vai ser removido
 * @pre O arquivo deve deve estar aberto e a posição valida
 * @post O produto é removido do arquivo de dados
 */
void remover_arquivo_dados(FILE *dados, int posDado);
/*
 * @brief Enfileira um nó
 * @param fila Fila de nós
 * @param item nó da arvore B
 * @pre condicao Fila tem que ser valida
 * @post o nó é enfileirado na fila
 */
void enqueue(Fila *fila, Arvore item);
/*
 * @brief Escreve o Produto no arquivo de dados
 * @param dados Arquivo binario contendo os dados dos produtos
 * @param p Produto
 * @param pos posição no arquivo binario
 * @pre O arquivo deve esta aberto e a posição valida
 * @post O produto esta armazenado no arquivo binario
 */
void escreve_produto(FILE *dados, Produto p, int pos);
/*
 * @brief Insere o Produto p no arquivo de dados
 * @param dados Arquivo binario contendo os dados dos produtos
 * @param p Produto
 * @pre O arquivo deve deve estar aberto
 * @post O produto é inserido no arquivo de dados
 */
void inserir_arquivo_dados(FILE *dados, Produto p);
/*
 * @brief Função de inserção do produto no arquivo de dados e no arquivo de index
 * @param index Arquivo binario contendo os index da arvore
 * @param dados Arquivo binario contendo os dados dos produtos
 * @param p Produto
 * @pre  Os arquivos devem estar abertos
 * @post O produto é inserido em ambos os arquivos
 */
void inserir(FILE *index, FILE *dados, Produto p);
/*
 * @brief Altera o estoque de um produto
 * @param dados Arquivo binario contendo os dados dos produtos
 * @param posDado Posição no arquivo de dados do produto que vai ser alterado
 * @param estoque Novo valor do estoque
 * @pre  O arquivo deve deve estar aberto e a posição valida
 * @post A quantidade do produto é alterada
 */
void alterar_estoque(FILE *dados, int posDado, int estoque);
/*
 * @brief Altera o preco de um produto
 * @param dados Arquivo binario contendo os dados dos produtos
 * @param posDado Posição no arquivo de dados do produto que vai ser alterado
 * @param preco Novo valor do preco
 * @pre  O arquivo deve deve estar aberto e a posição valida
 * @post O preco do produto é alterada
 */
void alterar_preco(FILE *dados, int posDado, float preco);
/*
 * @brief Altera o preco e quantidade de um produto
 * @param dados Arquivo binario contendo os dados dos produtos
 * @param codigo Posição no arquivo de dados do produto que vai ser alterado
 * @param estoque Novo valor do estoque
 * @param preco Novo valor do preco
 * @pre  O arquivo deve deve estar aberto e a posição valida
 * @post O preco e a quantidade do produto é alterada
 */
void alterar(FILE *index, FILE *dados, int codigo, int estoque, float preco);
/*
 * @brief desinfilera um nó
 * @param fila Fila de nós
 * @returns um nó da fila
 * @pre condicao Fila tem que ser valida
 * @post nó é desinfileirado
 */
struct noFila* dequeue(Fila *fila);
/*
 * @brief Aloca memoria para uma fila
 * @returns fila alocada
 * @pre nenhuma
 * @post retorna um ponteiro para uma fila
 */
Fila* criar_fila();
/*
 * @brief Le o cabecalho de dados no arquivo binario
 * @param dados Arquivo binario contendo os dados dos produtos
 * @returns Cabecalho do arquivo de dados
 * @pre O arquivo deve esta aberto
 * @post Leu o cabecalho de dados
 */
Cab_dados le_cab_dados(FILE *dados);
/*
 * @brief Abre ou cria o arquivo binario de dados
 * @returns O arquivo aberto
 * @pre nenhuma
 * @post Arquivo é aberto para edição
 */
FILE *abrir_arquivo_dados();
/*
 * @brief Le o Produto no arquivo de dados
 * @param dados Arquivo binario contendo os dados dos produtos
 * @param pos posição no arquivo binario
 * @returns Um produto
 * @pre O arquivo deve esta aberto e a posição valida
 * @post Retorna o produto lido no arquivo de dados
 */
Produto ler_produto(FILE *dados, int pos);

#endif // PRODUTO_H
