#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include "heuristicas.h"

/* =====================================================================
   FUNÇÕES AUXILIARES COMPARTILHADAS
   ===================================================================== */

// Soma os custos de todas as arestas do tour, fechando o ciclo no final
double custo(int *tour, int **matriz, int n) {
    double total = 0.0;
    for (int i = 0; i < n - 1; i++) {
        total += matriz[tour[i]][tour[i+1]];
    }
    total += matriz[tour[n-1]][tour[0]]; // fecha o ciclo: último -> primeiro
    return total;
}

void copia(int *dest, int *orig, int n) {
    for (int i = 0; i < n; i++)
        dest[i] = orig[i];
}

// Inverte o trecho do tour entre os índices i e j (inclusive)
void reverse(int *tour, int i, int j) {
    while (i < j) {
        int tmp = tour[i];
        tour[i] = tour[j];
        tour[j] = tmp;
        i++; j--;
    }
}

/* =====================================================================
   BUSCA LOCAL — 3-OPT
   Remove 3 arestas do tour e reconecta os segmentos da melhor forma.
   Itera até não haver mais melhorias (ótimo local).
   ===================================================================== */

/*
 * Aplica um dos 7 movimentos possíveis do 3-opt sobre os segmentos:
 *   A = [0 .. i-1]  |  B = [i .. j]  |  C = [j+1 .. k]  |  D = [k+1 .. n-1]
 * Tipos 1-3: inversão de um ou dois segmentos (equivalem a movimentos 2-opt).
 * Tipos 4-7: reordenação e/ou inversão de B e C (movimentos 3-opt puros).
 */
void aplica_3opt(int *tour, int n, int i, int j, int k, int tipo) {

    int *temp = (int *)malloc(n * sizeof(int));
    int idx = 0;

    if (tipo == 1) {
        copia(temp, tour, n);
        reverse(temp, i, j);           // A + B' + C + D
    }
    else if (tipo == 2) {
        copia(temp, tour, n);
        reverse(temp, j+1, k);         // A + B + C' + D
    }
    else if (tipo == 3) {
        copia(temp, tour, n);
        reverse(temp, i, j);
        reverse(temp, j+1, k);         // A + B' + C' + D
    }
    else if (tipo == 4) {
        for (int a = 0; a < i; a++)    temp[idx++] = tour[a];
        for (int a = j+1; a <= k; a++) temp[idx++] = tour[a];
        for (int a = i; a <= j; a++)   temp[idx++] = tour[a];
        for (int a = k+1; a < n; a++)  temp[idx++] = tour[a]; // A + C + B + D
    }
    else if (tipo == 5) {
        for (int a = 0; a < i; a++)    temp[idx++] = tour[a];
        for (int a = k; a > j; a--)    temp[idx++] = tour[a];
        for (int a = i; a <= j; a++)   temp[idx++] = tour[a];
        for (int a = k+1; a < n; a++)  temp[idx++] = tour[a]; // A + C' + B + D
    }
    else if (tipo == 6) {
        for (int a = 0; a < i; a++)    temp[idx++] = tour[a];
        for (int a = j+1; a <= k; a++) temp[idx++] = tour[a];
        for (int a = j; a >= i; a--)   temp[idx++] = tour[a];
        for (int a = k+1; a < n; a++)  temp[idx++] = tour[a]; // A + C + B' + D
    }
    else if (tipo == 7) {
        for (int a = 0; a < i; a++)    temp[idx++] = tour[a];
        for (int a = k; a > j; a--)    temp[idx++] = tour[a];
        for (int a = j; a >= i; a--)   temp[idx++] = tour[a];
        for (int a = k+1; a < n; a++)  temp[idx++] = tour[a]; // A + C' + B' + D
    }

    copia(tour, temp, n);
    free(temp);
}

/*
 * Gera solução inicial pelo critério do vizinho mais próximo (greedy):
 * a partir da origem, sempre visita a cidade mais próxima ainda não visitada.
 * Isso fornece um ponto de partida bem melhor do que uma rota sequencial.
 */
void solucao_inicial(int *tour, int **matriz, int n, int origem) {
    bool *visitado = (bool *)calloc(n, sizeof(bool));
    tour[0] = origem;
    visitado[origem] = true;

    for (int passo = 1; passo < n; passo++) {
        int atual = tour[passo - 1];
        int melhor_viz = -1;
        int melhor_dist = INF;

        for (int v = 0; v < n; v++) {
            if (!visitado[v] && matriz[atual][v] > 0 && matriz[atual][v] < melhor_dist) {
                melhor_dist = matriz[atual][v];
                melhor_viz = v;
            }
        }

        // Fallback: grafo desconectado — pega o primeiro nó ainda não visitado
        if (melhor_viz == -1) {
            for (int v = 0; v < n; v++) {
                if (!visitado[v]) { melhor_viz = v; break; }
            }
        }

        tour[passo] = melhor_viz;
        visitado[melhor_viz] = true;
    }

    free(visitado);
}

double buscaLocal(int **matriz, int num_nos, int origem) {

    int n = num_nos;

    int *tour        = (int *)malloc(n * sizeof(int));
    int *melhor_tour = (int *)malloc(n * sizeof(int));
    int *candidato   = (int *)malloc(n * sizeof(int));

    solucao_inicial(tour, matriz, n, origem);
    double melhor_custo = custo(tour, matriz, n);

    int melhorou = 1;

    // Repete enquanto algum movimento 3-opt ainda trouxer melhoria
    while (melhorou) {
        melhorou = 0;

        for (int i = 1; i < n - 4; i++) {
            for (int j = i + 1; j < n - 2; j++) {
                for (int k = j + 1; k < n - 1; k++) {

                    // Testa os 7 movimentos possíveis para este trio de pontos de corte
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

        if (melhorou) copia(tour, melhor_tour, n);
    }

    free(candidato);
    free(melhor_tour);
    free(tour);

    return melhor_custo;
}

/* =====================================================================
   ALGORITMO EVOLUTIVO — Algoritmo Genético
   Representação: permutação de cidades (tour).
   Seleção: torneio  |  Cruzamento: PMX  |  Mutação: swap
   ===================================================================== */

#define TAM_POPULACAO 200
#define NUM_GERACOES  5000
#define TAXA_MUTACAO  0.25

// Gera um indivíduo (tour) aleatório preservando a origem no índice 0
void embaralhar_genes(int *tour, int n, int origem) {
    int idx = 0;
    tour[idx++] = origem;
    for (int i = 0; i < n; i++) {
        if (i != origem) tour[idx++] = i;
    }
    // Fisher-Yates a partir do índice 1 para não mexer na origem
    for (int i = n - 1; i > 1; i--) {
        int j = 1 + (rand() % i);
        int tmp = tour[i]; tour[i] = tour[j]; tour[j] = tmp;
    }
}

// Torneio com 3 participantes: retorna o índice do indivíduo com menor custo
int selecao_torneio(double *fitness, int tam_pop) {
    int melhor = rand() % tam_pop;
    for (int i = 0; i < 4; i++) { // antes era 2
        int candidato = rand() % tam_pop;
        if (fitness[candidato] < fitness[melhor])
            melhor = candidato;
    }
    return melhor;
}

bool ja_existe(int *filho, int inicio, int fim, int cidade) {
    for (int i = inicio; i <= fim; i++) {
        if (filho[i] == cidade) return true;
    }
    return false;
}

/*
 * PMX (Partially Mapped Crossover):
 * 1. Copia um segmento aleatório do pai1 para o filho.
 * 2. Preenche as posições restantes com genes do pai2, resolvendo
 *    conflitos via mapeamento. Se o mapeamento entrar em ciclo,
 *    um fallback busca o primeiro gene ainda disponível.
 */
void crossover_pmx(int *pai1, int *pai2, int *filho, int n) {
    for (int i = 0; i < n; i++) filho[i] = -1;
    filho[0] = pai1[0]; // origem sempre preservada

    int corte1 = 1 + (rand() % (n - 1));
    int corte2 = 1 + (rand() % (n - 1));
    if (corte1 > corte2) { int tmp = corte1; corte1 = corte2; corte2 = tmp; }

    // Segmento herdado diretamente do pai1
    for (int i = corte1; i <= corte2; i++) filho[i] = pai1[i];

    // Demais posições: tenta usar o gene do pai2, mapeando se houver colisão
    for (int i = 1; i < n; i++) {
        if (i >= corte1 && i <= corte2) continue;

        int gene = pai2[i];
        if (!ja_existe(filho, corte1, corte2, gene)) {
            filho[i] = gene;
        } else {
            int max_iter = n; // guarda contra ciclo infinito
            while (ja_existe(filho, corte1, corte2, gene) && max_iter-- > 0) {
                for (int j = corte1; j <= corte2; j++) {
                    if (pai1[j] == gene) { gene = pai2[j]; break; }
                }
            }
            // Fallback: ciclo não resolvido — usa o primeiro gene livre
            if (ja_existe(filho, corte1, corte2, gene) || ja_existe(filho, 0, n-1, gene)) {
                for (int k = 0; k < n; k++) {
                    if (!ja_existe(filho, 0, n-1, k) && k != filho[0]) { gene = k; break; }
                }
            }
            filho[i] = gene;
        }
    }
}

// Troca dois genes aleatórios (exceto a origem) com probabilidade TAXA_MUTACAO
void mutacao_swap(int *tour, int n) {
    if ((double)rand() / RAND_MAX < TAXA_MUTACAO) {
        int i = 1 + (rand() % (n - 1));
        int j = 1 + (rand() % (n - 1));
        int tmp = tour[i]; tour[i] = tour[j]; tour[j] = tmp;

        // mutação extra ocasional
        if ((double)rand() / RAND_MAX < 0.3) {
            i = 1 + (rand() % (n - 1));
            j = 1 + (rand() % (n - 1));
            tmp = tour[i]; tour[i] = tour[j]; tour[j] = tmp;
        }
    }
}

double evolutivo(int **matriz, int num_nos, int origem) {
    int n = num_nos;

    int    **populacao      = (int **)malloc(TAM_POPULACAO * sizeof(int *));
    int    **nova_populacao = (int **)malloc(TAM_POPULACAO * sizeof(int *));
    double  *fitness        = (double *)malloc(TAM_POPULACAO * sizeof(double));

    // Inicializa população
    for (int i = 0; i < TAM_POPULACAO; i++) {
        populacao[i]      = (int *)malloc(n * sizeof(int));
        nova_populacao[i] = (int *)malloc(n * sizeof(int));
        solucao_inicial(populacao[i], matriz, n, origem);
        // pequena perturbação pra gerar diversidade
        for (int k = 0; k < n / 10; k++){
            int a = 1 + rand() % (n - 1);
            int b = 1 + rand() % (n - 1);
            int tmp = populacao[i][a];
            populacao[i][a] = populacao[i][b];
            populacao[i][b] = tmp;
        }
        fitness[i] = custo(populacao[i], matriz, n);
    }

    double melhor_custo_global = INF;

    for (int g = 0; g < NUM_GERACOES; g++) {
        if (g % 100 == 0) {
            for (int i = TAM_POPULACAO / 2; i < TAM_POPULACAO; i++) {
                embaralhar_genes(populacao[i], n, origem);
                fitness[i] = custo(populacao[i], matriz, n); // ← ESSENCIAL
            }
        }

        // =============================
        // Encontrar os dois melhores
        // =============================
        int melhor1 = 0, melhor2 = 1;

        if (fitness[melhor2] < fitness[melhor1]) {
            int tmp = melhor1;
            melhor1 = melhor2;
            melhor2 = tmp;
        }

        for (int i = 2; i < TAM_POPULACAO; i++) {
            if (fitness[i] < fitness[melhor1]) {
                melhor2 = melhor1;
                melhor1 = i;
            } else if (fitness[i] < fitness[melhor2]) {
                melhor2 = i;
            }
        }

        // Atualiza melhor global
        if (fitness[melhor1] < melhor_custo_global) {
            melhor_custo_global = fitness[melhor1];
        }

        // =============================
        // Elitismo
        // =============================
        copia(nova_populacao[0], populacao[melhor1], n);
        copia(nova_populacao[1], populacao[melhor2], n);

        // =============================
        // Gerar nova população
        // =============================
        for (int i = 2; i < TAM_POPULACAO; i++) {
            int pai1 = selecao_torneio(fitness, TAM_POPULACAO);
            int pai2 = selecao_torneio(fitness, TAM_POPULACAO);

            crossover_pmx(populacao[pai1], populacao[pai2], nova_populacao[i], n);
            mutacao_swap(nova_populacao[i], n);
        }

        // =============================
        // Atualiza população
        // =============================
        for (int i = 0; i < TAM_POPULACAO; i++) {
            copia(populacao[i], nova_populacao[i], n);
            fitness[i] = custo(populacao[i], matriz, n);
        }
    }

    // Libera memória
    for (int i = 0; i < TAM_POPULACAO; i++) {
        free(populacao[i]);
        free(nova_populacao[i]);
    }
    free(populacao);
    free(nova_populacao);
    free(fitness);

    return melhor_custo_global;
}