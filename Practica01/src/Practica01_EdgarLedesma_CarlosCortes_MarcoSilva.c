#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h> // Necesario para INT_MAX
#include <pthread.h> // Necesario para la programación en hilos

#define MAX_VERTICES 100
#define NUM_THREADS 4 // Número de hilos


// Estructura para representar un vértice en el grafo
struct vertice {
  int id;
  int retraso;
};

// Estructura para representar una arista (conexión) entre vértices
struct arista {
  int origen;  // Identificador del vértice de origen
  int destino; // Identificador del arista de origen
  int peso;    // Distancia entre los vértices 
};

// Estructura para representar el grafo
struct grafica {
  int nvertices;
  struct vertice *vertices;
  struct arista *aristas;
};

// Función para inicializar el grafo con retrasos aleatorios
void inicio_grafica(struct grafica *g, int nvertices) {
  // Asignar el número de vértices al grafo
  g->nvertices = nvertices;
  
  // Asignar memoria para el array de vértices en el grafo
  g->vertices = malloc(sizeof(struct vertice) * nvertices);
  
  // Inicializar los vértices con valores predeterminados
  for (int i = 0; i < nvertices; i++) {
    g->vertices[i].id = i;       // Identificador único del vértice
    g->vertices[i].retraso = 0;  // Inicialmente, no hay retraso
  }
  // Asignar memoria para el array de aristas en el grafo
  g->aristas = malloc(sizeof(struct arista) * nvertices);

  // Inicializar la semilla para obtener números aleatorios diferentes en cada ejecución
  srand(time(NULL));
  // Crear aristas entre vértices consecutivos y asignar retrasos aleatorios
  for (int i = 0; i < nvertices - 1; i++) {
    g->aristas[i].origen = i;         // Identificador del vértice de origen
    g->aristas[i].destino = i + 1;    // Identificador del vértice de destino (conexión secuencial)
    g->aristas[i].peso = rand() % 1000 + 1;  // Generar un retraso aleatorio entre 1 y 1000
    g->vertices[i + 1].retraso = g->aristas[i].peso;  // Asignar el retraso al vértice de destino
  }
}

// Función para imprimir la información del grafo
void imprimir_grafica(struct grafica *g) {
  printf("Información del Grafo:\n");
  printf("Nodo (Vertice)      | Retraso | \n");
  printf("------------------------------\n");
  for (int i = 0; i < g->nvertices; i++) {
    printf("%5d (Vertice %3d) | %4d    |\n", i, g->vertices[i].id, g->vertices[i].retraso);
  }
  printf("\n");
}

// Algoritmo de Dijkstra para encontrar las distancias más cortas desde un vértice fuente
void dijkstra(struct grafica *g, int origen) {
  int distancias[MAX_VERTICES];   // Almacena las distancias más cortas desde el vértice fuente
  int predecesores[MAX_VERTICES]; // Almacena los predecesores en el camino más corto

  // Inicializar distancias y predecesores
  for (int i = 0; i < g->nvertices; i++) {
    distancias[i] = INT_MAX; // Inicializar todas las distancias a infinito
    predecesores[i] = -1;    // No hay predecesores inicialmente
  }
  distancias[origen] = 0; // La distancia desde el vértice fuente a sí mismo es 0

  // Cola de prioridad para almacenar vértices a explorar
  struct {
    int vertice;
    int distancia;
  } cola[MAX_VERTICES];

  int cabeza = 0;     // Puntero al inicio de la cola
  int cola_final = 0; // Puntero al final de la cola

  // Agregar el vértice origen a la cola
  cola[cola_final].vertice = origen;
  cola[cola_final].distancia = 0;
  cola_final++;

  // Mientras la cola no esté vacía
  while (cabeza < cola_final) {
    // Obtener el vértice con la distancia más corta
    struct vertice u = g->vertices[cola[cabeza].vertice];
    int distancia_u = cola[cabeza].distancia;
    cabeza++;

    // Visitar el vértice u
    for (int i = 0; i < g->nvertices; i++) {
      // Si la distancia a través de u es menor que la distancia actual
      if (distancia_u + g->aristas[u.id * g->nvertices + i].peso < distancias[i]) {
        // Actualizar la distancia
        distancias[i] = distancia_u + g->aristas[u.id * g->nvertices + i].peso;
        // Actualizar el predecesor
        predecesores[i] = u.id;

        // Agregar el vértice i a la cola
        if (cabeza < cola_final) {
          cola[cola_final].vertice = i;
          cola[cola_final].distancia = distancias[i];
          cola_final++;
        }
      }
    }
  }
}

// Función para calcular Dijkstra en una porción del grafo
void *dijkstra_thread(void *arg) {
    struct grafica *g = (struct grafica *)arg;
    // Realizar el cálculo de Dijkstra en su porción del grafo aquí
    // Utilizar g para acceder a la estructura de datos del grafo
    return NULL;
}

int main() {
  struct grafica g;
  inicio_grafica(&g, 20);
  imprimir_grafica(&g);

  // Crear hilos para realizar el cálculo de Dijkstra en paralelo
  pthread_t threads[NUM_THREADS];

  for (int i = 0; i < NUM_THREADS; i++) {
    if (pthread_create(&threads[i], NULL, dijkstra_thread, &g) != 0) {
      perror("Error al crear un hilo");
      exit(EXIT_FAILURE);
    }
  }

  // Esperar a que todos los hilos terminen
  for (int i = 0; i < NUM_THREADS; i++) {
    if (pthread_join(threads[i], NULL) != 0) {
      perror("Error al unirse a un hilo");
      exit(EXIT_FAILURE);
    }
  }

  // Liberar memoria asignada dinámicamente
  free(g.vertices);
  free(g.aristas);
  
  return 0;
}