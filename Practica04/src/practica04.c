#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define ARRAY_SIZE 100

void print_array(int arr[], int size) {
    for (int i = 0; i < size; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

void merge(int arr[], int left[], int left_size, int right[], int right_size) {
    int i = 0, j = 0, k = 0;

    while (i < left_size && j < right_size) {
        if (left[i] <= right[j]) {
            arr[k++] = left[i++];
        } else {
            arr[k++] = right[j++];
        }
    }

    while (i < left_size) {
        arr[k++] = left[i++];
    }

    while (j < right_size) {
        arr[k++] = right[j++];
    }
}

void merge_sort(int arr[], int size) {
    if (size > 1) {
        int mid = size / 2;
        int left[mid];
        int right[size - mid];

        for (int i = 0; i < mid; i++) {
            left[i] = arr[i];
        }

        for (int i = mid; i < size; i++) {
            right[i - mid] = arr[i];
        }

        merge_sort(left, mid);
        merge_sort(right, size - mid);
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
        // 1: Generar un arreglo de números enteros aleatorios.
        for (int i = 0; i < ARRAY_SIZE; i++) {
            array[i] = rand() % 1000;
        }

        // 2: Imprimir el arreglo original.
        printf("Arreglo Original:\n");
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
        printf("Arreglo Ordenado:\n");
        print_array(array, ARRAY_SIZE);
    }

    MPI_Finalize();

    return 0;
}
