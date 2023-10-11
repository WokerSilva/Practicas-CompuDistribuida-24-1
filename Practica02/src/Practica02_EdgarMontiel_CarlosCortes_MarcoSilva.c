#include <stdio.h>
#include <stdlib.h>
#include <time.h>

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
int esVotacionValida(General generales[]) {
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

    return (votos_ataque >= mayoria_requerida) || (votos_retirada >= mayoria_requerida);
}

// Función para realizar una ronda de comunicación
void realizarRonda(General generales[]) {
    // Itera a través de cada general en el arreglo generales.
    for (int i = 0; i < NUM_GENERALES; i++) {
        // Elige un voto aleatorio para el general.
        generales[i].voto = rand() % 2;
        // Asigna el voto aleatorio al mensaje del general.
        generales[i].mensaje = generales[i].voto;
    }
}

int elegirRey(General generales[]) {
    // Inicialmente, no hay un rey elegido.
    int rey = -1;      
    // Se utiliza para rastrear el ID más alto de los generales no traidores.
    int max_id = -1;    

    for (int i = 0; i < NUM_GENERALES; i++) {
        // Verifica si el general actual no es un traidor y tiene un ID mayor que el máximo encontrado hasta ahora.
        if (!generales[i].es_traidor && generales[i].id > max_id) {
            max_id = generales[i].id;  // Actualiza el máximo ID encontrado.
            rey = i;                  // Actualiza el general elegido como rey.
        }
    }

    return rey;  // Devuelve el índice del general elegido como rey.
}

// Función para imprimir el resultado de una ronda y si la votación es válida
void imprimirResultado(int ronda, General generales[]) {
    // Imprime el número de la ronda actual.
    printf("Ronda %d:\n", ronda);
    
    // Itera a través de todos los generales y muestra su ID, si es traidor y su voto.
    for (int i = 0; i < NUM_GENERALES; i++) {
        // Imprime el ID del general y si es traidor o no.
        printf("General %d (Traidor: %s) - Voto: %d\n", generales[i].id, generales[i].es_traidor ? "Sí" : "No", generales[i].voto);
    }
    
    // Verifica si la votación es válida utilizando la función esVotacionValida.
    if (esVotacionValida(generales)) {
        // Si la votación es válida, imprime un mensaje indicando que es válida.
        printf("Votación válida.\n");
    } else {
        // Si la votación no es válida, imprime un mensaje indicando que no es válida.
        printf("Votación no válida.\n");
    }    
    // Imprime una línea en blanco para separar los resultados de diferentes rondas.
    printf("\n");
}

int main() {
    srand(time(NULL));
    General generales[NUM_GENERALES];
    
    // Establecer la estrategia de voto para cada general (puede ajustarse según sea necesario)
    for (int i = 0; i < NUM_GENERALES; i++) {
        generales[i].id = i;
        generales[i].es_traidor = (i < NUM_TRAIDORES) ? 1 : 0;
        generales[i].voto = -1;  // -1 indica voto indefinido
        generales[i].mensaje = -1;  // -1 indica mensaje indefinido
        generales[i].estrategia = rand() % 2;  // Estrategia de voto aleatoria
    }

    int ronda = 0;
    int rey = -1;

    while (ronda < MAX_RONDAS) {
        // Realiza una ronda de comunicación entre los generales.
        realizarRonda(generales);
        // Imprime el resultado de la ronda actual y verifica si la votación es válida.
        imprimirResultado(ronda, generales);

        // Si la votación es válida, se rompe el ciclo while.
        if (esVotacionValida(generales)) {
            printf("¡Votación válida alcanzada!\n");
            break;
        } else {
            // Si no se alcanza un consenso, se elige un nuevo "rey" de entre los generales no traidores.
            rey = elegirRey(generales);
            printf("No se alcanzó consenso, el General %d es el nuevo rey.\n", rey);
        }

    ronda++;  // Incrementa el número de la ronda.
    }

    // Si el límite de rondas (MAX_RONDAS) se alcanza sin llegar a un consenso, se muestra un mensaje.
    if (ronda >= MAX_RONDAS) {
        printf("Se alcanzó el límite de rondas sin consenso.\n");
    }
   
    return 0;
}