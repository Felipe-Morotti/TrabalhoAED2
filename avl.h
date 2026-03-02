#ifndef AVL_H
#define AVL_H
#include "municipio.h"

//Nó da árvore
typedef struct NO {
    Municipio   dados;
    int         altura;
    struct NO  *esq;
    struct NO  *dir;
} NO;

//Alias para ponteiro raiz
typedef NO Arv;

//Funções Básicas

//Cria/inicializa uma árvore vazia (retorna NULL)
Arv *criarArvore(void);

/* Insere um município na AVL (ordenado pelo nome).
 * Retorna a nova raiz após possíveis rotações. */
Arv *inserir(Arv *raiz, Municipio m);

//Percurso em ordem: lista municípios em ordem alfabética
void emOrdem(Arv *raiz);

//Libera toda a memória alocada
void liberarArvore(Arv *raiz);

//Funcionalidades

//i.  Conta o número total de municípios cadastrados
int contarMunicipios(Arv *raiz);

//ii. Exibe municípios com população > X
void mostrarPopulacaoMaiorQue(Arv *raiz, int X);

//iii. Exibe nome e densidade demográfica de cada cidade
void mostrarDensidade(Arv *raiz);

//iv. Soma das áreas e percentual em relação ao Brasil
void somatorioArea(Arv *raiz);

//v.  Exibe todas as cidades em ordem alfabética (dados completos)
void mostrarEmOrdemCompleto(Arv *raiz);

//vi. Retorna o município com maior população
Municipio cidadeMaiorPopulacao(Arv *raiz);

#endif
