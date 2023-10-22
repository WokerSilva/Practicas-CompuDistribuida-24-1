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

// Función para simular un timeout con probabilidad de fallo
bool simulaTimeout() {
    return (rand() % 10) < 3; // 30% de probabilidad de timeout
}

// Función para iniciar una elección
void inicioDeEleccIon(int myId, int totalNodos) {
    int lider = -1;
    bool soyLider = true;
    
    for (int i = myId + 1; i < totalNodos; i++) {
        if (!simulaTimeout()) {
            soyLider = false;
            break;
        }
    }
    
    if (soyLider) {
        lider = myId;
        printf("Soy tu padre. My ID: %d\n", lider);
    }
    
    // Lectura de los nodos
    for (int i = 0; i < totalNodos; i++) {
        printf("Nodo %d reporte: Estoy  %s. nuevo lider %d.\n", i, i == myId ? "vivo" : "muerto", lider);
    }
}

int main() {
    srand(time(NULL)); // Inicializar la semilla para simular fallos
    
    int myId = 2; // ID del nodo actual
    int totalNodos = 5; // Número total de nodos en el sistema
    
    printf("Nodo %d inicio de elección.\n", myId);
    
    inicioDeEleccIon(myId, totalNodos);
    
    return 0;
}
