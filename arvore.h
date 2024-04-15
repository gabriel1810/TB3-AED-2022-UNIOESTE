#ifndef ARVORE_H
#define ARVORE_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ORDEM 7

typedef struct
{
    int topo;  // Aponta para o topo do cabecalho de dados
    int livre; // Aponta para primeira posicao livre
} Cab_dados;

typedef struct
{
    int raiz;  // Raiz da arvore
    int topo;  // Aponta para o topo do cabecalho de indices
    int livre; // Aponta para primeira posicao livre
} Cab_index;

typedef struct no
{
    int numChaves;        // Numero de chaves ocupadas no Nó
    int chave[ORDEM];     // Vetor de chaves do Nó
    int dados[ORDEM];     // Vetor com a posicao dos dados referentes a chave
    int filho[ORDEM + 1]; // Vetor com a posição dos filhos de cada registro
} Arvore;

/*
 * @brief Abre ou cria o arquivo que vai ser escrito a arvore e coloca o cabeçalho
 * @returns Um ponteiro para um arquivo
 * @pre nenhuma
 * @post é retornado um ponrteiro para um arquivo binario
 */
FILE *abrir_arquivo_index();
/*
 * @brief Faz o split padrao 2-3 da arvore B*, realocando os registros, dados e filhos necessarios
* @param pai no pai da raiz
* @param raiz no que esta com overflow
* @param irmao irmao que sera usado no split
* @param index Arquivo binario contendo os index da arvore
* @param cab ponteiro para o cabacelho do arquivo de index
* @param fl flag que indica com qual irmao sera o split, 0 para esquerda, 1 para direita
 * @returns novo no criado com tamanho dois tercos da ORDEM
 * @pre raiz deve estar com overflow e ter um irmao com ORDEM-1 registros
 * @post realoca os registros da raiz, pai e irmao para satisfazer o split 2-3 da arvore B*
 */
Arvore split_duplo(Arvore *pai, Arvore *raiz, Arvore *irmao, FILE *index, Cab_index *cab, int fl);
/*
 * @brief Le o cabecalho do arquivo contendo a arvore
 * @param index Arquivo binario contendo os index da arvore
 * @returns o cabecalho que foi lido do arquivo
 * @pre O arquivo contendo a arvore deve estar aberto
 * @post cabecalho é lido no arquivo
 */
Cab_index le_cab_index(FILE *index);
/*
 * @brief Le um nó no arquivo contendo a arvore
 * @param index Arquivo binario contendo os index da arvore
 * @param pos posição do nó no arquivo
 * @returns o nó que foi lido
 * @pre O arquivo contendo a arvore deve estar aberto e a posição tem que ser valida
 * @post retorna o nó lido
 */
Arvore ler_no(FILE *index, int pos);
/*
 * @brief cria e inicializa um nó da arvore
 * @returns o novo nó inicializado
 * @pre nenhuma
 * @post novo nó ja inicializado retornado
 */
Arvore cria_no();
/*
 * @brief Realiza a operação de split em um nó com overflow
 * @param p Nó com overflow
 * @param info informação que vai ser promovido
 * @param dado dado do meio que vai ser promovido
 * @returns o novo nó gerado pelo split
 * @pre o nó passado deve ser valido
 * @post retorna o nó resultado do split
 */
Arvore split(Arvore *x, int *info, int *dado);
/*
 * @brief Inicializa o cabecalho do arquivo contendo a arvore
 * @param index Arquivo binario contendo os index da arvore
 * @pre O arquivo contendo a arvore deve estar aberto
 * @post Cabecalho é iniciado e escrito no arquivo
 */
void cria_cab_index(FILE *index);
/*
 * @brief escreve no arquivo binario o cabeçalho de index
 * @param index Arquivo binario contendo os index da arvore
 * @param cab Cabecalho do arquivo contendo a arvore
 * @pre O arquivo contendo a arvore deve estar aberto
 * @post Esccreve o cabecalho no arquivo de index
 */
void escreve_cab_index(FILE *index, Cab_index cab);
/*
 * @brief Escreve um nó da arvore no arquivo de index
 * @param index Arquivo binario contendo os index da arvore
 * @param p Produto a ser armazenado
 * @param pos Posição que deve ser escrito o nó
 * @pre O arquivo contendo a arvore deve estar aberto
 * @post O nó é escrito  no arquivo.
 */
void escreve_no(FILE *index, Arvore x, int pos);
/*
 * @brief Insere uma nova chave em um no de Arvore, deslocando as informacoes ja escritas caso necessario
 * @param raiz nó onde vai ser realizado a operação
 * @param codigo codigo a ser inserido
 * @param posDado posição do dado no nó
 * @param posChave posição da chave no nó
 * @param filho chave do filho
 * @pre No deve ser valido
 * @post a chave é inserida no nó da arvore
 */
void adiciona_direita(Arvore *raiz, int codigo, int posDado, int posChave, int filho);
/*
 * @brief Busca o local onde a chave deve ser inserida e a insere
 * @param index Arquivo binario contendo os index da arvore
 * @param cab cabeçalho do arquivo de index
 * @param raiz nó onde vai ser inserido a chave
 * @param codigo codigo a ser inserido
 * @param posDado posição do dado no arquivo binario
 * @pre O arquivo contendo a arvore deve estar aberto
 * @post
 */
void insere_arvore_aux(FILE *index, Cab_index *cab, Arvore *raiz, int codigo, int posDado);
/*
 * @brief Procura pelo local correto onde a chave passada deve ser inserida, em seguida a insere
 * @param index Arquivo binario contendo os index da arvore
 * @param codigo Codigo a ser inserido
 * @param posDado Posição onde deve inserir
 * @pre O arquivo contendo a arvore deve estar aberto
 * @post insere a nova chave na arvore, escrevendo no arquivo de index
 */
void inserir_arvore(FILE *index, int codigo, int posDado);

/*
 * @brief funcao auxiliar que desloca as chaves para realizar remocao
 * @param raiz nó que tera os registros deslocados
 * @pre deve ser passado a raiz e a posicao do registro
 * @post as chaves sao deslocadas
 */
void remover_aux(Arvore *raiz, int pos);
/*
 * @brief Junta duas chaves para que o underflow seja corrigido
 * @param index Arquivo binario contendo os index da arvore
 * @param raiz raiz da arvore
 * @param pos posicao do filho
 * @param posNo posição do no no arquivo de index
 * @pre O arquivo contendo a arvore deve estar aberto
 * @post faz merge no filho com underflow com seu irmão adjacente
 */
void merge(FILE *index, Arvore *raiz, int pos, int posNo);
/*
 * @brief Remove um codigo da arvore
 * @param index Arquivo binario contendo os index da arvore
 * @param raiz  raiz da arvore deve
 * @param codigo codigo do produto a ser removido
 * @param posNo posição do no no arquivo de index
 * @pre O arquivo contendo a arvore deve estar aberto
 * @post o nó é removido da arvore
 */
void remover_arvore(FILE *index, Arvore *raiz, int codigo, int posNo);

/*
 * @brief Remove um produto de ambos os arquivos
 * @param index Arquivo binario contendo os index da arvore
 * @param dados Arquivo binario contendo os dados dos produtos
 * @param codigo codigo do produto a ser removido
 * @pre Os arquivos contendo a arvore e dados deve estar abertos e o codigo valido
 * @post o produto é removido dos arquivos
 */
void remover(FILE *index, FILE *dados, int codigo);
/*
 * @brief Faz print in order da arvore
 * @param index Arquivo binario contendo os index da arvore
 * @param dados Arquivo binario contendo os dados dos produtos
 * @param raiz raiz da arvore a ser impressa
 * @pre a raiz deve ser valida, Os arquivos contendo a arvore e dados deve estar abertos
 * @post é impressa a arvore in ordem
 */
void imprime_in_ordem(FILE *index, FILE *dados, Arvore raiz);
/*
 * @brief Imprime a arvore por nivel visualmente
 * @param index Arquivo binario contendo os index da arvore
 * @param raiz nó da raiz da arvore
 * @pre O arquivo contendo a arvore deve estar aberto e a raiz deve ser valida
 * @post Imprime a arvore em niveis
 */
void imprime_por_nivel(FILE *index, Arvore raiz);

/*
 * @brief le uma linha no arquivo de texto e realiza as operações indicadas naquela linha.
 * @param f Arquivo de texto
 * @param index Arquivo binario contendo os index da arvore
 * @param dados Arquivo binario contendo os dados dos produtos
 * @pre Os arquivos contendo a arvore e dados deve estar abertos
 * @post sao realizadas as operações de cada linha
 */
void le_arquivo_txt(char *f, FILE *index, FILE *dados);
/*
 * @brief Verifica se nó esta em overflow
 * @param raiz nó da arvore
 * @returns 1 se sim, 0 se não
 * @pre o nó deve ser valido
 * @post retorna um inteiro
 */
int overflow(Arvore no);
/*
 * @brief verifica se um nó é folha
 * @param no nó a ser verificado
 * @returns 1 se é folha, 0 se não for
 * @pre o nó passado é valido
 * @post é verificado se é folha
 */
int eh_folha(Arvore no);
/*
 * @brief Indica se uma determinada chave esta presente neste no e sua posicao eh passada por referencia
 * @param raiz nó a ser verificado
 * @param info codigo do nó que esta procurando
 * @param pos posição do no no arquivo da arvore
 * @returns retorna 1 se achou, 0 se não achou
 * @pre O nó passado deve ser valido
 * @post retorna se achou ou não
 */
int busca_pos(Arvore *raiz, int info, int *pos);
/* @brief Retorna posiçao do No do pai no arquivo Index, posVetorFilhos recebe posição do filho em que o codigo foi encontrado
*  @param index arquivo de indice
*  @param codProcura codigo a ser procurado
*  @param posChavePai é a posicao que o registro pai foi encontrado no Nó pai
*  @returns retorna a posicao do pai no arquivo de indice ou -1 caso nao seja encontrado
*/
int busca_pai(FILE *index, int codProcura, int *posVetorFilhos, int *posChavePai);

/* @brief Funçao que faz as verificações necessarios de overflow, faz a redistribuicao ou split caso necessario
*  @param index arquivo de indices
*  @param pai Nó pai da raiz
*  @param raiz Nó da arvore a ser analisado
*  @post realiza as operações de acordo com a necessidade, seja redistribuiçao ou split
*/
int teste(FILE *index, Arvore *pai, Arvore *raiz, Cab_index *cab);
/*
 * @brief Inicializa um No com a raiz da Arvore, retorna o resultado da funçao que busca nos Nós recursivamente
 * @param index Arquivo binario contendo os index da arvore
 * @param codigo index do dado a ser buscado
 * @returns posição do dado
 * @pre O arquivo contendo a arvore deve estar aberto e a posição tem que ser valida
 * @post é retornado a posicao do dado no arquivo de dados ou -1 caso codigo invalido
 */
int busca_dado(FILE *index, int codigo);
/*
 * @brief Busca a posição do dado nos nós da arvore
 * @param index Arquivo binario contendo os index da arvore
 * @param raiz Raiz da arvore de index
 * @param codigo codigo a ser procurado
 * @returns retorna a posição do dado no arquivo de dados
 * @pre O arquivo contendo a arvore deve estar aberto e a posição tem que ser valida
 * @post retorna a posição do dado no arquivo de dados
 */
int busca_dado_aux(FILE *index, Arvore raiz, int codigo);
/*
 * @brief  Move o registro de um nó irmao para a raiz com underflow
 * @param index Arquivo binario contendo os index da arvore
 * @param raiz Raiz da arvore
 * @param pos posição da chave
 * @param posNo posição do no no arquivo
 * @returns 1 se houver emprestimo ou 0 se não
 * @pre O arquivo contendo a arvore deve estar aberto
 * @post tenta realizar emprestimo
 */
int emprestimo(FILE *index, Arvore *raiz, int pos, int posNo);
/*
 * @brief Verifica se o nó possui menos da metade de posicoes cheias
 * @param raiz nó da arvore
 * @returns 1 se sim, 0 se não
 * @pre o nó deve ser valido
 * @post retorna um inteiro
 */
int underflow(Arvore raiz);
/*
 * @brief Retorna o menor elemento da arvore
 * @param index Arquivo binario contendo os index da arvore
 * @param raiz Raiz da arvore
 * @returns o menor inteiro na arvore
 * @pre O arquivo contendo a arvore deve estar aberto
 * @post acha o menor inteiro
 */
int menor_elemento(FILE *index, Arvore raiz);
#endif // ARVOREB_H
