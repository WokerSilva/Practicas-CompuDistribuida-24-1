#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>

#define MAX_VERTICES 100

// Estructura para representar un vértice en el grafo
struct vertice {
    int id;     // Identificador único del vértice
    int delay;  // Retardo asociado al vértice (no utilizado en el algoritmo Dijkstra)
};

// Estructura para representar una arista (conexión) entre vértices
struct arista {
    int src;    // Identificador del vértice de origen
    int dst;    // Identificador del vértice de destino
    int weight; // Peso o costo de la arista (distancia entre los vértices)
};

// Estructura para representar el grafo
struct grafica {
    int nvertices;        // Número de vértices en el grafo
    struct vertice *vertices; // Array de vértices
    struct arista *aristas;  // Array de aristas (conexiones entre vértices)
};

// Función para inicializar el grafo
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
        g->aristas[i].weight = rand() % 1000 + 1; // Generar pesos aleatorios para las aristas
    }
}

// Función para imprimir la información del grafo
void imprimir_grafica(struct grafica *g) {
    for (int i = 0; i < g->nvertices; i++) {
        printf("Vértice %d: %d, %d\n", g->vertices[i].id, g->vertices[i].delay, g->aristas[i].weight);
    }
}

// Algoritmo de Dijkstra para encontrar las distancias más cortas desde un vértice fuente
void dijkstra(struct grafica *g, int src) {
    int distancias[MAX_VERTICES];  // Almacena las distancias más cortas desde el vértice fuente
    int predecesores[MAX_VERTICES]; // Almacena los predecesores en el camino más corto

    // Inicializar distancias y predecesores
    for (int i = 0; i < g->nvertices; i++) {
        distancias[i] = INT_MAX;   // Inicializar todas las distancias a infinito
        predecesores[i] = -1;      // No hay predecesores inicialmente
    }
    distancias[src] = 0; // La distancia desde el vértice fuente a sí mismo es 0

    // Cola de prioridad para almacenar vértices a explorar
    struct {
        int vertice;
        int distancia;
    } cola[MAX_VERTICES];
    int cabeza = 0; // Puntero al inicio de la cola
    int tail = 0;   // Puntero al final de la cola

    // Agregar el vértice origen a la cola
    cola[tail].vertice = src;
    cola[tail].distancia = 0;
    tail++;

    // Mientras la cola no esté vacía
    while (cabeza < tail) {
        // Obtener el vértice con la distancia más corta de la cola
        struct vertice u = g->vertices[cola[cabeza].vertice];
        int u_distancia = cola[cabeza].distancia;
        cabeza++;

        // Visitar los vértices adyacentes a u
        for (int i = 0; i < g->nvertices; i++) {
            // Si la distancia a través de u es menor que la distancia actual
            int peso = g->aristas[u.id * g->nvertices + i].weight;
            if (u_distancia + peso < distancias[i]) {
                // Actualizar la distancia más corta
                distancias[i] = u_distancia + peso;
                // Actualizar el predecesor en el camino más corto
                predecesores[i] = u.id;

                // Agregar el vértice i a la cola para explorar sus adyacentes
                cola[tail].vertice = i;
                cola[tail].distancia = distancias[i];
                tail++;
            }
        }
    }

    // Imprimir las distancias más cortas desde el vértice fuente a todos los demás vértices
    for (int i = 0; i < g->nvertices; i++) {
        printf("Vértice %d: %d\n", i, distancias[i]);
    }
}

int main() {
    struct grafica g;
    inicio_grafica(&g, 10); // Inicializar un grafo con 10 vértices
    imprimir_grafica(&g);   // Imprimir información del grafo
    dijkstra(&g, 0);        // Ejecutar el algoritmo de Dijkstra desde el vértice 0

    // Liberar memoria
    free(g.vertices);
    free(g.aristas);

    return 0;
}