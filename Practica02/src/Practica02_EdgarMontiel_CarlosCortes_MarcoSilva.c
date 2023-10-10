#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NUM_NODOS 10  // Número total de nodos
#define NUM_TRAIDORES 3  // Número de nodos traidores
#define MAX_RONDAS 100  // Número máximo de rondas antes de declarar un fracaso

// Estructura de un nodo
typedef struct {
    int id;
    int es_traidor;
    int plan;  // 0 para retirada, 1 para ataque
    int resultado;  // 0 para retirada, 1 para ataque
} Nodo;

// Función para determinar el plan inicial de un nodo
int determinarPlanInicial() {
    return rand() % 2;  // 0 para retirada, 1 para ataque
}

// Función para simular el comportamiento de un nodo traidor
int comportamientoTraidor() {
    return rand() % 2;  // Simulará un comportamiento aleatorio
}

// Función para realizar una ronda de comunicación
void realizarRonda(Nodo nodos[]) {
    int votos_ataque = 0;
    int votos_retirada = 0;

    for (int i = 0; i < NUM_NODOS; i++) {
        if (!nodos[i].es_traidor) {
            int voto;
            if (nodos[i].plan == 1) {  // Nodo leal que planea atacar
                voto = 1;
            } else {  // Nodo leal que planea retirarse
                voto = 0;
            }

            if (nodos[i].es_traidor) {
                voto = comportamientoTraidor();  // Nodo traidor que vota aleatoriamente
            }

            if (voto == 1) {
                votos_ataque++;
            } else {
                votos_retirada++;
            }
        }
    }

    // Determinar el resultado de la ronda
    int consenso = (votos_ataque > votos_retirada) ? 1 : 0;

    // Actualizar el resultado de cada nodo
    for (int i = 0; i < NUM_NODOS; i++) {
        nodos[i].resultado = consenso;
    }
}

int main() {
    srand(time(NULL));

    // Crear nodos y asignarles un plan inicial y traidor o no
    Nodo nodos[NUM_NODOS];
    for (int i = 0; i < NUM_NODOS; i++) {
        nodos[i].id = i;
        nodos[i].es_traidor = (i < NUM_TRAIDORES) ? 1 : 0;
        nodos[i].plan = determinarPlanInicial();
        nodos[i].resultado = -1;  // -1 indica resultado indefinido
    }

    int rondas = 0;

    // Realizar rondas hasta que se alcance el consenso o se alcance el límite de rondas
    while (rondas < MAX_RONDAS) {
        realizarRonda(nodos);

        // Verificar si se alcanzó el consenso
        int consenso = nodos[0].resultado;
        int todos_iguales = 1;

        for (int i = 1; i < NUM_NODOS; i++) {
            if (nodos[i].resultado != consenso) {
                todos_iguales = 0;
                break;
            }
        }

        if (todos_iguales) {
            printf("Se alcanzó el consenso en la ronda %d. Resultado: %d\n", rondas, consenso);
            break;
        }

        rondas++;
    }

    if (rondas >= MAX_RONDAS) {
        printf("Se alcanzó el límite de rondas sin consenso.\n");
    }

    return 0;
}
