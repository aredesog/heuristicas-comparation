#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "heuristicas.h"

//Heurística de algorítimo de busca local
// ----------------------------
// Calcula custo do tour
// ----------------------------
double custo(int *tour, int **matriz, int n) {
    double total = 0.0;
    for (int i = 0; i < n - 1; i++) {
        total += matriz[tour[i]][tour[i+1]];
    }
    total += matriz[tour[n-1]][tour[0]]; // fecha ciclo
    return total;
}

// ----------------------------
// Copia vetor
// ----------------------------
void copia(int *dest, int *orig, int n) {
    for (int i = 0; i < n; i++)
        dest[i] = orig[i];
}

// ----------------------------
// Inverte segmento [i, j]
// ----------------------------
void reverse(int *tour, int i, int j) {
    while (i < j) {
        int tmp = tour[i];
        tour[i] = tour[j];
        tour[j] = tmp;
        i++; j--;
    }
}

// ----------------------------
// Aplica movimentos 3-opt
// ----------------------------
void aplica_3opt(int *tour, int n, int i, int j, int k, int tipo) {

    int *temp = (int *)malloc(n * sizeof(int));
    int idx = 0;

    // Segmentos:
    // A = [0 .. i-1]
    // B = [i .. j]
    // C = [j+1 .. k]
    // D = [k+1 .. n-1]

    if (tipo == 1) {
        // reverse B
        copia(temp, tour, n);
        reverse(temp, i, j);
    }
    else if (tipo == 2) {
        // reverse C
        copia(temp, tour, n);
        reverse(temp, j+1, k);
    }
    else if (tipo == 3) {
        // reverse B e C
        copia(temp, tour, n);
        reverse(temp, i, j);
        reverse(temp, j+1, k);
    }
    else if (tipo == 4) {
        // C + B
        for (int a = 0; a < i; a++) temp[idx++] = tour[a];
        for (int a = j+1; a <= k; a++) temp[idx++] = tour[a];
        for (int a = i; a <= j; a++) temp[idx++] = tour[a];
        for (int a = k+1; a < n; a++) temp[idx++] = tour[a];
    }
    else if (tipo == 5) {
        // C invertido + B
        for (int a = 0; a < i; a++) temp[idx++] = tour[a];
        for (int a = k; a > j; a--) temp[idx++] = tour[a];
        for (int a = i; a <= j; a++) temp[idx++] = tour[a];
        for (int a = k+1; a < n; a++) temp[idx++] = tour[a];
    }
    else if (tipo == 6) {
        // C + B invertido
        for (int a = 0; a < i; a++) temp[idx++] = tour[a];
        for (int a = j+1; a <= k; a++) temp[idx++] = tour[a];
        for (int a = j; a >= i; a--) temp[idx++] = tour[a];
        for (int a = k+1; a < n; a++) temp[idx++] = tour[a];
    }
    else if (tipo == 7) {
        // C invertido + B invertido
        for (int a = 0; a < i; a++) temp[idx++] = tour[a];
        for (int a = k; a > j; a--) temp[idx++] = tour[a];
        for (int a = j; a >= i; a--) temp[idx++] = tour[a];
        for (int a = k+1; a < n; a++) temp[idx++] = tour[a];
    }

    copia(tour, temp, n);
    free(temp);
}

// ----------------------------
// Gera solução inicial (simples)
// ----------------------------
void solucao_inicial(int *tour, int n, int origem) {
    int idx = 0;

    tour[idx++] = origem;

    for (int i = 0; i < n; i++) {
        if (i != origem) {
            tour[idx++] = i;
        }
    }
}

// ----------------------------
// BUSCA LOCAL 3-OPT
// ----------------------------
double buscaLocal(int **matriz, int num_nos, int origem) {

    int n = num_nos;

    int *tour = (int *)malloc(n * sizeof(int));
    int *melhor_tour = (int *)malloc(n * sizeof(int));
    int *candidato = (int *)malloc(n * sizeof(int));

    // solução inicial
    solucao_inicial(tour, n, origem);

    double melhor_custo = custo(tour, matriz, n);

    int melhorou = 1;

    while (melhorou) {
        melhorou = 0;

        for (int i = 1; i < n - 4; i++) {
            for (int j = i + 1; j < n - 2; j++) {
                for (int k = j + 1; k < n - 1; k++) {

                    for (int tipo = 1; tipo <= 7; tipo++) {

                        copia(candidato, tour, n);

                        aplica_3opt(candidato, n, i, j, k, tipo);

                        double c = custo(candidato, matriz, n);

                        if (c < melhor_custo) {
                            melhor_custo = c;
                            copia(melhor_tour, candidato, n);
                            melhorou = 1;
                        }
                    }
                }
            }
        }

        if (melhorou) {
            copia(tour, melhor_tour, n);
        }
    }

    free(candidato);
    free(melhor_tour);
    free(tour);

    return melhor_custo;
}

/*------------------------------------------------------------------------------------------------------------------------------------*/

//Heurística de algorítimo devolutivo
//(Implemetar Daqui para baixo).