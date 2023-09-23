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
  int distancias[MAX_VERTICES];
  int predecesores[MAX_VERTICES];

  // Inicializar distancias y predecesores
  for (int i = 0; i < g->nvertices; i++) {
    distancias[i] = INT_MAX;
    predecesores[i] = -1;
  }
  distancias[src] = 0;

  // Cola de prioridad
  struct {
    int vertice;
    int distancia;
  } queue[MAX_VERTICES];
  int cabeza = 0;
  int tail = 0;

  // Agregar el vértice origen a la cola
  queue[tail++] = (struct {
    vertice: src,
    distancia: 0,
  });

  // Mientras la cola no esté vacía
  while (cabeza < tail) {
    // Obtener el vértice con la distancia más corta
    struct vertice u = queue[cabeza++];

    // Visitar el vértice u
    for (int i = 0; i < g->nvertices; i++) {
      // Si la distancia a través de u es menor que la distancia actual
      if (distancias[u.vertice] + g->aristas[u.vertice * g->nvertices + i].weight < distancias[i]) {
        // Actualizar la distancia
        distancias[i] = distancias[u.vertice] + g->aristas[u.vertice * g->nvertices + i].weight;
        // Actualizar el predecesor
        predecesores[i] = u.vertice;

        // Agregar el vértice i a la cola
        queue[tail++] = (struct {
          vertice: i,
          distancia: distancias[i],
        });
      }
    }
  }

  // Imprimir resultados
  for (int i = 0; i < g->nvertices; i++) {
    printf("Vértice %d: %d\n", i, distancias[i]);
  }
}

int main() {
  struct grafica g;
  inicio_grafica(&g, 10);
  print_grafica(&g);
  dijkstra(&g, 0);
  return 0;
}