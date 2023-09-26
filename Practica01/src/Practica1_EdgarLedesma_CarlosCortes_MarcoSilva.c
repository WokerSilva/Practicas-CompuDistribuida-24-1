//Trabajo realizado por:

//Edgar Montiel Ledesma 
//Marco Antonio Rivera Silva 
//Carlos Daniel Cortes Jimenez 

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>
#include <mpi.h>

#define INFINITY INT_MAX

//Funcion que simula los retrasos tomando números aleatorios entre 1 y 1000
int randomDelay() {
    return rand() % 1000 + 1;
}

// Función para encontrar el nodo que no ha visitado con el retraso mínimo
int minDelay(int delay[], int visited[], int n) {
    int min = INFINITY;
    int minIndex = -1;

    // Busca el nodo con el retraso mínimo que aún no ha sido visitado
    for (int v = 0; v < n; v++) {
        if (!visited[v] && delay[v] < min) {
            min = delay[v];
            minIndex = v;
        }
    }

    return minIndex;
}

// Imprime los resultados del algoritmo Dijkstra
void printDijkstra(int delay[], int parent[], int n) {
    printf("Nodo\tRetraso\tRuta desde 0\n");
    for (int i = 1; i < n; i++) {
        printf("%d\t%d\t%d ", i, delay[i], i);
        int j = i;
        while (parent[j] != -1) {
            printf("<-- %d ", parent[j]);
            j = parent[j];
        }
        printf("\n");
    }
}

// Función principal que realiza el algoritmo de Dijkstra
void dijkstra(int **graph, int n, int rank, int size) {
    int *delay = (int *)malloc(n * sizeof(int));
    int *parent = (int *)malloc(n * sizeof(int));
    int *visited = (int *)malloc(n * sizeof(int));

    //Se inicializan los arreglos de distancia, padres y visitados
    for (int i = 0; i < n; i++) {
        delay[i] = INFINITY;
        visited[i] = 0;
        parent[i] = -1;
    }

    delay[0] = 0;

    // Algoritmo de Dijkstra
    for (int count = 0; count < n - 1; count++) {
        int u = minDelay(delay, visited, n);

        // Se distribuye el trabajo entre los procesos utilizando OpenMPI
        int chunk_size = n / size;
        int start = rank * chunk_size;
        int end = (rank == size - 1) ? n : start + chunk_size;

        // Calculamos las distancias parciales
        for (int v = start; v < end; v++) {
            if (!visited[v] && graph[u][v] && delay[u] != INFINITY) {
                int newDelay = delay[u] + graph[u][v];
                if (newDelay < delay[v]) {
                    delay[v] = newDelay;
                    parent[v] = u;
                }
            }
        }

        // Sincronizamos los resultados entre los procesos
        MPI_Allreduce(MPI_IN_PLACE, delay, n, MPI_INT, MPI_MIN, MPI_COMM_WORLD);
        MPI_Bcast(parent, n, MPI_INT, 0, MPI_COMM_WORLD);

        visited[u] = 1;
    }
    // El proceso con rango 0 imprime los resultados
    if (rank == 0) {
        printDijkstra(delay, parent, n);
    }
    // Liberamos memoria
    free(delay);
    free(parent);
    free(visited);
}

int main(int argc, char *argv[]) {
    srand(time(NULL));

    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int n;
    if (rank == 0) {
        printf("Ingrese el número de nodos: ");
        scanf("%d", &n);
    }

    // Hacemos broadcast del número de nodos a todos los procesos
    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

    int **graph = (int **)malloc(n * sizeof(int *));
    for (int i = 0; i < n; i++) {
        graph[i] = (int *)malloc(n * sizeof(int));
    }

    // Se Genera el grafo aleatorio en el proceso con rango 0
    if (rank == 0) {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (i == j) {
                    graph[i][j] = 0;
                } else {
                    graph[i][j] = randomDelay();
                }
            }
        }
    }

    // Hacemos broadcast del grafo a todos los procesos
    MPI_Bcast(&(graph[0][0]), n * n, MPI_INT, 0, MPI_COMM_WORLD);

    // Llamamos a la funcion de Dijkstra
    dijkstra(graph, n, rank, size);

    for (int i = 0; i < n; i++) {
        free(graph[i]);
    }
    free(graph);

    MPI_Finalize();

    return 0;
}