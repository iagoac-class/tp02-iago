#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "arvores.h"

// INTEGRANTES DO GRUPO; Iago Sachsida Teixeira Rassilan Braga           Pedro Brassi Luccas
// Codigo testado em um ambiente Linux Pessoal.

// Criação da estrutura e funções da árvore balanceada:

/*
    Função que cria um novo nó
    x -> valor a ser inserido no nó
    Retorna: o endereço do nó criado ou NULL se a alocação falhar
*/
no_balanceada *novoNoBalanceada(int x)
{
    no_balanceada *novo = malloc(sizeof(no_balanceada)); // Aloca memória para um novo nó

    if (novo)
    {
        novo->valor = x; // Atribui o valor ao nó
        novo->esquerdo = NULL; // Inicializa o filho esquerdo como NULL
        novo->direito = NULL; // Inicializa o filho direito como NULL
        novo->altura = 0; // Inicializa a altura como 0
    }
    else
        printf("\nERRO ao alocar nó em novoNo!\n"); // Mensagem de erro se falhar a alocação
    return novo;
}

/*
    Retorna o maior dentre dois valores
    a, b -> altura de dois nós da árvore
*/
int maior(int a, int b)
{
    return (a > b) ? a : b; // Retorna o maior valor
}

// Retorna a altura de um nó ou -1 caso ele seja NULL
int alturaDoNo(no_balanceada *no)
{
    if (no == NULL)
        return -1; // Se o nó é NULL, retorna -1
    else
        return no->altura; // Retorna a altura do nó
}

// Calcula e retorna o fator de balanceamento de um nó
int fatorDeBalanceamento(no_balanceada *no)
{
    if (no)
        // O fator de balanceamento é a diferença entre as alturas das subárvores esquerda e direita
        return (alturaDoNo(no->esquerdo) - alturaDoNo(no->direito));
    else
        return 0; // Se o nó é NULL, o fator é 0
}

// --------- ROTAÇÕES ---------------------------

// Função para a rotação à esquerda
// Utilizada para manter a propriedade de balanceamento da árvore AVL
no_balanceada *rotacaoEsquerda(no_balanceada *r)
{
    no_balanceada *y, *f;

    y = r->direito; // O novo nó que será a raiz da subárvore
    f = y->esquerdo; // O filho esquerdo do novo nó

    // Realiza a rotação
    y->esquerdo = r; // O antigo nó raiz se torna o filho esquerdo do novo nó
    r->direito = f; // O antigo filho esquerdo do novo nó agora se torna o filho direito do antigo nó raiz

    // Atualiza as alturas dos nós
    r->altura = maior(alturaDoNo(r->esquerdo), alturaDoNo(r->direito)) + 1;
    y->altura = maior(alturaDoNo(y->esquerdo), alturaDoNo(y->direito)) + 1;

    return y; // Retorna a nova raiz da subárvore
}

// Função para a rotação à direita
// Utilizada para corrigir desbalanceamento em árvores AVL
no_balanceada *rotacaoDireita(no_balanceada *r)
{
    no_balanceada *y, *f;

    y = r->esquerdo; // O novo nó que será a raiz da subárvore
    f = y->direito; // O filho direito do novo nó

    // Realiza a rotação
    y->direito = r; // O antigo nó raiz se torna o filho direito do novo nó
    r->esquerdo = f; // O antigo filho direito do novo nó agora se torna o filho esquerdo do antigo nó raiz

    // Atualiza as alturas dos nós
    r->altura = maior(alturaDoNo(r->esquerdo), alturaDoNo(r->direito)) + 1;
    y->altura = maior(alturaDoNo(y->esquerdo), alturaDoNo(y->direito)) + 1;

    return y; // Retorna a nova raiz da subárvore
}

// Rotação dupla esquerda-direita
no_balanceada *rotacaoEsquerdaDireita(no_balanceada *r)
{
    r->esquerdo = rotacaoEsquerda(r->esquerdo); // Primeiro realiza a rotação à esquerda na subárvore esquerda
    return rotacaoDireita(r); // Depois realiza a rotação à direita na raiz original
}

// Rotação dupla direita-esquerda
no_balanceada *rotacaoDireitaEsquerda(no_balanceada *r)
{
    r->direito = rotacaoDireita(r->direito); // Primeiro realiza a rotação à direita na subárvore direita
    return rotacaoEsquerda(r); // Depois realiza a rotação à esquerda na raiz original
}

/*
    Função para realizar o balanceamento da árvore após uma inserção ou remoção
    Recebe o nó que está desbalanceado e retorna a nova raiz após o balanceamento
    O balanceamento é crucial para manter a eficiência das operações na árvore AVL.
*/
no_balanceada *balancear(no_balanceada *raiz)
{
    int fb = fatorDeBalanceamento(raiz); // Calcula o fator de balanceamento do nó

    // Rotação à esquerda
    if (fb < -1 && fatorDeBalanceamento(raiz->direito) <= 0)
        raiz = rotacaoEsquerda(raiz);

    // Rotação à direita
    else if (fb > 1 && fatorDeBalanceamento(raiz->esquerdo) >= 0)
        raiz = rotacaoDireita(raiz);

    // Rotação dupla à esquerda
    else if (fb > 1 && fatorDeBalanceamento(raiz->esquerdo) < 0)
        raiz = rotacaoEsquerdaDireita(raiz);

    // Rotação dupla à direita
    else if (fb < -1 && fatorDeBalanceamento(raiz->direito) > 0)
        raiz = rotacaoDireitaEsquerda(raiz);

    return raiz; // Retorna a nova raiz da árvore balanceada
}

/*
    Insere um novo nó na árvore balanceada
    raiz -> raiz da árvore
    x -> valor a ser inserido
    Retorno: endereço do novo nó ou nova raiz após o balanceamento
    A inserção em uma árvore AVL requer ajustes para manter a propriedade de balanceamento.
*/
no_balanceada *inserirBalanceada(no_balanceada *raiz, int x)
{
    if (raiz == NULL) // Se a árvore está vazia
        return novoNoBalanceada(x); // Cria um novo nó
    else
    { // Decide se insere à esquerda ou à direita
        if (x < raiz->valor)
            raiz->esquerdo = inserirBalanceada(raiz->esquerdo, x);
        else if (x > raiz->valor)
            raiz->direito = inserirBalanceada(raiz->direito, x);
    }

    // Recalcula a altura após a inserção
    raiz->altura = maior(alturaDoNo(raiz->esquerdo), alturaDoNo(raiz->direito)) + 1;

    // Verifica a necessidade de rebalancear a árvore
    raiz = balancear(raiz);

    return raiz; // Retorna a raiz (pode ser nova se ocorreu rebalanceamento)
}

/*
    Função para remover um nó da árvore binária balanceada
    Pode ser necessário rebalancear a árvore e a raiz pode ser alterada
    por isso precisamos retornar a raiz
    A remoção em uma árvore AVL também requer ajustes para manter a propriedade de balanceamento.
*/
no_balanceada *removerBalanceada(no_balanceada *raiz, int chave)
{
    if (raiz == NULL)
    {
        printf("Valor não encontrado!\n"); // Mensagem se o valor não for encontrado
        return NULL;
    }
    else
    { // Procura o nó a remover
        if (raiz->valor == chave)
        {
            // Caso 1: nó folha (sem filhos)
            if (raiz->esquerdo == NULL && raiz->direito == NULL)
            {
                free(raiz); // Libera a memória do nó removido
                return NULL; // Retorna NULL pois não há mais nós
            }
            else
            {
                // Caso 2: nó com dois filhos
                if (raiz->esquerdo != NULL && raiz->direito != NULL)
                {
                    no_balanceada *aux = raiz->esquerdo; // Encontra o maior nó da subárvore esquerda
                    while (aux->direito != NULL)
                        aux = aux->direito; // Navega até o maior nó

                    raiz->valor = aux->valor; // Substitui o valor do nó a ser removido
                    aux->valor = chave; // Atribui a chave ao nó auxiliar
                    raiz->esquerdo = removerBalanceada(raiz->esquerdo, chave); // Remove o nó auxiliar
                    return raiz; // Retorna a raiz
                }
                else
                {
                    // Caso 3: nó com um filho
                    no_balanceada *aux;
                    if (raiz->esquerdo != NULL)
                        aux = raiz->esquerdo; // Se tiver filho esquerdo, define como auxiliar
                    else
                        aux = raiz->direito; // Se não, é o filho direito
                    free(raiz); // Libera a memória do nó removido
                    return aux; // Retorna o filho
                }
            }
        }
        else
        {
            // Navega pela árvore para encontrar a chave
            if (chave < raiz->valor)
                raiz->esquerdo = removerBalanceada(raiz->esquerdo, chave);
            else
                raiz->direito = removerBalanceada(raiz->direito, chave);
        }

        // Recalcula a altura após a remoção
        raiz->altura = maior(alturaDoNo(raiz->esquerdo), alturaDoNo(raiz->direito)) + 1;

        // Verifica a necessidade de rebalancear a árvore
        raiz = balancear(raiz);

        return raiz; // Retorna a raiz (pode ser nova se ocorreu rebalanceamento)
    }
}

no_balanceada *root_avl = NULL; // Inicializa a raiz da árvore balanceada

// Função para medir o tempo de operações em uma árvore binária balanceada
double arvore_balanceada(int instancia_num)
{
    double tempoB = 0;
    clock_t beginB = clock(); // Marca o início do tempo
    char textNumber[12];

    sprintf(textNumber, "%d", instancia_num);
    char fileName[15] = "instancias/";
    strcat(fileName, textNumber); // Concatena o caminho do arquivo com o número da instância

    FILE *readFile = fopen(fileName, "r"); // Abre o arquivo de instâncias
    char leitor;
    int valor;

    // Lê o arquivo até o fim
    while (fscanf(readFile, "%c %d", &leitor, &valor) != EOF)
    {
        switch (leitor)
        {
        case 'I': // Caso de inserção
            root_avl = inserirBalanceada(root_avl, valor);
            break;
        case 'R': // Caso de remoção
            root_avl = removerBalanceada(root_avl, valor);
            break;
        }
    }
    clock_t endB = clock(); // Marca o fim do tempo
    tempoB += (double)(endB - beginB) / CLOCKS_PER_SEC; // Calcula o tempo total
    fclose(readFile); // Fecha o arquivo
    return (tempoB); // Retorna o tempo gasto
}

// Criação da estrutura e funções da árvore não balanceada:

// Cria o nó raiz da árvore
// A estrutura do nó contém um valor inteiro e ponteiros para os filhos esquerdo e direito.
no_NB *raiz_no(int valor)
{
    no_NB *temp = (no_NB *)malloc(sizeof(no_NB));
    temp->valor = valor;  // Atribui o valor ao nó
    temp->esquerda = temp->direita = NULL; // Inicializa os filhos como NULL
    return temp;
}

// Função para inserir um novo valor na árvore
// posicionando o novo nó na subárvore correta.
no_NB *inserirNB(no_NB *raiz, int valor)
{
    // Se a árvore está vazia, retorna um novo nó
    if (raiz == NULL)
    {
        return raiz_no(valor);
    }
    // Se o valor a ser inserido é menor, insere na subárvore esquerda
    if (valor < (raiz->valor))
    {
        raiz->esquerda = inserirNB(raiz->esquerda, valor);
    }
    // Se o valor a ser inserido é maior, insere na subárvore direita
    else if (valor > (raiz->valor))
    {
        raiz->direita = inserirNB(raiz->direita, valor);
    }

    return raiz; // Retorna a raiz (inicial ou modificada)
}

// Função para remover um nó da árvore
// Nó folha, nó com um filho, ou nó com dois filhos.
no_NB *removerNB(no_NB *raiz, int valor)
{
    // Se a árvore está vazia, não faz nada
    if (raiz == NULL)
        return raiz;

    // Localiza o nó a ser removido
    if (valor < (raiz->valor))
    {
        raiz->esquerda = removerNB(raiz->esquerda, valor);
    }
    else if (valor > (raiz->valor))
    {
        raiz->direita = removerNB(raiz->direita, valor);
    }
    else // O nó foi encontrado
    {
        // Caso 1: nó sem filhos
        if (raiz->esquerda == NULL)
        {
            no_NB *no_temporario = raiz->direita;
            free(raiz); // Libera a memória do nó removido
            return no_temporario;
        }
        else if (raiz->direita == NULL) // Caso 2: nó com um filho
        {
            no_NB *no_temporario = raiz->esquerda;
            free(raiz);
            return no_temporario;
        }

        // Caso 3: nó com dois filhos
        no_NB *no_substituto = raiz->direita;
        // Encontra o menor nó na subárvore direita
        while (no_substituto->esquerda != NULL)
        {
            no_substituto = no_substituto->esquerda;
        }

        // Substitui o valor do nó a ser removido pelo valor do nó substituto
        raiz->valor = no_substituto->valor;
        // Remove o nó substituto da árvore
        raiz->direita = removerNB(raiz->direita, no_substituto->valor);
    }
    return raiz; // retorna a raiz inicial ou modificada
}

no_NB *root_NB = NULL; // Inicializa a raiz da árvore não balanceada

// Função para medir o tempo de operações em uma árvore binária não balanceada
double arvore_binaria(int instancia_num)
{
    double tempo = 0;
    clock_t beginNB = clock(); // Marca o início do tempo
    char textNumber[12];

    sprintf(textNumber, "%d", instancia_num);
    char fileName[15] = "instancias/";
    strcat(fileName, textNumber); // Concatena o caminho do arquivo com o número da instância

    FILE *readFile = fopen(fileName, "r"); // Abre o arquivo de instâncias
    char leitor;
    int valor;

    // Lê o arquivo até o fim
    while (fscanf(readFile, "%c %d", &leitor, &valor) != EOF)
    {
        switch (leitor)
        {
        case 'I': // Caso de inserção
            root_NB = inserirNB(root_NB, valor);
            break;
        case 'R': // Caso de remoção
            root_NB = removerNB(root_NB, valor);
            break;
        }
    }
    clock_t endNB = clock(); // Marca o fim do tempo
    tempo += (double)(endNB - beginNB) / CLOCKS_PER_SEC; // Calcula o tempo total
    fclose(readFile); // Fecha o arquivo
    return (tempo); // Retorna o tempo gasto
}

int main(int argc, char *argv[])
{
    ///////////////////////////////////////////////////////////
    /////////////////// Leitor de instâncias //////////////////
    ///////////////// Não deve ser modificado /////////////////
    ///////////////////////////////////////////////////////////
    int instancia_num = -1;
    instancia_num = atoi(argv[1]); // Converte o argumento da linha de comando para um número
    if (instancia_num <= 0 || instancia_num > 10) // Verifica se o número está dentro do intervalo esperado
    {
        printf("Para executar o código, digite ./arvores x\nonde x é um número entre 1 e 10 que simboliza a instância utilizada\n");
        exit(0); // Encerra o programa se o número não for válido
    }

    // Executa as funções de operações nas árvores e mede o tempo gasto
    double tempo_n_balanceada = arvore_binaria(instancia_num);
    double tempo_balanceada = arvore_balanceada(instancia_num);

    // Exibe os tempos das operações nas duas árvores
    printf("%f\n%f\n", tempo_n_balanceada, tempo_balanceada);

    return (1); // Retorna 1 para indicar que o programa terminou com sucesso
}
