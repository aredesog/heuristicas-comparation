#ifndef HEURISTICAS_H
#define HEURISTICAS_H

#include <limits.h>
#include <stdbool.h>

#define INF INT_MAX

//Executa a heurística do algoritmo de busca local a partir de um nó de origem
double buscaLocal(int **matriz, int num_nos, int origem);

//Executa a heurística do algoritmo evolutivo a partir de um nó de origem
double evolutivo(int **matriz, int num_nos, int origem);

#endif