#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>

#define ARRAY_SIZE 100


// Imprimimos el arrglo en bloques de 15
void print_array(int arr[], int size) {
    for (int i = 0; i < size; i++) {
        printf("%d ", arr[i]);
        if ((i + 1) % 15 == 0) {
            printf("\n");
        }
    }
    printf("\n");
}


void merge(int arr[], int left[], int left_size, int right[], int right_size) {
    int i = 0, j = 0, k = 0;
    // Compara los elementos de los subarreglos y coloca el menor en el arreglo resultante.
    while (i < left_size && j < right_size) {
        if (left[i] <= right[j]) {
            arr[k++] = left[i++];
        } else {
            arr[k++] = right[j++];
        }
    }
    // Agrega los elementos restantes del subarreglo izquierdo, si los hay.
    while (i < left_size) {
        arr[k++] = left[i++];
    }
    // Agrega los elementos restantes del subarreglo derecho, si los hay.
    while (j < right_size) {
        arr[k++] = right[j++];
    }
}

void merge_sort(int arr[], int size) {
    if (size > 1) {
        // Calcula el punto medio del arreglo.
        int mid = size / 2;

        // Crea subarreglos izquierdo y derecho.
        int left[mid];
        int right[size - mid];

        // Copia elementos al subarreglo izquierdo.
        for (int i = 0; i < mid; i++) {
            left[i] = arr[i];
        }

        // Copia elementos al subarreglo derecho.
        for (int i = mid; i < size; i++) {
            right[i - mid] = arr[i];
        }

        // Llamadas recursivas para ordenar los subarreglos.
        merge_sort(left, mid);
        merge_sort(right, size - mid);

        // Combina los subarreglos ordenados utilizando la función merge.
        merge(arr, left, mid, right, size - mid);
    }
}

int main() {
    int rank, size;
    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int array[ARRAY_SIZE];

    if (rank == 0) {
        // Inicializar la semilla del generador de números aleatorios con el tiempo actual
        srand(time(NULL));
        // 1: Generar un arreglo de números enteros aleatorios.
        for (int i = 0; i < ARRAY_SIZE; i++) {
            array[i] = rand() % 1000;
        }
        
        printf("\n -- -- -- --    Ordenamiento Distribuido     -- -- -- -- \n");

        // 2: Imprimir el arreglo original.
        printf("\n    Arreglo Original:\n\n");
        print_array(array, ARRAY_SIZE);
    }

    int local_size = ARRAY_SIZE / size;
    int local_array[local_size];

    // 3: Repartir el arreglo entre los nodos.
    MPI_Scatter(array, local_size, MPI_INT, local_array, local_size, MPI_INT, 0, MPI_COMM_WORLD);

    // 4: Ordenar el subarreglo local.
    merge_sort(local_array, local_size);

    // 5: Recopilar los resultados parciales.
    MPI_Gather(local_array, local_size, MPI_INT, array, local_size, MPI_INT, 0, MPI_COMM_WORLD);

    // 6: Imprimir el resultado final.
    if (rank == 0) {
        printf("\n    Arreglo Ordenado:\n\n");
        print_array(array, ARRAY_SIZE);
    }

    MPI_Finalize();

    return 0;
}
