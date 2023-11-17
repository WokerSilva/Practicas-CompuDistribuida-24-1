#include <stdio.h>
#include <stdlib.h>

#define ARRAY_SIZE 100

void print_array(int arr[], int size) {
    for (int i = 0; i < size; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

int main() {
    // 1: Generar un arreglo de números enteros aleatorios.
    int array[ARRAY_SIZE]; // 1.1: Definir el tamaño del arreglo.
    for (int i = 0; i < ARRAY_SIZE; i++) { // 1.2: Para cada posición en el arreglo:
        array[i] = rand() % 1000; // Generar un número entero aleatorio y asignarlo a esa posición.
    }

    // 2: Imprimir el arreglo original.
    print_array(array, ARRAY_SIZE); 

    
}