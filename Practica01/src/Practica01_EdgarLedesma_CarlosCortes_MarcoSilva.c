#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_VERTICES 100

struct vertice {
  int id;
  int delay;
};

struct arista {
  int src;
  int dst;
  int weight;
};

struct grafica {
  int nvertices;
  struct vertice *vertices;
  struct arista *aristas;
};

void inicio_grafica(struct grafica *g, int nvertices) {
  g->nvertices = nvertices;
  g->vertices = malloc(sizeof(struct vertice) * nvertices);
  for (int i = 0; i < nvertices; i++) {
    g->vertices[i].id = i;
    g->vertices[i].delay = 0;
  }
  g->aristas = malloc(sizeof(struct arista) * nvertices * nvertices);
  for (int i = 0; i < nvertices * nvertices; i++) {
    g->aristas[i].src = i / nvertices;
    g->aristas[i].dst = i % nvertices;
    g->aristas[i].weight = rand() % 1000 + 1;
  }
}

void print_grafica(struct grafica *g) {
  for (int i = 0; i < g->nvertices; i++) {
    printf("Vértice %d: %d, %d\n", g->vertices[i].id, g->vertices[i].delay, g->aristas[i].weight);
  }
}

void dijkstra(struct grafica *g, int src) {
 int visited[MAX_VERTICES] = {0};
    int i, j;

    for (i = 0; i < g->nvertices; i++) {
        distancia[i] = INT_MAX;
        predecessors[i] = -1;
    }

    distancia[src] = 0;

    for (i = 0; i < g->nvertices; i++) {
        int min_dist = INT_MAX;
        int u;

        for (j = 0; j < g->nvertices; j++) {
            if (!visited[j] && distancia[j] < min_dist) {
                min_dist = distancia[j];
                u = j;
            }
        }

        visited[u] = 1;

        for (j = 0; j < g->nvertices; j++) {
            if (!visited[j] && g->aristas[u][j] && (distancia[u] + g->aristas[u][j] < distancia[j])) {
                distancia[j] = distancia[u] + g->aristas[u][j];
                predecessors[j] = u;
            }
        }
    }
}


int main() {
  struct Graph grafo;
    grafo.nvertices = 5;

    int distancia[MAX_VERTICES];
    int predecessors[MAX_VERTICES];

    for (int i = 0; i < grafo.nvertices; i++) {
        for (int j = 0; j < grafo.nvertices; j++) {
            grafo.aristas[i][j] = 0;
        }
    }

    // Configurar las aristas del grafo
    // grafo.aristas[i][j] = peso;

    int src = 0;  // Cambia el origen según tu necesidad

    dijkstra(&grafo, src, distancia, predecessors);

    return 0;
}