//Marco Silva Huerta 316205326
//Edgar Montiel Ledesma 317317794
//Carlos Daniel Cortes Jimenez 420004846

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>

#define NUM_GENERALES 10  // Número total de generales
#define NUM_TRAIDORES 3   // Número de nodos traidores
#define MAX_RONDAS 100    // Número máximo de rondas antes de declarar un fracaso
#define F 1               // Número de generales traidores tolerados

// Estructura de un nodo
typedef struct {
    int id;
    int es_traidor;   
    int voto;  // 0 para retirada, 1 para ataque
    int mensaje;
     int estrategia;  // Estrategia de voto de los generales (0 para retirada, 1 para ataque)  
} General;

// Función para determinar si la votación es válida
int esVotacionValida(General generales[], int rank, int size) {
    int votos_ataque = 0;
    int votos_retirada = 0;

    // Esta estructura for se utiliza para contar los votos a favor de cada opción, ataque o retirada.
    for (int i = 0; i < NUM_GENERALES; i++) {
        // Si el general no es traidor, se incrementa el contador de votos correspondiente a la opción que votó.
        if (!generales[i].es_traidor) {
            if (generales[i].voto == 1) {
                votos_ataque++;
            } else {
                votos_retirada++;
            }
        }
    }

    int mayoria_requerida = (NUM_GENERALES / 2) + F;

    return (votos_ataque >= mayoria_requerida || votos_retirada >= mayoria_requerida);
}

// Función para realizar una ronda de comunicación
void realizarRonda(General generales[], int rank, int size) {
    // Itera a través de cada general en el arreglo generales.
    for (int i = 0; i < NUM_GENERALES; i++) {
        // Si el general no es el que está ejecutando la función, elige un voto aleatorio.
        if (i != rank) {
            generales[i].voto = rand() % 2;
        } else {
            // Si el general es el que está ejecutando la función, no vota por sí mismo.
            generales[i].voto = -1;
        }
        // Asigna el voto aleatorio al mensaje del general.
        generales[i].mensaje = generales[i].voto;
    }

    // Comunicación entre generales usando Broadcast
    MPI_Bcast(generales, NUM_GENERALES * sizeof(General), MPI_BYTE, 0, MPI_COMM_WORLD);
}


int elegirRey(General generales[], int rank, int size) {
    // Inicializa el vector `generales_no_traidores` con todos los generales no traidores.
    // Declaración de un arreglo para almacenar los índices de generales no traidores.
    int generales_no_traidores[NUM_GENERALES];
    // Inicialización de una variable para rastrear el número de generales no traidores encontrados.
    int num_generales_no_traidores = 0;

    // Iteración a través de todos los generales (índices del 0 al NUM_GENERALES - 1).
    for (int i = 0; i < NUM_GENERALES; i++) {
        // Verifica si el general en la posición 'i' no es un traidor.
        if (!generales[i].es_traidor) {
            // Si no es un traidor, agrega su índice al arreglo generales_no_traidores.
            generales_no_traidores[num_generales_no_traidores++] = i;
        }
    }

    // Verifica que haya generales no traidores.
    if (num_generales_no_traidores == 0) {
        printf("Error: no hay generales no traidores.\n");
        return -1;
    }

    // Genera un índice aleatorio dentro del rango de generales no traidores.
    int indice_aleatorio = rand() % num_generales_no_traidores;

    // Devuelve el índice del general elegido como rey.
    return generales_no_traidores[indice_aleatorio];
}


// Función para imprimir el resultado de una ronda y si la votación es válida
void imprimirResultado(int ronda, General generales[], int rank, int size) {
    // Imprime el número de la ronda actual.
    printf("Ronda %d:\n", ronda);

    // Itera a través de todos los generales y muestra su ID, si es traidor y su voto.
    for (int i = 0; i < NUM_GENERALES; i++) {
        // Imprime el ID del general y si es traidor o no.
        printf("General %d (Traidor: %s) - Voto: %d\n", generales[i].id, generales[i].es_traidor ? "Sí" : "No", generales[i].voto);
    }

    // Verifica si la votación es válida utilizando la función esVotacionValida.
    if (esVotacionValida(generales, rank, size)) {
        // Si la votación es válida, imprime un mensaje indicando que es válida.
        printf("Votación válida.\n");
    } else {
        // Si la votación no es válida, imprime un mensaje indicando que no es válida.
        printf("Votación no válida.\n");
    }    
    // Imprime una línea en blanco para separar los resultados de diferentes rondas.
    printf("\n");
}

int main(int argc, char **argv) {
    srand(time(NULL));
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    General generales[NUM_GENERALES];

    if (rank == 0) {
        // Establecer la estrategia de voto para cada general (puede ajustarse según sea necesario)
        for (int i = 0; i < NUM_GENERALES; i++) {
            generales[i].id = i;
            generales[i].es_traidor = (i < NUM_TRAIDORES) ? 1 : 0;
            generales[i].voto = -1;  // -1 indica voto indefinido
            generales[i].mensaje = -1;  // -1 indica mensaje indefinido
            generales[i].estrategia = rand() % 2;  // Estrategia de voto aleatoria
        }
    }

    int ronda = 0;
    int rey = -1;

    while (ronda < MAX_RONDAS) {
        // Realiza una ronda de comunicación entre los generales.
        realizarRonda(generales, rank, size);
        // Imprime el resultado de la ronda actual y verifica si la votación es válida.
        imprimirResultado(ronda, generales, rank, size);

        // Si la votación es válida, se rompe el ciclo while.
        if (esVotacionValida(generales, rank, size)) {
            if (rank == 0) {
                printf("¡Votación válida alcanzada!\n");
            }
            break;
        } else {
            // Si no se alcanza un consenso, se elige un nuevo "rey" de entre los generales no traidores.
            rey = elegirRey(generales, rank, size);
            if (rank == 0) {
                printf("No se alcanzó consenso, el General %d es el nuevo rey.\n", rey);
            }
        }

        ronda++;  // Incrementa el número de la ronda.
    }

    // Si el límite de rondas (MAX_RONDAS) se alcanza sin llegar a un consenso, se muestra un mensaje.
    if (ronda >= MAX_RONDAS) {
        if (rank == 0) {
            printf("Se alcanzó el límite de rondas sin consenso.\n");
        }
    }

    MPI_Finalize();
    return 0;
}