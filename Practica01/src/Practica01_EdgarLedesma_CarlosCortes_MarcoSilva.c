#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#define MAX_VERTICES 100
#define INFINITY INT_MAX

// Estructura para representar una arista (conexión) entre vértices
struct arista {
    int destino;
    int peso;
    struct arista* siguiente;
};

// Estructura para representar un vértice en el grafo
struct vertice {
    int id;
    struct arista* adyacente;
};

// Estructura para representar el grafo
struct grafica {
    int nvertices;
    struct vertice vertices[MAX_VERTICES];
};

// Función para inicializar el grafo con retrasos aleatorios
void inicio_grafica(struct grafica* g, int nvertices) {
    // Asignar el número de vértices al grafo
    g->nvertices = nvertices;

    // Inicializar los vértices con valores predeterminados
    for (int i = 0; i < nvertices; i++) {
        g->vertices[i].id = i;
        g->vertices[i].adyacente = NULL;
    }

    // Inicializar la semilla para obtener números aleatorios diferentes en cada ejecución
    srand(time(NULL));

    // Inicializar las aristas con pesos aleatorios entre 1 y 1000
    for (int i = 0; i < nvertices; i++) {
        for (int j = 0; j < nvertices; j++) {
            if (i != j) {
                struct arista* nueva_arista = (struct arista*)malloc(sizeof(struct arista));
                nueva_arista->destino = j;
                nueva_arista->peso = rand() % 1000 + 1;
                nueva_arista->siguiente = g->vertices[i].adyacente;
                g->vertices[i].adyacente = nueva_arista;
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

// Función para imprimir la ruta desde el origen hasta un vértice dado
void imprimir_ruta(int predecesores[], int nodo) {
    if (predecesores[nodo] == -1) {
        printf("%d", nodo);
        return;
    }
    imprimir_ruta(predecesores, predecesores[nodo]);
    printf(" -> %d", nodo);
}

// Declaración de la función para imprimir el árbol generador
void imprimir_arbol_generador(int predecesores[], int nvertices);

// Algoritmo de Dijkstra distribuido
void dijkstra_distribuido(struct grafica* g, int origen) {
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

        // Actualizar las distancias de los vértices adyacentes a u
        struct arista* arista_actual = g->vertices[u].adyacente;
        while (arista_actual != NULL) {
            int v = arista_actual->destino;
            if (!conjunto_cerrado[v] && retrasos[u] != INFINITY && retrasos[u] + arista_actual->peso < retrasos[v]) {
                retrasos[v] = retrasos[u] + arista_actual->peso;
                predecesores[v] = u;
            }
            arista_actual = arista_actual->siguiente;
        }
    }

    printf("Nodo \t Retraso \t Ruta\n");
    for (int i = 0; i < nvertices; i++) {
        printf("%d \t %d \t\t ", i, retrasos[i]);
        imprimir_ruta(predecesores, i);
        printf("\n");
    }

    imprimir_arbol_generador(predecesores, nvertices);
}

// Definición de la función para imprimir el árbol generador
void imprimir_arbol_generador(int predecesores[], int nvertices) {
    printf("Arbol generador:\n");
    for (int i = 0; i < nvertices; i++) {
        if (predecesores[i] != -1) {
            printf("%d -> %d\n", predecesores[i], i);
        }
    }
}

int main() {
    struct grafica g;
    int nvertices = 2; // Cambia el número de vértices según tus necesidades
    inicio_grafica(&g, nvertices);
    int origen = 0; // Cambia el origen según tus necesidades

    dijkstra_distribuido(&g, origen);

    return 0;
}