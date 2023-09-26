#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h> // Necesario para generar números aleatorios

#define MAX_VERTICES 100
#define INFINITY INT_MAX
int predecesores[MAX_VERTICES];
int nvertices;


// Estructura para representar un vértice en el grafo
struct vertice {
    int id;
};

// Estructura para representar una arista (conexión) entre vértices
struct arista {
    int origen;
    int destino;
    int peso;
};

// Estructura para representar el grafo
struct grafica {
    int nvertices;
    struct vertice vertices[MAX_VERTICES];
    struct arista aristas[MAX_VERTICES * MAX_VERTICES];
};

// Función para inicializar el grafo con retrasos aleatorios
void inicio_grafica(struct grafica *g, int nvertices) {
    // Asignar el número de vértices al grafo
    g->nvertices = nvertices;

    // Inicializar los vértices con valores predeterminados
    for (int i = 0; i < nvertices; i++) {
        g->vertices[i].id = i;
    }

    // Inicializar la semilla para obtener números aleatorios diferentes en cada ejecución
    srand(time(NULL));

    // Inicializar las aristas con pesos aleatorios entre 1 y 1000
    for (int i = 0; i < nvertices; i++) {
        for (int j = 0; j < nvertices; j++) {
            if (i != j) {
                g->aristas[i * nvertices + j].origen = i;
                g->aristas[i * nvertices + j].destino = j;
                g->aristas[i * nvertices + j].peso = rand() % 1000 + 1;
            }
        }
    }
}

// Función para encontrar el vértice con la distancia mínima
int encontrar_minimo(int distancias[], bool conjunto_cerrado[], int nvertices) {
    int minimo = INFINITY;
    int minimo_indice = -1;

    for (int v = 0; v < nvertices; v++) {
        if (!conjunto_cerrado[v] && distancias[v] < minimo) {
            minimo = distancias[v];
            minimo_indice = v;
        }
    }

    return minimo_indice;
}

// Función para imprimir los retrasos mínimos y predecesores
void imprimir_resultados(int distancias[], int predecesores[], int nvertices) {
    printf("Nodo \t Retraso \t Ruta\n");
    for (int i = 0; i < nvertices; i++) {
        printf("%d \t %d \t\t ", i, distancias[i]);
        int nodo_actual = i;
        while (nodo_actual != -1) {
            printf("%d ", nodo_actual);
            nodo_actual = predecesores[nodo_actual];
        }
        printf("\n");
    }
}

// Algoritmo de Dijkstra distribuido
void dijkstra_distribuido(struct grafica *g, int origen) {
    int nvertices = g->nvertices;
    int retrasos[MAX_VERTICES];
    int predecesores[MAX_VERTICES];
    bool conjunto_cerrado[MAX_VERTICES];

    for (int i = 0; i < nvertices; i++) {
        retrasos[i] = INFINITY;
        predecesores[i] = -1;
        conjunto_cerrado[i] = false;
    }

    retrasos[origen] = 0;

    for (int count = 0; count < nvertices - 1; count++) {
        int u = encontrar_minimo(retrasos, conjunto_cerrado, nvertices);
        conjunto_cerrado[u] = true;

        for (int v = 0; v < nvertices; v++) {
            if (!conjunto_cerrado[v] && g->aristas[u * nvertices + v].peso &&
                retrasos[u] != INFINITY && retrasos[u] + g->aristas[u * nvertices + v].peso < retrasos[v]) {
                retrasos[v] = retrasos[u] + g->aristas[u * nvertices + v].peso;
                predecesores[v] = u;
            }
        }
    }

    imprimir_resultados(retrasos, predecesores, nvertices);
}

// Función para imprimir la ruta que sigue el algoritmo
void imprimir_ruta(int predecesores[], int nvertices) {
    printf("\nRuta:\n");
    for (int i = 0; i < nvertices; i++) {
        int nodo_actual = i;
        while (nodo_actual != -1) {
            printf("%d ", nodo_actual);
            nodo_actual = predecesores[nodo_actual];
        }
        printf("\n");
    }    
}

int main() {
    struct grafica g;
    inicio_grafica(&g, 20);
    imprimir_ruta(predecesores, nvertices);
    int origen = 0; // Cambia el origen según tus necesidades
    dijkstra_distribuido(&g, origen);
    
    return 0;
}
