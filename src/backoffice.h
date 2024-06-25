#ifndef BACKOFFICE_H
#define BACKOFFICE_H
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include "Estrutura.h"



void adicionarNovaRua(Grafo *grafo) {
    int inicio, fim, peso;
    printf("Digite o id do vertice de inicio: ");
    scanf("%d", &inicio);
    printf("Digite o id do vertice de fim: ");
    scanf("%d", &fim);
    printf("Digite o peso da aresta (distancia): ");
    scanf("%d", &peso);

    adicionarAresta(grafo, inicio, fim, peso);
    printf("Rua adicionada com sucesso!\n");
}

void removerRua(Grafo *grafo) {
    int inicio, fim;
    printf("Digite o id do vertice de inicio: ");
    scanf("%d", &inicio);
    printf("Digite o id do vertice de fim: ");
    scanf("%d", &fim);

    removerAresta(grafo, inicio, fim);
    printf("Rua removida com sucesso!\n");
}

void adicionarBuracos(Grafo *grafo) {
    int inicio, fim;
    printf("Digite o id do vertice de inicio: ");
    scanf("%d", &inicio);
    printf("Digite o id do vertice de fim: ");
    scanf("%d", &fim);

    grafo->matrizAdj[inicio][fim] = -1;
    grafo->matrizAdj[fim][inicio] = -1;
    printf("Buracos adicionados na rua com sucesso!\n");
}

void removerBuracos(Grafo *grafo, int pesoOriginal) {
    int inicio, fim;
    printf("Digite o id do vertice de inicio: ");
    scanf("%d", &inicio);
    printf("Digite o id do vertice de fim: ");
    scanf("%d", &fim);

    grafo->matrizAdj[inicio][fim] = pesoOriginal;
    grafo->matrizAdj[fim][inicio] = pesoOriginal;
    printf("Buracos removidos da rua com sucesso!\n");
}

void imprimirLocalidade(Grafo *grafo) {
    printf("Localidade (Grafo de ruas):\n");
    for (int i = 0; i < grafo->numVertices; i++) {
        for (int j = 0; j < grafo->numVertices; j++) {
            if (grafo->matrizAdj[i][j] != INFINITO && grafo->matrizAdj[i][j] != 0) {
                printf("Rua de %d para %d com peso %d\n", i, j, grafo->matrizAdj[i][j]);
            }
        }
    }
}

void imprimirTodosCaminhosSemBuracosAux(Grafo *grafo, int u, int d, bool visitado[], int caminho[], int *index) {
    visitado[u] = true;
    caminho[(*index)++] = u;

    if (u == d) {
        imprimirCaminho(caminho, *index);
    } else {
        for (int i = 0; i < grafo->numVertices; i++) {
            if (!visitado[i] && grafo->matrizAdj[u][i] != INFINITO && grafo->matrizAdj[u][i] != -1) {
                imprimirTodosCaminhosSemBuracosAux(grafo, i, d, visitado, caminho, index);
            }
        }
    }

    (*index)--;
    visitado[u] = false;
}

void imprimirTodosCaminhosSemBuracos(Grafo *grafo, int s, int d) {
    bool *visitado = (bool *)malloc(grafo->numVertices * sizeof(bool));
    int *caminho = (int *)malloc(grafo->numVertices * sizeof(int));
    int index = 0;

    for (int i = 0; i < grafo->numVertices; i++) {
        visitado[i] = false;
    }

    imprimirTodosCaminhosSemBuracosAux(grafo, s, d, visitado, caminho, &index);

    free(visitado);
    free(caminho);
}

void cadastrarTaxistas(ListaUsuarioTaxi *listaUsuariosTaxis, ListaVertices *listaVertices) {
    int id, verticeId;
    printf("Digite o id do taxista: ");
    scanf("%d", &id);
    printf("Digite o id do vertice onde o taxista está localizado: ");
    scanf("%d", &verticeId);

    if (verticeId >= 0 && verticeId < listaVertices->numVertices) {
        adicionarTaxi(listaUsuariosTaxis, id, listaVertices->vertices[verticeId]);
        printf("Taxista cadastrado com sucesso!\n");
    } else {
        printf("Vertice invalido.\n");
    }
}

#endif