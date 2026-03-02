#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "avl.h"

#define AREA_BRASIL 8510000.0f

//Funções auxiliares internas (não exportadas)

static int altura(Arv *no) {
    return (no == NULL) ? 0 : no->altura;
}

static int maximo(int a, int b) {
    return (a > b) ? a : b;
}

static void atualizarAltura(Arv *no) {
    if (no != NULL)
        no->altura = 1 + maximo(altura(no->esq), altura(no->dir));
}

static int fatorBalanceamento(Arv *no) {
    return (no == NULL) ? 0 : altura(no->esq) - altura(no->dir);
}

//Aloca e inicializa um novo nó
static NO *novoNo(Municipio m) {
    NO *novo = (NO *)malloc(sizeof(NO));
    if (novo == NULL) {
        fprintf(stderr, "Erro: falha ao alocar memória.\n");
        exit(EXIT_FAILURE);
    }
    novo->dados  = m;
    novo->altura = 1;
    novo->esq    = NULL;
    novo->dir    = NULL;
    return novo;
}

// Rotações

static Arv *rotacaoDireita(Arv *y) {
    Arv *x  = y->esq;
    Arv *T2 = x->dir;

    x->dir = y;
    y->esq = T2;

    atualizarAltura(y);
    atualizarAltura(x);
    return x;
}

static Arv *rotacaoEsquerda(Arv *x) {
    Arv *y  = x->dir;
    Arv *T2 = y->esq;

    y->esq = x;
    x->dir = T2;

    atualizarAltura(x);
    atualizarAltura(y);
    return y;
}

// Balanceamento após inserção

static Arv *balancear(Arv *no, const char *chave) {
    int fb = fatorBalanceamento(no);

    //Caso Esquerda-Esquerda
    if (fb > 1 && strcmp(chave, no->esq->dados.nome) < 0)
        return rotacaoDireita(no);

    //Caso Direita-Direita
    if (fb < -1 && strcmp(chave, no->dir->dados.nome) > 0)
        return rotacaoEsquerda(no);

    //Caso Esquerda-Direita
    if (fb > 1 && strcmp(chave, no->esq->dados.nome) > 0) {
        no->esq = rotacaoEsquerda(no->esq);
        return rotacaoDireita(no);
    }

    //Caso Direita-Esquerda
    if (fb < -1 && strcmp(chave, no->dir->dados.nome) < 0) {
        no->dir = rotacaoDireita(no->dir);
        return rotacaoEsquerda(no);
    }

    return no; //Já balanceado
}

// Funções Básicas
 
Arv *criarArvore(void) {
    return NULL;
}

Arv *inserir(Arv *raiz, Municipio m) {
    // Inserção BST padrão
    if (raiz == NULL)
        return novoNo(m);

    int cmp = strcmp(m.nome, raiz->dados.nome);

    if (cmp < 0)
        raiz->esq = inserir(raiz->esq, m);
    else if (cmp > 0)
        raiz->dir = inserir(raiz->dir, m);
    else {
        //Chave duplicada: ignora
        printf("Aviso: município \"%s\" já cadastrado. Ignorando.\n", m.nome);
        return raiz;
    }

    atualizarAltura(raiz);
    return balancear(raiz, m.nome);
}

void emOrdem(Arv *raiz) {
    if (raiz == NULL) return;
    emOrdem(raiz->esq);
    printf("%s\n", raiz->dados.nome);
    emOrdem(raiz->dir);
}

void liberarArvore(Arv *raiz) {
    if (raiz == NULL) return;
    liberarArvore(raiz->esq);
    liberarArvore(raiz->dir);
    free(raiz);
}

// Funcionalidades Específicas

//i. Contar municípios
int contarMunicipios(Arv *raiz) {
    if (raiz == NULL) return 0;
    return 1 + contarMunicipios(raiz->esq) + contarMunicipios(raiz->dir);
}

//ii. Mostrar municípios com população > X
void mostrarPopulacaoMaiorQue(Arv *raiz, int X) {
    if (raiz == NULL) return;
    mostrarPopulacaoMaiorQue(raiz->esq, X);
    if (raiz->dados.populacao > X) {
        printf("  %-30s  %10d hab\n",
               raiz->dados.nome,
               raiz->dados.populacao);
    }
    mostrarPopulacaoMaiorQue(raiz->dir, X);
}

//iii. Densidade demográfica
void mostrarDensidade(Arv *raiz) {
    if (raiz == NULL) return;
    mostrarDensidade(raiz->esq);

    float densidade = (raiz->dados.area > 0.0f)
                      ? (float)raiz->dados.populacao / raiz->dados.area
                      : 0.0f;
    printf("  %-30s  %.2f hab/km²\n",
           raiz->dados.nome, densidade);

    mostrarDensidade(raiz->dir);
}

//Função auxiliar acumuladora para iv.
static void somarAreas(Arv *raiz, float *total) {
    if (raiz == NULL) return;
    *total += raiz->dados.area;
    somarAreas(raiz->esq, total);
    somarAreas(raiz->dir, total);
}

//iv. Somatório de área e percentual
void somatorioArea(Arv *raiz) {
    float total = 0.0f;
    somarAreas(raiz, &total);
    float percentual = (total / AREA_BRASIL) * 100.0f;
    printf("  Área total cadastrada : %.2f km²\n", total);
    printf("  Percentual do Brasil  : %.4f%%\n", percentual);
}

//v. Exibir em ordem alfabética com todos os dados
void mostrarEmOrdemCompleto(Arv *raiz) {
    if (raiz == NULL) return;
    mostrarEmOrdemCompleto(raiz->esq);

    Municipio *m = &raiz->dados;
    float densidade = (m->area > 0.0f)
                      ? (float)m->populacao / m->area
                      : 0.0f;

    printf("  Nome      : %s\n",       m->nome);
    printf("  Área      : %.2f km²\n", m->area);
    printf("  População : %d hab\n",   m->populacao);
    printf("  Densidade : %.2f hab/km²\n", densidade);
    printf("  %s\n", "--------------------------------------------");

    mostrarEmOrdemCompleto(raiz->dir);
}

//Auxiliar recursivo para vi.
static void buscarMaiorPop(Arv *raiz, Municipio *maior) {
    if (raiz == NULL) return;
    if (raiz->dados.populacao > maior->populacao)
        *maior = raiz->dados;
    buscarMaiorPop(raiz->esq, maior);
    buscarMaiorPop(raiz->dir, maior);
}

//vi. Cidade com maior população
Municipio cidadeMaiorPopulacao(Arv *raiz) {
    Municipio maior;
    memset(&maior, 0, sizeof(Municipio));

    if (raiz == NULL) return maior;

    maior = raiz->dados;          //semente: nó raiz
    buscarMaiorPop(raiz->esq, &maior);
    buscarMaiorPop(raiz->dir, &maior);
    return maior;
}
