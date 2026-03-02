#include <stdio.h>
#include <stdlib.h>

#include "municipio.h"
#include "avl.h"

//Utilitário para criar um Municipio inline
static Municipio fazerMunicipio(const char *nome, float area, int populacao) {
    Municipio m;
    int i;
    //copia nome manualmente para evitar dependência de strncpy
    for (i = 0; i < 99 && nome[i] != '\0'; i++)
    m.nome[i] = nome[i];
    m.nome[i] = '\0';
    m.area = area;
    m.populacao = populacao;
    return m;
}

//Imprime separador decorativo
static void separador(const char *titulo) {
    printf("\n╔══════════════════════════════════════════════════════╗\n");
    printf("║  %-52s║\n", titulo);
    printf("╚══════════════════════════════════════════════════════╝\n");
}

int main(void) {

    //1. Criar árvore
    Arv *raiz = criarArvore();

    //2. Cadastrar 10 municípios
    //                                  nome                 área(km²) população
    raiz = inserir(raiz, fazerMunicipio("Uberlândia",        4115.0f,  706597));
    raiz = inserir(raiz, fazerMunicipio("São Paulo",        1521.0f, 12325232));
    raiz = inserir(raiz, fazerMunicipio("Rio de Janeiro",   1200.0f,  6747815));
    raiz = inserir(raiz, fazerMunicipio("Belo Horizonte",    330.9f,  2722000));
    raiz = inserir(raiz, fazerMunicipio("Manaus",          11401.0f,  2255903));
    raiz = inserir(raiz, fazerMunicipio("Curitiba",          435.2f,  1963726));
    raiz = inserir(raiz, fazerMunicipio("Fortaleza",         313.8f,  2703391));
    raiz = inserir(raiz, fazerMunicipio("Recife",            218.4f,  1660786));
    raiz = inserir(raiz, fazerMunicipio("Porto Alegre",      496.7f,  1488252));
    raiz = inserir(raiz, fazerMunicipio("Goiânia",           739.5f,  1555626));
    raiz = inserir(raiz, fazerMunicipio("Belém",             1065.0f, 1499641));
    raiz = inserir(raiz, fazerMunicipio("Ituiutaba",         2598.0f,  107339));
    raiz = inserir(raiz, fazerMunicipio("Araguari",          2739.0f,  117518));

    //Tentativa de inserção duplicada
    raiz = inserir(raiz, fazerMunicipio("Uberlândia", 4115.0f, 706597));

    //3. Testar funcionalidades

    //i. Contar municípios
    separador("i. Total de municípios cadastrados");
    printf("  Total: %d município(s)\n", contarMunicipios(raiz));

    //i. Municípios com população > 1.000.000
    separador("ii. Municípios com população > 1.000.000");
    mostrarPopulacaoMaiorQue(raiz, 1000000);

    //iii. Densidade demográfica
    separador("iii. Densidade demográfica (hab/km²)");
    mostrarDensidade(raiz);

    //iv. Somatório de área
    separador("iv. Somatório de área e percentual do Brasil");
    somatorioArea(raiz);

    //v. Lista completa em ordem alfabética
    separador("v. Municípios em ordem alfabética (dados completos)");
    mostrarEmOrdemCompleto(raiz);

    //vi. Cidade com maior população
    separador("vi. Município com maior população");
    Municipio maior = cidadeMaiorPopulacao(raiz);
    if (maior.nome[0] != '\0') {
        float densidade = (maior.area > 0.0f)
                          ? (float)maior.populacao / maior.area
                          : 0.0f;
        printf("  Nome      : %s\n",       maior.nome);
        printf("  Área      : %.2f km²\n", maior.area);
        printf("  População : %d hab\n",   maior.populacao);
        printf("  Densidade : %.2f hab/km²\n", densidade);
    }

    //Lista simples em ordem — função básica
    separador("Percurso em ordem (nomes apenas)");
    emOrdem(raiz);

    //4. Liberar memória
    liberarArvore(raiz);
    raiz = NULL;

    printf("\nMemória liberada com sucesso.\n\n");
    return EXIT_SUCCESS;
}
