#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NUM_GENERALES 10  // Número total de generales
#define NUM_TRAIDORES 3  // Número de nodos traidores
// #define MAX_RONDAS 100  // Número máximo de rondas antes de declarar un fracaso
#define F 1  // Número de generales traidores tolerados

// Estructura de un nodo
typedef struct {
    int id;
    int es_traidor;   
    int voto;  // 0 para retirada, 1 para ataque
    int mensaje;
     int estrategia;  // Estrategia de voto de los generales (0 para retirada, 1 para ataque)
    //  int plan;  // 0 para retirada, 1 para ataque
    // int resultado;  // 0 para retirada, 1 para ataque
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
//void ronda(General generales[]) {
    //for (int i = 0; i < NUM_GENERALES; i++) {
      //  // En esta implementación, la estrategia de voto de los generales se elige aleatoriamente
//        generales[i].voto = rand() % 2;
        //generales[i].mensaje = generales[i].voto;
    //}
//}

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

// Función para imprimir el resultado de una ronda y si la votación es válida
void imprimirResultado(int ronda, General generales[]) {
    printf("Ronda %d:\n", ronda);
    for (int i = 0; i < NUM_GENERALES; i++) {
        printf("General %d (Traidor: %s) - Voto: %d\n", generales[i].id, generales[i].es_traidor ? "Sí" : "No", generales[i].voto);
    }
    if (esVotacionValida(generales)) {
        printf("Votación válida.\n");
    } else {
        printf("Votación no válida.\n");
    }
    printf("\n");
}

// Función para determinar el plan inicial de un nodo
//int determinarPlanInicial() {
//    return rand() % 2;  // 0 para retirada, 1 para ataque
//}

// Función para simular el comportamiento de un nodo traidor
//int comportamientoTraidor() {
//return rand() % 2;  // Simulará un comportamiento aleatorio
//}



    // Crear nodos y asignarles un plan inicial y traidor o no
    //Nodo nodos[NUM_NODOS];
    //for (int i = 0; i < NUM_NODOS; i++) {
        //nodos[i].id = i;
    //    nodos[i].es_traidor = (i < NUM_TRAIDORES) ? 1 : 0;
    //    nodos[i].plan = determinarPlanInicial();
    //    nodos[i].resultado = -1;  // -1 indica resultado indefinido
    //}

    // int rondas = 0;
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

    while (ronda < 5) {
        realizarRonda(generales);
        imprimirResultado(ronda, generales);

        if (esVotacionValida(generales)) {
            printf("¡Votación válida alcanzada!\n");
            break;
        }

        ronda++;
    }

    if (ronda >= 5) {
        printf("Se alcanzó el límite de rondas sin consenso.\n");
    }

    return 0;
}

