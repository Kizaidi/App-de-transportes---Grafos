#ifndef ESTRUTURA_H
#define ESTRUTURA_H

#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

#define MAX_VERTICES 100
#define INFINITO INT_MAX

typedef struct {
    int **matrizAdj;
    int numVertices;
} Grafo;

typedef struct {
    int id;
    int x;
    int y;
} Vertice;

typedef struct {
    Vertice *vertices[MAX_VERTICES];
    int numVertices;
} ListaVertices;

typedef struct {
    int id;
    Vertice *localizacao;
} Taxi;

typedef struct {
    int id;
    Vertice *localizacao;
} Usuario;

typedef struct {
    Usuario *usuarios[MAX_VERTICES];
    int numUsuarios;
    Taxi *taxis[MAX_VERTICES];
    int numTaxis;
} ListaUsuarioTaxi;

Grafo *criarGrafo(int numVertices) {
    Grafo *grafo = (Grafo *)malloc(sizeof(Grafo));
    grafo->numVertices = numVertices;
    grafo->matrizAdj = (int **)malloc(numVertices * sizeof(int *));

    for (int i = 0; i < numVertices; i++) {
        grafo->matrizAdj[i] = (int *)malloc(numVertices * sizeof(int));
        for (int j = 0; j < numVertices; j++) {
            grafo->matrizAdj[i][j] = (i == j) ? 0 : INFINITO;
        }
    }

    return grafo;
}

void adicionarAresta(Grafo *grafo, int inicio, int fim, int peso) {
    if (inicio >= 0 && inicio < grafo->numVertices && fim >= 0 && fim < grafo->numVertices) {
        grafo->matrizAdj[inicio][fim] = peso;
        grafo->matrizAdj[fim][inicio] = peso; // Grafo não direcionado
    }
}

void removerAresta(Grafo *grafo, int inicio, int fim) {
    if (inicio >= 0 && inicio < grafo->numVertices && fim >= 0 && fim < grafo->numVertices) {
        grafo->matrizAdj[inicio][fim] = INFINITO;
        grafo->matrizAdj[fim][inicio] = INFINITO; // Grafo não direcionado
    }
}

ListaUsuarioTaxi *criarListaUsuarioTaxi() {
    ListaUsuarioTaxi *lista = (ListaUsuarioTaxi *)malloc(sizeof(ListaUsuarioTaxi));
    lista->numUsuarios = 0;
    lista->numTaxis = 0;
    return lista;
}

void adicionarUsuario(ListaUsuarioTaxi *lista, int id, Vertice *localizacao) {
    if (lista->numUsuarios < MAX_VERTICES) {
        Usuario *usuario = (Usuario *)malloc(sizeof(Usuario));
        usuario->id = id;
        usuario->localizacao = localizacao;
        lista->usuarios[lista->numUsuarios++] = usuario;
    }
}

void adicionarTaxi(ListaUsuarioTaxi *lista, int id, Vertice *localizacao) {
    if (lista->numTaxis < MAX_VERTICES) {
        Taxi *taxi = (Taxi *)malloc(sizeof(Taxi));
        taxi->id = id;
        taxi->localizacao = localizacao;
        lista->taxis[lista->numTaxis++] = taxi;
    }
}

ListaVertices *criarListaVertices() {
    ListaVertices *lista = (ListaVertices *)malloc(sizeof(ListaVertices));
    lista->numVertices = 0;
    return lista;
}

void adicionarVertice(ListaVertices *lista, int id, int x, int y) {
    if (lista->numVertices < MAX_VERTICES) {
        Vertice *vertice = (Vertice *)malloc(sizeof(Vertice));
        vertice->id = id;
        vertice->x = x;
        vertice->y = y;
        lista->vertices[lista->numVertices++] = vertice;
    }
}

int calcularDistancia(Vertice *vertice1, Vertice *vertice2) {
    // Exemplo simplificado: Distância euclidiana
    return abs(vertice1->x - vertice2->x) + abs(vertice1->y - vertice2->y);
}

void chamarTaxi(ListaUsuarioTaxi *listaUsuariosTaxis, Taxi *taxi, Vertice *destino) {
    int custo = calcularDistancia(listaUsuariosTaxis->usuarios[taxi->id]->localizacao, destino) * 2; // Exemplo de cálculo de custo
    printf("Custo estimado da viagem: %d\n", custo);

    // Atualizar a localização do táxi para o destino
    listaUsuariosTaxis->usuarios[taxi->id]->localizacao = destino;
    printf("Localização atual do táxi: (%d, %d)\n", destino->x, destino->y);
}

void encontrarTaxiMaisProximo(ListaUsuarioTaxi *listaUsuariosTaxis, Vertice *localUsuario, Vertice *destino) {
    Taxi *taxiMaisProximo = NULL;
    int menorDistancia = INFINITO;

    for (int i = 0; i < listaUsuariosTaxis->numTaxis; i++) {
        Taxi *taxi = listaUsuariosTaxis->taxis[i];
        int distancia = calcularDistancia(listaUsuariosTaxis->usuarios[taxi->id]->localizacao, localUsuario);
        if (distancia < menorDistancia) {
            menorDistancia = distancia;
            taxiMaisProximo = taxi;
        }
    }

    Vertice *localizacaoTaxi = listaUsuariosTaxis->usuarios[taxiMaisProximo->id]->localizacao;
    printf("Taxi mais próximo está localizado em (%d, %d)\n", localizacaoTaxi->x, localizacaoTaxi->y);

    // Simulando chamada do táxi após encontrar o mais próximo
    chamarTaxi(listaUsuariosTaxis, taxiMaisProximo, destino);
}

void imprimirCaminho(int caminho[], int n) {
    for (int i = 0; i < n; i++) {
        printf("%d ", caminho[i]);
    }
    printf("\n");
}

void imprimirTodosCaminhosAux(Grafo *grafo, int u, int d, bool visitado[], int caminho[], int *index) {
    visitado[u] = true;
    caminho[(*index)++] = u;

    if (u == d) {
        imprimirCaminho(caminho, *index);
    } else {
        for (int i = 0; i < grafo->numVertices; i++) {
            if (!visitado[i] && grafo->matrizAdj[u][i] != INFINITO) {
                imprimirTodosCaminhosAux(grafo, i, d, visitado, caminho, index);
            }
        }
    }

    (*index)--;
    visitado[u] = false;
}

void imprimirTodosCaminhos(Grafo *grafo, int s, int d) {
    bool *visitado = (bool *)malloc(grafo->numVertices * sizeof(bool));
    int *caminho = (int *)malloc(grafo->numVertices * sizeof(int));
    int index = 0;

    for (int i = 0; i < grafo->numVertices; i++) {
        visitado[i] = false;
    }

    imprimirTodosCaminhosAux(grafo, s, d, visitado, caminho, &index);

    free(visitado);
    free(caminho);
}

int minDistancia(int dist[], bool sptSet[], int V) {
    int min = INFINITO, min_index;

    for (int v = 0; v < V; v++) {
        if (sptSet[v] == false && dist[v] <= min) {
            min = dist[v], min_index = v;
        }
    }

    return min_index;
}

void dijkstra(Grafo *grafo, int src, int dest) {
    int V = grafo->numVertices;
    int dist[V];
    bool sptSet[V];
    int parent[V];

    for (int i = 0; i < V; i++) {
        parent[src] = -1;
        dist[i] = INFINITO;
        sptSet[i] = false;
    }

    dist[src] = 0;

    for (int count = 0; count < V - 1; count++) {
        int u = minDistancia(dist, sptSet, V);
        sptSet[u] = true;

        for (int v = 0; v < V; v++) {
            if (!sptSet[v] && grafo->matrizAdj[u][v] && dist[u] != INFINITO && dist[u] + grafo->matrizAdj[u][v] < dist[v]) {
                parent[v] = u;
                dist[v] = dist[u] + grafo->matrizAdj[u][v];
            }
        }
    }

    printf("Melhor caminho de %d até %d com custo %d: ", src, dest, dist[dest]);
    int crawl = dest;
    while (parent[crawl] != -1) {
        printf("%d <- ", crawl);
        crawl = parent[crawl];
    }
    printf("%d\n", src);
}

void carregarGrafoDeArquivo(Grafo *grafo, const char *nomeArquivo) {
    FILE *arquivo = fopen(nomeArquivo, "r");
    if (!arquivo) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }
    int V, E;
    fscanf(arquivo, "%d %d", &V, &E);
    for (int i = 0; i < E; i++) {
        int inicio, fim, peso;
        fscanf(arquivo, "%d %d %d", &inicio, &fim, &peso);
        adicionarAresta(grafo, inicio, fim, peso);
    }
    fclose(arquivo);
}

#endif