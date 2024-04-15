#ifndef INTERFACE_H
#define INTERFACE_H
#include <stdio.h>
/*
 * @brief Imprime as opções do menu
 * @returns o index da opção no menu
 * @pre
 * @post
 */
int menu();
/*
 * @brief Menu para o usuario digitar o nome do arquivo de texto
 * @param index Arquivo binario contendo os index da arvore
 * @param dados Arquivo binario contendo os dados dos produtos
 * @pre nenhuma
 * @post Se o arquivo existe, lê o arquivo.
 */
void menu_ler_arquivo(FILE *arqIndex, FILE *arqData);
/*
 * @brief Menu para o usuario inserir as informações do produto
 * @param index Arquivo binario contendo os index da arvore
 * @param dados Arquivo binario contendo os dados dos produtos
 * @pre nenhuma
 * @post o produto é inserido nos arquivos binarios
 */
void menu_inserir(FILE *arqIndex, FILE *arqData);
/*
 * @brief Menu para o usuario alterar o preço de um produto
 * @param index Arquivo binario contendo os index da arvore
 * @param dados Arquivo binario contendo os dados dos produtos
 * @pre o codigo do produto existe
 * @post o preço do produto é alterado
 */
void menu_alterar_preco(FILE *arqIndex, FILE *arqData);
/*
 * @brief Menu para o usuario alterar o estoque de um produto
 * @param index Arquivo binario contendo os index da arvore
 * @param dados Arquivo binario contendo os dados dos produtos
 * @pre o codigo do produto existe
 * @post o estoque do produto é alterado
 */
void menu_alterar_estoque(FILE *arqIndex, FILE *arqData);
/*
 * @brief Menu para o usuario digitar o codigo do produto que ira ser removido
 * @param index Arquivo binario contendo os index da arvore
 * @param dados Arquivo binario contendo os dados dos produtos
 * @pre o codigo do produto existe
 * @post o produto é removido
 */
void menu_remover(FILE *arqIndex, FILE *arqData);
/*
 * @brief Menu para o usuario inserir o codigo do produto que deseja imprimir
 * @param index Arquivo binario contendo os index da arvore
 * @param dados Arquivo binario contendo os dados dos produtos
 * @pre o produto existe
 * @post o produto é impresso
 */
void menu_imprimir_item(FILE *arqIndex, FILE *arqData);
/*
 * @brief Menu para imprimir a arvore por nivel
 * @param index Arquivo binario contendo os index da arvore
 * @pre nenhuma
 * @post imprime a arvore por nivel
 */
void imprimir_arvore(FILE *arqIndex);
/*
 * @brief Imprime todos os produtos armazenados
 * @param index Arquivo binario contendo os index da arvore
 * @param dados Arquivo binario contendo os dados dos produtos
 * @pre nenhuma
 * @post todos os produtos existentes são impressos
 */
void imprimir_tudo(FILE *arqIndex, FILE *arqData);
/*
 * @brief Imprime as posições livres no arquivo de index
 * @param index Arquivo binario contendo os index da arvore
 * @pre nenhuma
 * @post É impresso as posições livres
 */
void imprimir_livres_arvore(FILE *arqIndex);
/*
 * @brief Imprime as posições livres no arquivo de dados
 * @param dados Arquivo binario contendo os dados dos produtos
 * @pre nenhuma
 * @post Imprime as posições livres
 */
void imprimir_livres_dados(FILE *arqData);
/*
 * @brief Imprime um produto armazenado no arquivo de dados
 * @param dados Arquivo binario contendo os dados dos produtos
 * @param posDado Posição do dado a ser impresso
 * @pre O produto existe
 * @post É impresso o produto
 */
void imprimir_item(FILE *arqData, int posDado);

#endif // INTERFACE_H
