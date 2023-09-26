#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h> 
#include <pthread.h>

#define MAX_VERTICES 100
#define NUM_THREADS 4

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
  struct vertice *vertices;
  struct arista *aristas;
  int *retrasos;  // Array para almacenar retrasos de comunicación
};

// Función para inicializar el grafo con retrasos aleatorios
void inicio_grafica(struct grafica *g, int nvertices) {
  g->nvertices = nvertices;
  g->vertices = malloc(sizeof(struct vertice) * nvertices);
  g->aristas = malloc(sizeof(struct arista) * nvertices);
  g->retrasos = malloc(sizeof(int) * nvertices);

  for (int i = 0; i < nvertices; i++) {
    g->vertices[i].id = i;
    g->retrasos[i] = rand() % 1000 + 1; // Generar retrasos aleatorios entre 1 y 1000
  }

  for (int i = 0; i < nvertices - 1; i++) {
    g->aristas[i].origen = i;
    g->aristas[i].destino = i + 1;
    g->aristas[i].peso = g->retrasos[i]; // Asignar retrasos como pesos de las aristas
  }
}

// Función para imprimir la información del grafo
void imprimir_grafica(struct grafica *g) {
  printf("Información del Grafo:\n");
  printf("Nodo (Vertice)      | Retraso | \n");
  printf("------------------------------\n");
  for (int i = 0; i < g->nvertices; i++) {
    printf("%5d (Vertice %3d) | %4d    |\n", i, g->vertices[i].id, g->retrasos[i]);
  }
  printf("\n");
}

// Algoritmo de Dijkstra para encontrar las distancias más cortas desde un vértice fuente
void dijkstra(struct grafica *g, int origen) {
  int distancias[MAX_VERTICES];   
  int predecesores[MAX_VERTICES]; 

  for (int i = 0; i < g->nvertices; i++) {
    distancias[i] = INT_MAX; 
    predecesores[i] = -1;    
  }
  distancias[origen] = 0; 

  struct {
    int vertice;
    int distancia;
  } cola[MAX_VERTICES];

  int cabeza = 0;     
  int cola_final = 0; 

  cola[cola_final].vertice = origen;
  cola[cola_final].distancia = 0;
  cola_final++;

  while (cabeza < cola_final) {
    struct vertice u = g->vertices[cola[cabeza].vertice];
    int distancia_u = cola[cabeza].distancia;
    cabeza++;

    for (int i = 0; i < g->nvertices; i++) {
      if (distancia_u + g->aristas[u.id * g->nvertices + i].peso < distancias[i]) {
        distancias[i] = distancia_u + g->aristas[u.id * g->nvertices + i].peso;
        predecesores[i] = u.id;

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
    // Debe calcular las rutas más cortas considerando los retrasos aleatorios
    dijkstra(g, 0); // Ejemplo: Calcular rutas desde el nodo 0
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
  free(g.retrasos);
  
  return 0;
}
