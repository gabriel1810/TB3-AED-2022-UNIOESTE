#include "arvore.h"
#include "produto.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

FILE *abrir_arquivo_index()
{
    FILE *arqIndex = fopen("Index.bin", "rb+");
    if (arqIndex == NULL)
    {
        arqIndex = fopen("Index.bin", "wb+");
        cria_cab_index(arqIndex);
    }
    return arqIndex;
}


void cria_cab_dados(FILE *arqData)
{
    Cab_dados cab;
    cab.topo = 0;
    cab.livre = -1;
    fseek(arqData, 0, SEEK_SET);
    fwrite(&cab, sizeof(Cab_dados), 1, arqData);
}

void cria_cab_index(FILE *arqIndex)
{
    Cab_index cab;
    cab.raiz = -1;
    cab.topo = 0;
    cab.livre = -1;
    fseek(arqIndex, 0, SEEK_SET);
    fwrite(&cab, sizeof(Cab_index), 1, arqIndex);
}

Cab_dados le_cab_dados(FILE *arqData)
{
    Cab_dados cab;
    fseek(arqData, 0, SEEK_SET);
    fread(&cab, sizeof(Cab_dados), 1, arqData);
    return cab;
}

Cab_index le_cab_index(FILE *arqIndex)
{
    Cab_index cab;
    fseek(arqIndex, 0, SEEK_SET);
    fread(&cab, sizeof(Cab_index), 1, arqIndex);
    return cab;
}

Arvore ler_no(FILE *arqIndex, int pos)
{
    Arvore x;
    fseek(arqIndex, sizeof(Cab_index), SEEK_SET);
    fseek(arqIndex, pos * sizeof(Arvore), SEEK_CUR);
    fread(&x, sizeof(Arvore), 1, arqIndex);
    return x;
}

void escreve_cab_index(FILE *arqIndex, Cab_index cab)
{
    fseek(arqIndex, 0, SEEK_SET);
    fwrite(&cab, sizeof(Cab_index), 1, arqIndex);
}


void escreve_no(FILE *arqIndex, Arvore x, int pos)
{
    fseek(arqIndex, sizeof(Cab_index), SEEK_SET);
    fseek(arqIndex, pos * sizeof(Arvore), SEEK_CUR);
    fwrite(&x, sizeof(Arvore), 1, arqIndex);
}


Arvore cria_no()
{
    Arvore no;
    int i;
    no.numChaves = 0;
    for (i = 0; i <= ORDEM; i++)
        no.chave[i] = no.dados[i] = no.filho[i] = -1;
    no.filho[i] = -1;
    return no;
}

int overflow(Arvore no)
{
    return (no.numChaves == ORDEM);
}

int eh_folha(Arvore no)
{
    return (no.filho[0] == -1);
}


Arvore split(Arvore *x, int *info, int *dado)
{
    printf("Fazendo split1-2\n");
    Arvore y = cria_no();
    int i, q = x->numChaves / 2;
    x->numChaves = q;
    y.numChaves = q;
    *info = x->chave[q];
    *dado = x->dados[q];
    y.filho[0] = x->filho[q + 1];
    for (i = 0; i < y.numChaves; i++)
    {
        y.chave[i] = x->chave[q + i + 1];
        y.dados[i] = x->dados[q + i + 1];
        y.filho[i + 1] = x->filho[q + i + 2];
    }
    return y;
}


int busca_pos(Arvore *raiz, int info, int *pos)
{
    for (*pos = 0; *pos < raiz->numChaves; (*pos)++)
    {
        if (info == raiz->chave[*pos])
            return 1;
        if (info < raiz->chave[*pos])
            break;
    }
    return 0;
}

void adiciona_direita(Arvore *raiz, int codigo, int posDado, int posChave, int filho)
{
    int i;

    for (i = raiz->numChaves; i > posChave; i--)
    {
        raiz->chave[i] = raiz->chave[i - 1];
        raiz->dados[i] = raiz->dados[i - 1];
        raiz->filho[i + 1] = raiz->filho[i];
    }
    raiz->chave[i] = codigo;
    raiz->dados[i] = posDado;
    raiz->filho[i + 1] = filho;
    raiz->numChaves++;
}

void insere_arvore_aux(FILE *arqIndex, Cab_index *cab, Arvore *raiz, int codigo, int posDado)
{
    int posChave;
    if (!busca_pos(raiz, codigo, &posChave))
    {
        if (eh_folha(*raiz))
            adiciona_direita(raiz, codigo, posDado, posChave, -1);

        else
        {
            Arvore filho = ler_no(arqIndex, raiz->filho[posChave]);
            insere_arvore_aux(arqIndex, cab, &filho, codigo, posDado);
            if (overflow(filho))
            {
                teste(arqIndex, raiz, &filho, cab);
            }
            escreve_no(arqIndex, filho, raiz->filho[posChave]);
        }
    }
}

Arvore split_duplo(Arvore *pai, Arvore *raiz, Arvore *irmao, FILE *arqIndex, Cab_index *cab, int fl)
{
    int tam = raiz->numChaves + irmao->numChaves + 1;
    int tamVetFilhos = raiz->numChaves + irmao->numChaves + 2;
    int s1 = (2 * ORDEM) / 3, s2 = (s1 * 2) + 1, j = 0, posChavePai, a, vetChave[tam], vetDados[tam], vetFilhos[tamVetFilhos], paiRaiz;
    Arvore novoNo = cria_no();

    //Se fl == 1 faz split com irmao da direita
    if (fl == 1)
    {
        //Procura a posicao do registro pai
        for (int i = 0; i < irmao->numChaves; i++)
        {
            paiRaiz = busca_pai(arqIndex, irmao->chave[i], &a, &posChavePai);
            if (paiRaiz != -1)
                break;
        }

        // Copia as chaves e dados para os vetores auxiliares
        for (int i = 0; i < tam; i++)
        {
            vetChave[i] = (i != raiz->numChaves) ? (i < raiz->numChaves ? raiz->chave[i] : irmao->chave[j]) : pai->chave[posChavePai];
            vetDados[i] = (i != raiz->numChaves) ? (i < raiz->numChaves ? raiz->dados[i] : irmao->dados[j++]) : pai->dados[posChavePai];
        }
        // Copia os filhos para o vetor auxiliar de filhos
        int k;
        j = 0;
        for (k = 0; k < tamVetFilhos; k++)
        {
            vetFilhos[k] = k < raiz->numChaves + 1 ? raiz->filho[k] : irmao->filho[j++];
        }

        for (int i = s1 + 1, j = 0; i < s2; i++)
        {
            irmao->chave[j] = vetChave[i];
            irmao->dados[j++] = vetDados[i];
        }
        k = j = 0;
        for (k = s1 + 1, j = 0; k < s2; k++)
        {
            irmao->filho[j++] = vetFilhos[k];
        }
        irmao->filho[j++] = vetFilhos[k];
    }

    //Se fl == 0 faz split com irmao da esquerda
    else if (fl == 0)
    {
        for (int i = 0; i < raiz->numChaves; i++)
        {
            paiRaiz = busca_pai(arqIndex, raiz->chave[i], &a, &posChavePai);
            if (paiRaiz != -1)
                break;
        }
        for (int i = 0; i < tam; i++)
        {
            vetChave[i] = (i != irmao->numChaves) ? (i < irmao->numChaves ? irmao->chave[i] : raiz->chave[j]) : pai->chave[posChavePai];
            vetDados[i] = (i != irmao->numChaves) ? (i < irmao->numChaves ? irmao->dados[i] : raiz->dados[j++]) : pai->dados[posChavePai];
        }
        int k;
        j = 0;
        for (k = 0; k < tamVetFilhos; k++)
        {
            vetFilhos[k] = k < irmao->numChaves + 1 ? irmao->filho[k] : raiz->filho[j++];
        }

        for (int i = s1 + 1, j = 0; i < s2; i++)
        {
            raiz->chave[j] = vetChave[i];
            raiz->dados[j++] = vetDados[i];
        }
        k = j = 0;
        for (k = s1 + 1, j = 0; k < s2; k++)
        {
            raiz->filho[j++] = vetFilhos[k];
        }
        raiz->filho[j++] = vetFilhos[k];
    }

    // Popula o novo no com os maiores registros
    for (int i = s2 + 1, j = 0; i < tam; i++)
    {
        novoNo.chave[j] = vetChave[i];
        novoNo.dados[j++] = vetDados[i];
        novoNo.numChaves++;
    }

    for (int i = s2 + 1, j = 0; i < tamVetFilhos; i++, j++)
    {
        novoNo.filho[j] = vetFilhos[i];
    }

    int posicao = cab->livre == -1 ? cab->topo : cab->livre;
    pai->chave[posChavePai] = vetChave[s1];
    pai->dados[posChavePai] = vetDados[s1];

    // shift right nas chaves do pai caso precise
    if (posChavePai + 1 < pai->numChaves)
    {
        for (j = pai->numChaves; j > posChavePai; j--)
        {
            pai->chave[j] = pai->chave[j - 1];
            pai->dados[j] = pai->dados[j - 1];
            pai->filho[j + 1] = pai->filho[j];
        }
        if (posChavePai != 0)
            pai->filho[j + 1] = pai->filho[j];
    }

    pai->numChaves++;
    pai->chave[posChavePai + 1] = vetChave[s2];
    pai->dados[posChavePai + 1] = vetDados[s2];
    pai->filho[posChavePai + 2] = posicao;
    raiz->numChaves = s1;
    irmao->numChaves = s1;
    return novoNo;
}

void shiftRight(Arvore *pai, Arvore *raiz, Arvore *irmao, int posPai)
{
    int i;
    for (i = irmao->numChaves; i > 0; i--)
    {
        irmao->chave[i] = irmao->chave[i - 1];
        irmao->dados[i] = irmao->dados[i - 1];
        irmao->filho[i + 1] = irmao->filho[i];
    }
    irmao->filho[i + 1] = irmao->filho[i];
    irmao->numChaves++;
    irmao->chave[0] = pai->chave[posPai];
    irmao->dados[0] = pai->dados[posPai];
    irmao->filho[0] = raiz->filho[raiz->numChaves];
    pai->chave[posPai] = raiz->chave[raiz->numChaves - 1];
    pai->dados[posPai] = raiz->dados[raiz->numChaves - 1];
    raiz->numChaves--;
}

void shiftLeft(Arvore *pai, Arvore *raiz, Arvore *irmao, int posPai)
{
    irmao->chave[irmao->numChaves] = pai->chave[posPai];
    irmao->dados[irmao->numChaves] = pai->dados[posPai];
    irmao->numChaves++;
    irmao->filho[irmao->numChaves] = raiz->filho[0];
    pai->chave[posPai] = raiz->chave[0];
    pai->dados[posPai] = raiz->dados[0];

    int j = raiz->numChaves - 1;
    for (int i = 0; i < j; i++)
    {
        raiz->chave[i] = raiz->chave[i + 1];
        raiz->dados[i] = raiz->dados[i + 1];
        raiz->filho[i] = raiz->filho[i + 1];
    }
    raiz->filho[raiz->numChaves - 1] = raiz->filho[raiz->numChaves];
    raiz->numChaves--;
}

int teste(FILE *arqIndex, Arvore *pai, Arvore *raiz, Cab_index *cab)
{
    int pos, posChavePai, paiRaiz, paiIrmao;

    if (raiz->numChaves == ORDEM)
    {
        for (int i = 0; i < raiz->numChaves; i++)
        {
            paiRaiz = busca_pai(arqIndex, raiz->chave[i], &pos, &posChavePai);
            if (paiRaiz != -1)
                break;
        }
        Arvore irmaoDir;
        if (pos < pai->numChaves)
            irmaoDir = ler_no(arqIndex, pai->filho[pos + 1]);
        Arvore irmaoEsq;
        if (pos - 1 >= 0)
            irmaoEsq = ler_no(arqIndex, pai->filho[pos - 1]);

        if (pos < pai->numChaves)
        {
            // Tem irmão na direita
            if ((pos - 1 >= 0 && irmaoEsq.numChaves > irmaoDir.numChaves && (irmaoDir.numChaves < ORDEM - 1)) || (pos - 1 < 0 && irmaoDir.numChaves < ORDEM - 1))
            {
                // Tem espaço, redistribui e insere
                int posChavepaiIrmao, posPontIrmao;
                for (int i = 0; i < irmaoDir.numChaves; i++)
                {
                    paiIrmao = busca_pai(arqIndex, irmaoDir.chave[i], &posPontIrmao, &posChavepaiIrmao);
                    if (paiIrmao != -1)
                        break;
                }
                if (paiRaiz == paiIrmao)
                {
                    printf("Tem espaco na direita, redistribuindo...\n");
                    shiftRight(pai, raiz, &irmaoDir, posChavepaiIrmao);
                    escreve_no(arqIndex, irmaoDir, pai->filho[pos + 1]);
                    return 1;
                }
            }
        }

        if (pos - 1 >= 0)
        {
            if (irmaoEsq.numChaves < ORDEM - 1)
            {
                // Tem espaço, redistribui e insere
                printf("Tem espaco na esquerda, redistribuindo...\n");
                for (int i = 0; i < raiz->numChaves; i++)
                {
                    paiRaiz = busca_pai(arqIndex, raiz->chave[i], &pos, &posChavePai);
                    if (paiRaiz != -1)
                        break;
                }
                shiftLeft(pai, raiz, &irmaoEsq, posChavePai);
                escreve_no(arqIndex, irmaoEsq, pai->filho[pos - 1]);
                return 1;
            }
        }

        if (raiz->numChaves == ORDEM)
        {
            // Se até agora não retornou é pq não tem espaço e precisa de um split
            printf("Nao possui espaco em nenhum irmao, ");
            if (pos < pai->numChaves)
            {
                printf("fazendo split2-3 com o irmao da direita...\n");
                Arvore novoNo = split_duplo(pai, raiz, &irmaoDir, arqIndex, cab, 1);
                printf("\n");
                escreve_no(arqIndex, irmaoDir, pai->filho[pos + 1]);
                escreve_no(arqIndex, novoNo, cab->topo++);
                return 1;
            }
            else
            {
                printf("fazendo split2-3 com o irmao da esquerda...\n");
                Arvore novoNo = split_duplo(pai, raiz, &irmaoEsq, arqIndex, cab, 0);
                printf("\n");
                escreve_no(arqIndex, irmaoEsq, pai->filho[pos - 1]);
                escreve_no(arqIndex, novoNo, cab->topo++);
                return 1;
            }
        }
    }
    return 1;
}


void inserir_arvore(FILE *arqIndex, int codigo, int posicaoDado)
{
    printf("\nInserindo: %d\n", codigo);
    Cab_index cab = le_cab_index(arqIndex);
    Arvore raiz, novaRaiz, aux, vazio;
    int infoMeio, dadoMeio;
    if (cab.raiz == -1)
    {
        raiz = cria_no();
        raiz.chave[0] = codigo;
        raiz.dados[0] = posicaoDado;
        raiz.numChaves++;
        if (cab.livre == -1)
        {
            cab.raiz = cab.topo;
            cab.topo++;
        }
        else
        {
            vazio = ler_no(arqIndex, cab.livre);
            cab.raiz = cab.livre;
            cab.livre = vazio.chave[0];
        }
        escreve_no(arqIndex, raiz, cab.raiz);
    }
    else
    {
        raiz = ler_no(arqIndex, cab.raiz);
        insere_arvore_aux(arqIndex, &cab, &raiz, codigo, posicaoDado);
        if (overflow(raiz))
        {
            printf("Overflow na raiz\n");
            aux = split(&raiz, &infoMeio, &dadoMeio);
            novaRaiz = cria_no();
            novaRaiz.chave[0] = infoMeio;
            novaRaiz.dados[0] = dadoMeio;
            novaRaiz.filho[0] = cab.raiz;
            novaRaiz.numChaves++;
            escreve_no(arqIndex, raiz, cab.raiz);
            if (cab.livre == -1)
            {
                novaRaiz.filho[1] = cab.topo;
                escreve_no(arqIndex, aux, cab.topo++);
            }
            else
            {
                vazio = ler_no(arqIndex, cab.livre);
                novaRaiz.filho[1] = cab.livre;
                escreve_no(arqIndex, aux, cab.livre);
                cab.livre = vazio.chave[0];
            }
            if (cab.livre == -1)
            {
                cab.raiz = cab.topo++;
                escreve_no(arqIndex, novaRaiz, cab.raiz);
            }
            else
            {
                vazio = ler_no(arqIndex, cab.livre);
                cab.raiz = cab.livre;
                escreve_no(arqIndex, novaRaiz, cab.raiz);
                cab.livre = vazio.chave[0];
            }
        }
        else
            escreve_no(arqIndex, raiz, cab.raiz);
    }
    escreve_cab_index(arqIndex, cab);
}

void remover_arquivo_dados(FILE *arqData, int posicaoDado)
{
    Cab_dados cab = le_cab_dados(arqData);
    Produto x = ler_produto(arqData, posicaoDado);
    if (cab.livre == -1)
    {
        cab.livre = posicaoDado;
        x.codigo = -1;
    }
    else
    {
        x.codigo = cab.livre;
        cab.livre = posicaoDado;
    }
    escreve_produto(arqData, x, posicaoDado);
    escreve_cab_dados(arqData, cab);
}


int underflow(Arvore raiz)
{
    return (raiz.numChaves < (ORDEM - 1) / 2 ? 1 : 0);
}

int menor_elemento(FILE *arqIndex, Arvore raiz)
{
    if (eh_folha(raiz))
        return raiz.chave[0];
    if (raiz.filho[0] != -1)
        return menor_elemento(arqIndex, ler_no(arqIndex, raiz.filho[0]));
    return -1;
}

void remover_aux(Arvore *raiz, int posicao)
{
    int i;
    for (i = posicao; i < raiz->numChaves - 1; i++)
    {
        raiz->chave[i] = raiz->chave[i + 1];
        raiz->dados[i] = raiz->dados[i + 1];
        raiz->filho[i] = raiz->filho[i + 1];
    }
    raiz->filho[i] = raiz->filho[i + 1];
    raiz->numChaves--;
}


int emprestimo(FILE *arqIndex, Arvore *raiz, int pos, int posNo)
{
    Arvore filho, irmaoEsq, irmaoDir;
    int posChave;
    filho = ler_no(arqIndex, raiz->filho[pos]);
    irmaoEsq = ler_no(arqIndex, raiz->filho[pos - 1]);
    irmaoDir = ler_no(arqIndex, raiz->filho[pos + 1]);
    if (pos - 1 >= 0)
    {
        if (irmaoEsq.numChaves > (ORDEM - 1) / 2)
        {
            adiciona_direita(&filho, raiz->chave[pos - 1], raiz->dados[pos - 1], 0, -1);
            escreve_no(arqIndex, filho, raiz->filho[pos]);
            posChave = irmaoEsq.numChaves - 1;
            raiz->chave[pos - 1] = irmaoEsq.chave[posChave];
            raiz->dados[pos - 1] = irmaoEsq.dados[posChave];
            escreve_no(arqIndex, *raiz, posNo);
            remover_aux(&irmaoEsq, posChave);
            escreve_no(arqIndex, irmaoEsq, raiz->filho[pos - 1]);
            return 1;
        }
    }
    if (pos + 1 <= raiz->numChaves)
    {
        if (irmaoDir.numChaves > (ORDEM - 1) / 2)
        {
            adiciona_direita(&filho, raiz->chave[pos], raiz->dados[pos], filho.numChaves, -1);
            escreve_no(arqIndex, filho, raiz->filho[pos]);
            raiz->chave[pos] = irmaoDir.chave[0];
            raiz->dados[pos] = irmaoDir.dados[0];
            escreve_no(arqIndex, *raiz, posNo);
            remover_aux(&irmaoDir, 0);
            escreve_no(arqIndex, irmaoDir, raiz->filho[pos + 1]);
            return 1;
        }
    }
    return 0;
}

void merge(FILE *arqIndex, Arvore *raiz, int pos, int posNo)
{
    Arvore filho, irmaoEsq, irmaoDir;
    Cab_index cab = le_cab_index(arqIndex);
    filho = ler_no(arqIndex, raiz->filho[pos]);
    irmaoEsq = ler_no(arqIndex, raiz->filho[pos - 1]);
    irmaoDir = ler_no(arqIndex, raiz->filho[pos + 1]);
    if (pos - 1 >= 0)
    {
        adiciona_direita(&filho, raiz->chave[pos - 1], raiz->dados[pos - 1], 0, filho.filho[0]);
        for (int i = 0; i < irmaoEsq.numChaves; i++)
            adiciona_direita(&filho, irmaoEsq.chave[i], irmaoEsq.dados[i], i, irmaoEsq.filho[i + 1]);
        filho.filho[0] = irmaoEsq.filho[0];
        escreve_no(arqIndex, filho, raiz->filho[pos]);
        if (cab.livre == -1)
        {
            cab.livre = raiz->filho[pos - 1];
            irmaoEsq.chave[0] = -1;
        }
        else
        {
            irmaoEsq.chave[0] = cab.livre;
            cab.livre = raiz->filho[pos - 1];
        }
        irmaoEsq.numChaves = 0;
        escreve_no(arqIndex, irmaoEsq, raiz->filho[pos - 1]);
        remover_aux(raiz, pos - 1);
        escreve_no(arqIndex, *raiz, posNo);
    }
    else if (pos + 1 <= raiz->numChaves)
    {
        adiciona_direita(&irmaoDir, raiz->chave[pos], raiz->dados[pos], 0, irmaoDir.filho[0]);
        for (int i = 0; i < filho.numChaves; i++)
            adiciona_direita(&irmaoDir, filho.chave[i], filho.dados[i], i, filho.filho[i + 1]);
        irmaoDir.filho[0] = filho.filho[0];
        escreve_no(arqIndex, irmaoDir, raiz->filho[pos + 1]);
        if (cab.livre == -1)
        {
            cab.livre = raiz->filho[pos];
            filho.chave[0] = -1;
        }
        else
        {
            filho.chave[0] = cab.livre;
            cab.livre = raiz->filho[pos];
        }
        filho.numChaves = 0;
        escreve_no(arqIndex, filho, raiz->filho[pos]);
        remover_aux(raiz, pos);
        escreve_no(arqIndex, *raiz, posNo);
    }
    escreve_cab_index(arqIndex, cab);
}

void remover_arvore(FILE *arqIndex, Arvore *raiz, int codigo, int posNo)
{
    int i, sucessor;
    Arvore filho;
    filho.numChaves = -1;
    for (i = 0; i < raiz->numChaves; i++)
    {
        if (codigo == raiz->chave[i] || codigo < raiz->chave[i])
            break;
    }
    if (i < raiz->numChaves && codigo == raiz->chave[i])
    {
        if (eh_folha(*raiz))
        {
            remover_aux(raiz, i);
            escreve_no(arqIndex, *raiz, posNo);
        }
        else
        {
            Arvore filhoDireita = ler_no(arqIndex, raiz->filho[i + 1]);
            sucessor = menor_elemento(arqIndex, filhoDireita);
            raiz->dados[i] = busca_dado(arqIndex, sucessor);
            raiz->chave[i] = sucessor;
            escreve_no(arqIndex, *raiz, posNo);
            remover_arvore(arqIndex, &filhoDireita, sucessor, raiz->filho[i + 1]);
            if (underflow(filhoDireita))
            {
                if (!emprestimo(arqIndex, raiz, i + 1, posNo))
                {
                    merge(arqIndex, raiz, i + 1, posNo);
                }
            }
        }
    }
    else
    {
        filho = ler_no(arqIndex, raiz->filho[i]);
        remover_arvore(arqIndex, &filho, codigo, raiz->filho[i]);
    }
    if (!eh_folha(*raiz))
    {
        if (filho.numChaves != -1 && underflow(filho))
        {
            if (!emprestimo(arqIndex, raiz, i, posNo))
            {
                merge(arqIndex, raiz, i, posNo);
            }
        }
    }
    if (raiz->numChaves == 0)
    {
        Cab_index cab = le_cab_index(arqIndex);
        if (cab.livre == -1)
        {
            raiz->chave[0] = -1;
            cab.livre = posNo;
        }
        else
        {
            raiz->chave[0] = cab.livre;
            cab.livre = posNo;
        }
        cab.raiz = raiz->filho[0];
        escreve_no(arqIndex, *raiz, posNo);
        escreve_cab_index(arqIndex, cab);
    }
}

void remover(FILE *arqIndex, FILE *arqData, int codigo)
{
    Cab_index cabIndex = le_cab_index(arqIndex);
    Arvore raiz = ler_no(arqIndex, cabIndex.raiz);
    int posDado = -1;

    if (cabIndex.raiz != -1)
        posDado = busca_dado(arqIndex, codigo);

    if (posDado != -1)
    {
        remover_arvore(arqIndex,&raiz,codigo, cabIndex.raiz);
        remover_arquivo_dados(arqData, posDado);
    }
    else
        printf("Codigo invalido!!\n");
}


int busca_dado_aux(FILE *arqIndex, Arvore raiz, int codigo)
{
    int i;
    for (i = 0; i < raiz.numChaves; i++)
    {
        if (raiz.chave[i] == codigo)
            return raiz.dados[i];
        if (codigo < raiz.chave[i])
            break;
    }
    if (raiz.filho[i] != -1)
        return busca_dado_aux(arqIndex, ler_no(arqIndex, raiz.filho[i]), codigo);
    else
        return -1;
}

int busca_dado(FILE *arqIndex, int codigo)
{
    Cab_index cab = le_cab_index(arqIndex);
    if (cab.raiz != -1)
    {
        Arvore raiz = ler_no(arqIndex, cab.raiz);
        return busca_dado_aux(arqIndex, raiz, codigo);
    }
    return -1;
}

void imprime_in_ordem(FILE *arqIndex, FILE *arqData, Arvore raiz)
{
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

void imprime_por_nivel(FILE *arqIndex, Arvore raiz)
{
    Fila *fila = criar_fila();
    int i;
    struct noFila *aux;
    Arvore vazio;
    vazio.numChaves = -1;
    enqueue(fila, raiz);
    enqueue(fila, vazio);
    int totalRegistros = 0;
    while (fila->inicio != NULL)
    {
        aux = dequeue(fila);
        if (aux->item.numChaves != -1)
        {
            printf("[");
            for (i = 0; i < aux->item.numChaves; i++, totalRegistros++)
            {
                if (i == aux->item.numChaves - 1)
                    printf("%d", aux->item.chave[i]);
                else
                    printf("%d ", aux->item.chave[i]);
                if (aux->item.filho[i] != -1)
                    enqueue(fila, ler_no(arqIndex, aux->item.filho[i]));
            }
            printf("] ");
            if (aux->item.filho[i] != -1)
                enqueue(fila, ler_no(arqIndex, aux->item.filho[i]));
        }
        else
        {
            printf("\n");
            if (fila->inicio != NULL)
                enqueue(fila, vazio);
        }
        free(aux);
    }
    printf("Quantidade de Registros: %d\n", totalRegistros);
    free(fila);
}

int busca_pai(FILE *arqIndex, int codProcura, int *posVetorFilhos, int *posChavePai)
{
    //Procura a posicao do no pai no arquivo de indices, tambem a posicao
    //do registro no vetor de chaves
    Cab_index cab = le_cab_index(arqIndex);
    for (int i = 0; i < cab.topo; i++)
    {
        Arvore no = ler_no(arqIndex, i);
        if (!(eh_folha(no)))
            for (int j = 0; j <= no.numChaves; j++)
            {
                if (no.filho[j] != -1)
                {
                    Arvore filho = ler_no(arqIndex, no.filho[j]);
                    if (codProcura >= filho.chave[0] && codProcura <= filho.chave[filho.numChaves - 1])
                    {
                        *posChavePai = j - 1 < 0 ? 0 : j - 1;
                        *posVetorFilhos = j;
                        return i;
                    }
                }
            }
    }
    return -1;
}

void tira_espacos(char *dst, char *branco)
{
    while (*branco)
    {
        if (*branco == ' ')
            if (*(branco + 1) == ' ')
                break;
        *dst++ = *branco++;
    }
    *dst = '\0';
}

char *swapChar(char *str)
{
    int tam = strlen(str);
    for (int i = 0; i < tam; i++)
        if (str[i] == ',')
            str[i] = '.';
    return str;
}


void le_arquivo_txt(char *fileName, FILE *arqIndex, FILE *arqData)
{
    char operacao, teste, branco[200];
    int qnt, cod;
    float preco;
    char precoComVirgula[10];
    Produto aux;
    FILE *f;
    f = fopen(fileName, "r");
    if (f != NULL)
    {
        while (fscanf(f, " %c ;", &operacao) != EOF)
        {
            switch (operacao)
            {
            case 'I':
                fscanf(f, " %d ;", &aux.codigo);
                fscanf(f, " %[^;];", branco);
                tira_espacos(aux.nome, branco);
                fscanf(f, " %[^;];", branco);
                tira_espacos(aux.marca, branco);
                fscanf(f, " %[^;];", branco);
                tira_espacos(aux.categoria, branco);
                fscanf(f, " %d ;", &aux.estoque);
                fscanf(f, "%s", precoComVirgula);
                swapChar(precoComVirgula);
                aux.preco = atof(precoComVirgula);
                inserir(arqIndex, arqData, aux);
                break;
            case 'A':
                preco = qnt = -1;
                fscanf(f, " %d ;", &cod);
                fscanf(f, " %c", &teste);
                if (teste != ';')
                {
                    ungetc(teste, f);
                    fscanf(f, " %d ;", &qnt);
                    fscanf(f, " %c", &teste);
                    if (teste >= '0' && teste <= '9')
                    {
                        ungetc(teste, f);
                        fscanf(f, "%s", precoComVirgula);
                        swapChar(precoComVirgula);
                        preco = atof(precoComVirgula);
                    }
                    else if (teste == 'A' || teste == 'I' || teste == 'R')
                        ungetc(teste, f);
                }
                else if (teste == ';')
                {
                    fscanf(f, "%s", precoComVirgula);
                    swapChar(precoComVirgula);
                    preco = atof(precoComVirgula);
                }
                alterar(arqIndex, arqData, cod, qnt, preco);
                break;
            case 'R':
                fscanf(f, " %d \n", &aux.codigo);
                remover(arqIndex, arqData, aux.codigo);
                break;
            default:
                printf("Operacao desconhecida\n");
            }
        }
        printf("\nLeitura concluida com sucesso\n");
        fclose(f);
    }
    else
        printf("\nArquivo nao encontrado\n");
}
