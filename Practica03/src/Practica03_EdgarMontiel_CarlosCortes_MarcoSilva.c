// PRACTICA 03 - Computación Distribuida
//        Algoritmo Abusión
// Alumnos:
//  Marco Silva Huerta 316205326
//  Edgar Montiel Ledesma 317317794
//  Carlos Daniel Cortes Jimenez 420004846

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <unistd.h> // Necesario para sleep() en algunas plataformas
<<<<<<< HEAD
#include <mpi.h> // Incluimos MPI
#define NUM_NODOS 25  // Número total de nodos en el sistema
=======

#define NUM_NODOS 25 // Número total de nodos en el sistema
>>>>>>> testMarcoP3

// Estructura de un nodo
typedef struct {
    int id;
    bool estaVivo;
    int lider;
    bool esperandoRespuesta; // Nueva variable para indicar si se está esperando una respuesta
} Nodo;

// Función para simular un timeout con probabilidad de fallo
bool simulaTimeout() {
    return (rand() % 100) < 30; // 30% de probabilidad de timeout
}
<<<<<<< HEAD
// Función para iniciar una elección
void iniciarElección(Nodo* nodos, int myId, int rank, int size) {
=======

// Función para iniciar una elección
void iniciarElección(Nodo* nodos, int myId) {
>>>>>>> testMarcoP3
    // Inicializa una variable para el líder como -1 (sin líder).
    int lider = -1;
    // Inicializa una variable booleana para indicar si este nodo es el convocante como verdadero.
    bool soyConvocante = true;
    // Inicializa una variable booleana para indicar si este nodo está esperando un coordinador como falso.
    bool esperandoCoordinador = false;

    // Recorre todos los nodos con identificadores más altos que el propio.
    for (int i = myId + 1; i < NUM_NODOS; i++) {
        // Verifica si el nodo i está vivo y si no se simula un timeout
        // Si ambas condiciones son verdaderas, significa que el nodo i responde a la elección.
        if (nodos[i].estaVivo && !simulaTimeout()) {
            // Establece la variable como falso porque al menos un nodo responde.
            soyConvocante = false; 
            break; // Sale del bucle para no ver más nodos 
        }
    }

    if (soyConvocante) {
        lider = myId;
        printf("Nodo %d: Soy el líder. Mi ID: %d\n", myId, lider);
    } else {
        for (int i = myId + 1; i < NUM_NODOS; i++) {
            if (nodos[i].estaVivo) {
                // Envía mensaje de elección a procesos con id mayor
                printf("Nodo %d: Envía mensaje elección a Nodo %d\n", myId, i);
<<<<<<< HEAD
                MPI_Send(&myId, 1, MPI_INT, i % size, 0, MPI_COMM_WORLD);
                // Espera por un mensaje de respuesta (simulado)
                sleep(1);

                int respuesta;
                MPI_Status status;
                if (!simulaTimeout()) {
                    MPI_Recv(&respuesta, 1, MPI_INT, i % size, 0, MPI_COMM_WORLD, &status);
                    // Recibe un mensaje de respuesta
                    esperandoCoordinador = true;
                    nodos[i].esperandoRespuesta = true;
=======
                // Espera por un mensaje de respuesta (simulado)
                sleep(1); // Simula la espera de respuesta
                if (!simulaTimeout()) {
                    // Recibe un mensaje de respuesta
                    esperandoCoordinador = true;
                    nodos[i].esperandoRespuesta = true; // Marcar el nodo como esperando una respuesta
>>>>>>> testMarcoP3
                    printf("Nodo %d: Recibió mensaje de respuesta de Nodo %d\n", myId, i);
                    break;
                }
            }
        }
    }

    if (esperandoCoordinador) {
        // Espera por un mensaje de coordinador
        sleep(2); // Simula la espera de coordinador
        if (!simulaTimeout()) {
            // Recibe un mensaje de coordinador
<<<<<<< HEAD
            for (int i = 0; i < size; i++) {
                if (i != rank) {
                    MPI_Send(&myId, 1, MPI_INT, i, 1, MPI_COMM_WORLD);
                }
            }
=======
>>>>>>> testMarcoP3
            lider = myId; // Este nodo es el líder
            printf("Nodo %d: Soy el líder. Mi ID: %d\n", myId, lider);
        }
    }

    for (int i = 0; i < NUM_NODOS; i++) {
        if (nodos[i].esperandoRespuesta) {
            // Restablecer el estado de espera de respuesta de los nodos
            nodos[i].esperandoRespuesta = false;
        }
        nodos[i].lider = lider;
    }
}

<<<<<<< HEAD
int main(int argc, char** argv) {
    srand(time(NULL));  // Inicializar la semilla para simular fallos

    int rank, size;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
=======
int main() {
    srand(time(NULL)); // Inicializar la semilla para simular fallos
>>>>>>> testMarcoP3

    Nodo nodos[NUM_NODOS];

    // Inicializar nodos
    for (int i = 0; i < NUM_NODOS; i++) {
        nodos[i].id = i;
        nodos[i].estaVivo = true;
        nodos[i].lider = -1;
    }

    int myId = 2; // ID del nodo actual

    printf("Nodo %d: Inicio de elección.\n", myId);

<<<<<<< HEAD
    iniciarElección(nodos, myId, rank, size);
=======
    iniciarElección(nodos, myId);
>>>>>>> testMarcoP3

    // Reportar resultados
    for (int i = 0; i < NUM_NODOS; i++) {
        printf("Nodo %d: Estoy %s. Nuevo líder es Nodo %d.\n", i, nodos[i].estaVivo ? "vivo" : "muerto", nodos[i].lider);
    }

<<<<<<< HEAD
    MPI_Finalize();

=======
>>>>>>> testMarcoP3
    return 0;
}