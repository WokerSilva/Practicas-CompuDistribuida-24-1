#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#define MAX_VERTICES 100
#define INFINITY INT_MAX

// Estructura para representar una arista (conexión) entre vértices
struct arista {
    int destino;          // Identificador del vértice de destino de la arista
    int peso;             // Peso o costo de la arista
    struct arista* siguiente;  // Puntero al siguiente nodo de arista (lista de adyacencia)
};

// Estructura para representar un vértice en el grafo
struct vertice {
    int id;               // Identificador único del vértice en el grafo
    struct arista* adyacente;  // Puntero al primer nodo de arista conectado a este vértice (lista de adyacencia)
};

// Estructura para representar el grafo
struct grafica {
    int nvertices;        // Número total de vértices en el grafo
    struct vertice vertices[MAX_VERTICES];  // Arreglo de vértices en el grafo
};

// Función para inicializar el grafo con retrasos aleatorios
void inicio_grafica(struct grafica* g, int nvertices){
    // Asignar el número de vértices al grafo
    g->nvertices = nvertices;

    // Inicializar los vértices con valores predeterminados
    for (int i = 0; i < nvertices; i++) {
        g->vertices[i].id = i;            // Asigna un identificador único al vértice
        g->vertices[i].adyacente = NULL;   // Inicializa la lista de adyacencia del vértice como vacía
    }

    // Inicializar la semilla para obtener números aleatorios diferentes en cada ejecución
    srand(time(NULL));

    // Inicializar las aristas con pesos aleatorios entre 1 y 1000
    for (int i = 0; i < nvertices; i++) {
        for (int j = 0; j < nvertices; j++) {
            if (i != j) {
                // Crea una nueva arista y asigna el destino y un peso aleatorio
                struct arista* nueva_arista = (struct arista*)malloc(sizeof(struct arista));
                nueva_arista->destino = j;
                nueva_arista->peso = rand() % 1000 + 1;
                
                // Agrega la nueva arista al inicio de la lista de adyacencia del vértice i
                nueva_arista->siguiente = g->vertices[i].adyacente;
                g->vertices[i].adyacente = nueva_arista;
            }
        }
    }
}

// Función para encontrar el vértice con la distancia mínima
int encontrar_minimo(int distancias[], bool conjunto_cerrado[], int nvertices){
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
void imprimir_ruta(int predecesores[], int nodo){
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
void dijkstra_distribuido(struct grafica* g, int origen){
    int nvertices = g->nvertices;
    int retrasos[MAX_VERTICES];         // Arreglo para almacenar las distancias mínimas desde el origen
    int predecesores[MAX_VERTICES];     // Arreglo para almacenar los predecesores en la ruta mínima
    bool conjunto_cerrado[MAX_VERTICES];// Arreglo para rastrear los vértices visitados

    // Inicializar todos los vértices con distancias infinitas, predecesores desconocidos y no visitados
    for (int i = 0; i < nvertices; i++) {
        retrasos[i] = INFINITY;         // Distancia inicialmente infinita
        predecesores[i] = -1;           // Predecesor desconocido
        conjunto_cerrado[i] = false;    // Ningún vértice visitado
    }

    retrasos[origen] = 0; // La distancia al origen es 0, ya que no se necesita moverse para alcanzarlo

    // Iterar para encontrar la ruta más corta a todos los vértices
    for (int count = 0; count < nvertices - 1; count++) {
        int u = encontrar_minimo(retrasos, conjunto_cerrado, nvertices); // Encontrar el vértice no visitado más cercano
        conjunto_cerrado[u] = true; // Marcar el vértice como visitado

        // Actualizar las distancias de los vértices adyacentes a u
        struct arista* arista_actual = g->vertices[u].adyacente;
        while (arista_actual != NULL) {
            int v = arista_actual->destino; // Vértice adyacente
            if (!conjunto_cerrado[v] && retrasos[u] != INFINITY && retrasos[u] + arista_actual->peso < retrasos[v]){
                // Si la distancia actual es mayor que la nueva distancia a través de u
                retrasos[v] = retrasos[u] + arista_actual->peso; // Actualizar la distancia mínima
                predecesores[v] = u; // Establecer a u como el predecesor en la ruta mínima
            }
            arista_actual = arista_actual->siguiente;
        }
    }

    // Imprimir las distancias mínimas y las rutas
    printf("Nodo \t Retraso \t Ruta\n");
    for (int i = 0; i < nvertices; i++) {
        printf("%d \t %d \t\t ", i, retrasos[i]);
        imprimir_ruta(predecesores, i); // Imprimir la ruta desde el origen hasta el vértice i
        printf("\n");
    }
    // Llamar a la función para imprimir el árbol generador
    imprimir_arbol_generador(predecesores, nvertices); 
}

// Definición de la función para imprimir el árbol generador
void imprimir_arbol_generador(int predecesores[], int nvertices){
    printf("Arbol generador:\n");
    for (int i = 0; i < nvertices; i++) {
        if (predecesores[i] != -1) {
            printf("%d -> %d\n", predecesores[i], i);
        }
    }
}

int main(){
    struct grafica g;
    int nvertices = 20; // Número de vértices
    inicio_grafica(&g, nvertices);
    int origen = 0; // Origen del vértice
    dijkstra_distribuido(&g, origen);

    return 0;
}