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

#define NUM_NODOS 10 // Número total de nodos en el sistema

// Estructura de un nodo
typedef struct {
    int id;
    bool estaVivo;
    int lider;
} Nodo;

// Función para simular un timeout con probabilidad de fallo
bool simulaTimeout() {
    int randomValue = rand() % 100; // Genera un valor aleatorio entre 0 y 99
    return randomValue < 30; 
}

// Función para iniciar una elección
void iniciarElección(Nodo* nodos, int myId) {
    int lider = -1;
    bool soyConvocante = true;

    for (int i = myId + 1; i < NUM_NODOS; i++) {
        if (nodos[i].estaVivo && !simulaTimeout()) {
            soyConvocante = false;
            break;
        }
    }

    if (soyConvocante) {
        lider = myId;
        printf("Nodo %d: Soy el líder. Mi ID: %d\n", myId, lider);
    }

    nodos[myId].lider = lider;
}

int main() {
    srand(time(NULL)); // Inicializar la semilla para simular fallos

    Nodo nodos[NUM_NODOS];

    // Inicializar nodos
    for (int i = 0; i < NUM_NODOS; i++) {
        nodos[i].id = i;
        nodos[i].estaVivo = true;
        nodos[i].lider = -1;
    }

    int myId = 2; // ID del nodo actual

    printf("Nodo %d: Inicio de elección.\n", myId);

    iniciarElección(nodos, myId);

    // Reportar resultados
    for (int i = 0; i < NUM_NODOS; i++) {
        printf("Nodo %d: Estoy %s. Nuevo líder es Nodo %d.\n", i, nodos[i].estaVivo ? "vivo" : "muerto", nodos[i].lider);
    }

    return 0;
}